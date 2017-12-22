#ifndef STEALTH_NOISE_GENERATOR_H
#define STEALTH_NOISE_GENERATOR_H
#define CURRENT_TIME std::chrono::system_clock::now().time_since_epoch().count()
#include "Noise/NoiseGenerator.hpp"
#include "Noise/InterpolationKernel.hpp"
#include "TileMap/TileMap.hpp"
#include "Utility.hpp"
#include <chrono>
#include <random>
#include <any>
#include <cmath>

namespace StealthWorldGenerator {
    namespace {
        template <int rows, int cols>
        using NoiseMap = TileMap<float, rows, cols>;

        template <int rows, int cols>
        using InternalNoiseMap = TileMap<float, rows, cols>;

        // Initialize with random values according to provided distribution
        template <int internalRows, int internalCols, typename Distribution, typename Generator>
        constexpr InternalNoiseMap<internalRows, internalCols> generateInternalNoiseMap(Distribution& distribution, Generator& generator) {
            // Internal noise map should be large enough to fit tiles of size (scale, scale).
            InternalNoiseMap<internalRows, internalCols> internalNoiseMap{};
            for (int i = 0; i < internalRows; ++i) {
                for (int j = 0; j < internalCols; ++j) {
                    float val = distribution(generator);
                    internalNoiseMap.at(i, j) = val;
                }
            }
            return internalNoiseMap;
        }

        constexpr float interpolate(float topLeft, float topRight, float bottomLeft, float bottomRight, const Vector2f& attenuation) noexcept {
            // Interpolate horizontally
            float nx0 = topLeft * (1.0f - attenuation.x) + topRight * (attenuation.x);
            float nx1 = bottomLeft * (1.0f - attenuation.x) + bottomRight * (attenuation.x);
            // Interpolate vertically
            float nxy = nx0 * (1.0f - attenuation.y) + nx1 * (attenuation.y);
            // Return a random value in the range (0, 1) instead of (-sqrt(2) / 2, sqrt(2) / 2)
            return nxy;
        }


        template <int scale, int internalRows, int internalCols, int rows, int cols>
        constexpr void fillTile(int internalRow, int internalCol, int scaledRow, int scaledCol,
            const InternalNoiseMap<internalRows, internalCols>* internalNoise,
            NoiseMap<rows, cols>* generatedNoise, const InterpolationKernel<scale>* kernel) {
            // Only fill the part of the tile that is valid.
            const int maxValidRow = std::min(rows - scaledRow, scale);
            const int maxValidCol = std::min(cols - scaledCol, scale);
            // Cache local gradient vectors
            float topLeft = internalNoise -> at(internalRow, internalCol);
            float topRight = internalNoise -> at(internalRow, internalCol + 1);
            float bottomLeft = internalNoise -> at(internalRow + 1, internalCol);
            float bottomRight = internalNoise -> at(internalRow + 1, internalCol + 1);
            // Cache points and attenuations TileMaps
            const auto& attenuations = kernel -> getAttenuations();
            // Loop over one interpolation kernel tile.
            for (int row = 0; row < maxValidRow; ++row) {
                for (int col = 0; col < maxValidCol; ++col) {
                    // Interpolate based on the 4 surrounding internal noise points.
                    generatedNoise -> at(scaledRow + row, scaledCol + col) = interpolate(topLeft,
                        topRight, bottomLeft, bottomRight, attenuations.at(row, col));
                }
            }
        }
    } /* namespace */

    // Scale maps one pixel of the generated noise to n pixels of the output.
    class StealthNoiseGenerator : public NoiseGenerator {
        public:
            // Create octaved noise
            template <int rows, int cols, int scale, int numOctaves, typename Distribution = std::uniform_real_distribution<float>,
                typename Generator = std::default_random_engine>
            constexpr NoiseMap<rows, cols> generateOctaves(float multiplier = 0.5f, float decayFactor = 0.5f, Distribution distribution
                = std::uniform_real_distribution(0.0f, 1.0f), Generator generator = std::default_random_engine(CURRENT_TIME)) {
                if constexpr (numOctaves == 1) {
                    // This multiplier should equal the last one if this is the final octave.
                    return (multiplier / decayFactor) * generate<rows, cols, scale>(distribution, generator);
                } else {
                    return multiplier * generate<rows, cols, scale>(distribution, generator)
                    + generateOctaves<rows, cols, ceilDivide(scale, 2), numOctaves - 1>(multiplier * decayFactor, decayFactor, distribution, generator);
                }
            }

            // Create the smoothed noise
            template <int rows, int cols, int scale = 1, typename Distribution = std::uniform_real_distribution<float>,
                typename Generator = std::default_random_engine>
            constexpr NoiseMap<rows, cols> generate(Distribution distribution = std::uniform_real_distribution(0.0f, 1.0f),
                Generator generator = std::default_random_engine(CURRENT_TIME)) {
                // Generate a new interpolation kernel if one does not exist.
                if (kernels.count(scale) < 1) {
                    kernels.emplace(scale, InterpolationKernel<scale>{});
                }
                const InterpolationKernel<scale>& kernel = std::any_cast<const InterpolationKernel<scale>>(kernels[scale]);
                // Generate a new internal noise map.
                constexpr int internalRows = ceilDivide(rows, scale) + 1;
                constexpr int internalCols = ceilDivide(cols, scale) + 1;
                InternalNoiseMap<internalRows, internalCols> internalNoiseMap
                    = generateInternalNoiseMap<internalRows, internalCols>(distribution, generator);
                // Return noise map.
                NoiseMap<rows, cols> generatedNoise;
                // Create threads
                for (int i = 0; i < NUM_THREADS; ++i) {
                    noiseThreads[i] = std::thread{&StealthNoiseGenerator::fillRows<internalRows, internalCols, rows, cols, scale>,
                        this, i, &internalNoiseMap, &generatedNoise, &kernel};
                }
                // Wait for all threads to finish
                for (auto& thread : noiseThreads) {
                    thread.join();
                }
                return generatedNoise;
            }
        private:
            template <int internalRows, int internalCols, int rows, int cols, int scale>
            constexpr void fillRows(int id, InternalNoiseMap<internalRows, internalCols>* internalNoiseMap,
                NoiseMap<rows, cols>* generatedNoise, const InterpolationKernel<scale>* kernel) {
                constexpr int portionSize = ceilDivide(internalRows - 1, NUM_THREADS);
                const int start = id * portionSize;
                const int end = std::min(start + portionSize, internalRows - 1);
                // Keep track of coordinates on the generated noise map.
                int scaledRow = start * scale;
                for (int i = start; i < end; ++i) {
                    int scaledCol = 0;
                    for (int j = 0; j < internalCols - 1; ++j) {
                        // Fill a tile of size scale x scale.
                        fillTile<scale>(i, j, scaledRow, scaledCol, internalNoiseMap, generatedNoise, kernel);
                        scaledCol += scale;
                    }
                    scaledRow += scale;
                }
            }

    };
} /* StealthWorldGenerator */

#endif
