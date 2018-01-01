#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H
#define CURRENT_TIME std::chrono::system_clock::now().time_since_epoch().count()
#include "config.hpp"
#include "Noise/InterpolationKernel.hpp"
#include "TileMap/TileMap.hpp"
#include "Utility.hpp"
#include <unordered_map>
#include <chrono>
#include <random>
#include <thread>

namespace StealthWorldGenerator {
    class NoiseGenerator {
        public:
            // Create octaved noise
            template <int width, int length, int scale, int numOctaves, typename Distribution = std::uniform_real_distribution<float>,
                typename Generator = std::default_random_engine>
            constexpr StealthTileMap::TileMapF<width, length> generateOctaves(Distribution distribution = std::uniform_real_distribution(0.0f, 1.0f),
                Generator generator = std::default_random_engine(CURRENT_TIME), float multiplier = 0.5f, float decayFactor = 0.5f) {
                if constexpr (numOctaves == 1) {
                    // This multiplier should equal the last one if this is the final octave.
                    return (multiplier / decayFactor) * generate<width, length, scale>(distribution, generator);
                } else {
                    return multiplier * generate<width, length, scale>(distribution, generator)
                    + generateOctaves<width, length, ceilDivide(scale, 2), numOctaves - 1>(distribution, generator, multiplier * decayFactor, decayFactor);
                }
            }

            // Create the smoothed noise
            template <int width, int length, int scale = 1, typename Distribution = std::uniform_real_distribution<float>,
                typename Generator = std::default_random_engine>
            constexpr StealthTileMap::TileMapF<width, length> generate(Distribution distribution = std::uniform_real_distribution(0.0f, 1.0f),
                Generator generator = std::default_random_engine(CURRENT_TIME)) {
                // Generate a new interpolation kernel if one does not exist.
                if (kernels.count(scale) < 1) {
                    kernels.emplace(scale, std::make_unique<InterpolationKernel<scale>>());
                }
                const InterpolationKernel<scale>& kernel = *static_cast<const InterpolationKernel<scale>*>(kernels[scale].get());
                // Generate a new internal noise map.
                constexpr int internalWidth = ceilDivide(width, scale) + 1;
                constexpr int internalLength = ceilDivide(length, scale) + 1;
                StealthTileMap::TileMapF<internalWidth, internalLength> internalNoiseMap
                    = generateInternalNoiseMap<internalWidth, internalLength>(distribution, generator);
                // Return noise map.
                StealthTileMap::TileMapF<width, length> generatedNoise;
                // Create threads
                for (int i = 0; i < NUM_THREADS; ++i) {
                    noiseThreads[i] = std::thread{&NoiseGenerator::fillRows<internalWidth, internalLength, width, length, scale>,
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
            static std::unordered_map<int, std::unique_ptr<const InterpolationKernelBase>> kernels;

            constexpr float interpolate(float topLeft, float topRight, float bottomLeft,
                float bottomRight, float attenuationX, float attenuationY) noexcept {
                // Interpolate horizontally
                float nx0 = topLeft * (1.0f - attenuationX) + topRight * (attenuationX);
                float nx1 = bottomLeft * (1.0f - attenuationX) + bottomRight * (attenuationX);
                // Interpolate vertically
                float nxy = nx0 * (1.0f - attenuationY) + nx1 * (attenuationY);
                // Return a random value in the range (0, 1) instead of (-sqrt(2) / 2, sqrt(2) / 2)
                return nxy;
            }

            // Initialize with random values according to provided distribution
            template <int internalWidth, int internalLength, typename Distribution, typename Generator>
            constexpr StealthTileMap::TileMapF<internalWidth, internalLength> generateInternalNoiseMap(Distribution& distribution, Generator& generator) {
                // Internal noise map should be large enough to fit tiles of size (scale, scale).
                StealthTileMap::TileMapF<internalWidth, internalLength> internalNoiseMap{};
                for (int i = 0; i < internalWidth; ++i) {
                    for (int j = 0; j < internalLength; ++j) {
                        float val = distribution(generator);
                        internalNoiseMap(i, j) = val;
                    }
                }
                return internalNoiseMap;
            }

            template <int scale, int internalWidth, int internalLength, int width, int length>
            constexpr void fillTile(int internalRow, int internalCol, int scaledRow, int scaledCol,
                const StealthTileMap::TileMapF<internalWidth, internalLength>* internalNoise,
                StealthTileMap::TileMapF<width, length>* generatedNoise, const InterpolationKernel<scale>* kernel) {
                // Only fill the part of the tile that is valid.
                const int maxValidRow = std::min(width - scaledRow, scale);
                const int maxValidCol = std::min(length - scaledCol, scale);
                // Cache local gradient vectors
                float topLeft = internalNoise -> operator()(internalRow, internalCol);
                float topRight = internalNoise -> operator()(internalRow, internalCol + 1);
                float bottomLeft = internalNoise -> operator()(internalRow + 1, internalCol);
                float bottomRight = internalNoise -> operator()(internalRow + 1, internalCol + 1);
                // Cache points and attenuations TileMaps
                const auto& attenuations = kernel -> getAttenuations();
                // Loop over one interpolation kernel tile.
                for (int row = 0; row < maxValidRow; ++row) {
                    for (int col = 0; col < maxValidCol; ++col) {
                        // Interpolate based on the 4 surrounding internal noise points.
                        generatedNoise -> operator()(scaledRow + row, scaledCol + col) = interpolate(topLeft,
                            topRight, bottomLeft, bottomRight, attenuations(col), attenuations(row));
                    }
                }
            }

            template <int internalWidth, int internalLength, int width, int length, int scale>
            constexpr void fillRows(int id, StealthTileMap::TileMapF<internalWidth, internalLength>* internalNoiseMap,
                StealthTileMap::TileMapF<width, length>* generatedNoise, const InterpolationKernel<scale>* kernel) {
                constexpr int portionSize = ceilDivide(internalWidth - 1, NUM_THREADS);
                const int start = id * portionSize;
                const int end = std::min(start + portionSize, internalWidth - 1);
                // Keep track of coordinates on the generated noise map.
                int scaledRow = start * scale;
                for (int i = start; i < end; ++i) {
                    int scaledCol = 0;
                    for (int j = 0; j < internalLength - 1; ++j) {
                        // Fill a tile of size scale x scale.
                        fillTile<scale>(i, j, scaledRow, scaledCol, internalNoiseMap, generatedNoise, kernel);
                        scaledCol += scale;
                    }
                    scaledRow += scale;
                }
            }

    };

    std::unordered_map<int, std::unique_ptr<const InterpolationKernelBase>> NoiseGenerator::kernels{};
} /* StealthWorldGenerator */

#endif
