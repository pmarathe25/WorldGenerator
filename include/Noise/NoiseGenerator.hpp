#ifndef STEALTH_NOISE_GENERATOR_H
#define STEALTH_NOISE_GENERATOR_H
#define CURRENT_TIME std::chrono::system_clock::now().time_since_epoch().count()
#include "Noise/InterpolationKernel.hpp"
#include "TileMap/TileMap.hpp"
#include "Utility.hpp"
#include <chrono>
#include <random>
#include <any>
#include <unordered_map>

namespace StealthWorldGenerator {
    template <int rows, int cols>
    using NoiseMap = TileMap<float, rows, cols>;

    template <int rows, int cols>
    using InternalNoiseMap = TileMap<Vector2f, rows, cols>;

    // Initialize with random values according to provided distribution
    template <int internalRows, int internalCols, typename Distribution, typename Generator>
    constexpr InternalNoiseMap<internalRows, internalCols> generateInternalNoiseMap(Distribution& distribution, Generator& generator) {
        // Internal noise map should be large enough to fit tiles of size (scale, scale).
        InternalNoiseMap<internalRows, internalCols> internalNoiseMap{};
        for (int i = 0; i < internalRows; ++i) {
            for (int j = 0; j < internalCols; ++j) {
                float angle = distribution(generator);
                internalNoiseMap.at(i, j) = Vector2f{(float) cos(angle), (float) sin(angle)};
            }
        }
        return internalNoiseMap;
    }

    constexpr float interpolatePerlin(const Vector2f& topLeft, const Vector2f& topRight, const Vector2f& bottomLeft,
        const Vector2f& bottomRight, const Vector2f& point, const Vector2f& attenuation) noexcept {
        // Compute dot products.
        float n00 = topLeft.y * point.y + topLeft.x * point.x;
        float n01 = topRight.y * point.y + topRight.x * (point.x - 1.0f);
        float n10 = bottomLeft.y * (point.y - 1.0f) + bottomLeft.x * point.x;
        float n11 = bottomRight.y * (point.y - 1.0f) + bottomRight.x * (point.x - 1.0f);
        // Interpolate horizontally
        float nx0 = n00 * (1.0f - attenuation.x) + n01 * (attenuation.x);
        float nx1 = n10 * (1.0f - attenuation.x) + n11 * (attenuation.x);
        // Interpolate vertically
        float nxy = nx0 * (1.0f - attenuation.y) + nx1 * (attenuation.y);
        // Return a random value in the range (0, 1) instead of (-0.75, 0.75)
        return (nxy + 0.75f) / 1.5f;
    }

    // Scale maps one pixel of the generated noise to n pixels of the output.
    class NoiseGenerator {
        public:
            // Create octaved noise
            template <int rows, int cols, int scale, int numOctaves, typename Distribution = std::uniform_real_distribution<float>,
                typename Generator = std::default_random_engine>
            constexpr NoiseMap<rows, cols> generateOctaves(float multiplier = 0.5f, float decayFactor = 0.5f, Distribution distribution
                = std::uniform_real_distribution(0.0f, TAU), Generator generator = std::default_random_engine(CURRENT_TIME)) {
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
            constexpr NoiseMap<rows, cols> generate(Distribution distribution = std::uniform_real_distribution(0.0f, TAU),
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
                    noiseThreads[i] = std::thread{&NoiseGenerator::fillRows<internalRows, internalCols, rows, cols, scale>,
                        this, i, &internalNoiseMap, &generatedNoise, &kernel};
                }
                // Wait for all threads to finish
                for (auto& thread : noiseThreads) {
                    thread.join();
                }
                return generatedNoise;
            }
        private:
            std::array<std::thread, NUM_THREADS> noiseThreads;
            // Maintain a cache of interpolation kernels of different sizes
            static std::unordered_map<int, std::any> kernels;

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

            template <int scale, int internalRows, int internalCols, int rows, int cols>
            constexpr void fillTile(int internalRow, int internalCol, int scaledRow, int scaledCol,
                const InternalNoiseMap<internalRows, internalCols>* internalNoise, NoiseMap<rows, cols>* generatedNoise,
                const InterpolationKernel<scale>* kernel) const {
                // Only fill the part of the tile that is valid.
                const int maxValidRow = std::min(rows - scaledRow, scale);
                const int maxValidCol = std::min(cols - scaledCol, scale);
                // Cache local gradient vectors
                const Vector2f& topLeft = internalNoise -> at(internalRow, internalCol);
                const Vector2f& topRight = internalNoise -> at(internalRow, internalCol + 1);
                const Vector2f& bottomLeft = internalNoise -> at(internalRow + 1, internalCol);
                const Vector2f& bottomRight = internalNoise -> at(internalRow + 1, internalCol + 1);
                // Cache points and attenuations TileMaps
                const auto& points = kernel -> getPoints();
                const auto& attenuations = kernel -> getAttenuations();
                // Loop over one interpolation kernel tile.
                for (int row = 0; row < maxValidRow; ++row) {
                    for (int col = 0; col < maxValidCol; ++col) {
                        // Interpolate based on the 4 surrounding internal noise points.
                        generatedNoise -> at(scaledRow + row, scaledCol + col) = interpolatePerlin(topLeft,
                            topRight, bottomLeft, bottomRight, points.at(row, col), attenuations.at(row, col));
                    }
                }
            }
    };

    std::unordered_map<int, std::any> NoiseGenerator::kernels{};
} /* StealthWorldGenerator */

#endif
