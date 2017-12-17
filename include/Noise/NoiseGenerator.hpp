#ifndef STEALTH_NOISE_GENERATOR_H
#define STEALTH_NOISE_GENERATOR_H
#define CURRENT_TIME std::chrono::system_clock::now().time_since_epoch().count()
#include "TileMap.hpp"
#include "Noise/Interpolation.hpp"
#include <chrono>
#include <random>
#include <algorithm>

namespace StealthWorldGenerator {
    inline constexpr int ceilDivide(int numerator, int denominator) {
        return 1 + ((numerator - 1) / denominator);
    }

    inline constexpr float linearScale(float value, float distance) {
        return value * std::max(1.0f - distance, 0.0f);
    }

    inline float linearInterpolation(float topLeft, float topRight, float bottomLeft, float bottomRight, const InterpolationDistances& distances) {
        // Linearly interpolate between four points.
        return linearScale(topLeft, distances.at(0, 0)) + linearScale(topRight, distances.at(0, 1))
            + linearScale(bottomLeft, distances.at(1, 0)) + linearScale(bottomRight, distances.at(1, 1));
    }

    // Scale maps one pixel of the generated noise to n pixels of the output.
    template <int scale = 1>
    class NoiseGenerator {
        typedef TileMap<float> NoiseMapType;

        public:
            NoiseGenerator() { }

            // Create the smoothed noise
            template <typename Distribution = std::normal_distribution<float>, typename Generator = std::default_random_engine>
            NoiseMapType generate(int rows, int cols, Distribution distribution = std::normal_distribution<float>(0.5, 0.16667),
                Generator generator = std::default_random_engine(CURRENT_TIME)) {
                NoiseMapType internalNoiseMap = generateInternalNoiseMap(rows, cols, distribution, generator);
                NoiseMapType generatedNoise{rows, cols};

                // display(internalNoiseMap, "Noise Map");
                // display(interpolationKernel, "Interpolation Kernel");

                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        // Figure out where the point lies and its distance to points on the internalNoiseMap
                        generatedNoise.at(i, j) = interpolatePoint(i, j, internalNoiseMap);
                    }
                }
                return generatedNoise;
            }

        private:
            static const InterpolationKernel<scale> interpolationKernel;

            // Initialize with random values according to provided distribution
            template <typename Distribution, typename Generator>
            NoiseMapType generateInternalNoiseMap(int rows, int cols, Distribution& distribution, Generator& generator) {
                // Internal noise map should be large enough to fit tiles of size (scale, scale).
                NoiseMapType internalNoiseMap{ceilDivide(rows, scale) + 1, ceilDivide(cols, scale) + 1};
                for (int i = 0; i < internalNoiseMap.rows(); ++i) {
                    for (int j = 0; j < internalNoiseMap.cols(); ++j) {
                        internalNoiseMap.at(i, j) = distribution(generator);
                    }
                }
                return internalNoiseMap;
            }

            // Interpolate a single point inside a square from the internalNoiseMap
            float interpolatePoint(int row, int col, const NoiseMapType& internalNoiseMap) {
                int scaledRow = row / scale;
                int scaledCol = col / scale;

                // std::cout << "Looking at top-left point (" << scaledRow << ", " << scaledCol
                //     << ") on noise map for point (" << row << ", " << col << ")" << '\n';

                return linearInterpolation(internalNoiseMap.at(scaledRow, scaledCol), internalNoiseMap.at(scaledRow, scaledCol + 1),
                    internalNoiseMap.at(scaledRow + 1, scaledCol), internalNoiseMap.at(scaledRow + 1, scaledCol + 1),
                    interpolationKernel.getDistanceAt(row, col));
            }
    };

    // Create static interpolation kernel for different scales.
    template <int scale>
    const InterpolationKernel<scale> NoiseGenerator<scale>::interpolationKernel{};
} /* StealthWorldGenerator */

#endif
