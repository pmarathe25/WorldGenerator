#ifndef STEALTH_NOISE_H
#define STEALTH_NOISE_H
#define CURRENT_TIME std::chrono::system_clock::now().time_since_epoch().count()
#include "TileMap.hpp"
#include <chrono>
#include <random>

namespace StealthWorldGenerator {
    template <int numerator, int denominator>
    constexpr inline int ceilDivide() {
        return 1 + ((numerator - 1) / denominator);
    }

    /*
    Maintains a cache of weights to use for each possible location of a pixel.
    */
    template <int scale = 1>
    class InterpolationTable {
        public:
        private:
            TileMap<float, scale, scale> weightCache;
    };

    /*
    Scale maps one pixel of the generated noise to n pixels of the output.
    */
    template <int rowsAtCompileTime, int colsAtCompileTime, int scale = 1>
    class StealthNoise : public TileMap<std::string, rowsAtCompileTime, colsAtCompileTime> {
        public:
            template <typename Generator = std::default_random_engine, typename Distribution = std::normal_distribution<float>>
            StealthNoise(Distribution distribution = std::normal_distribution<float>(0.5, 0.16667),
                Generator generator = std::default_random_engine(CURRENT_TIME)) {
                initializeNoiseMap(distribution, generator);
                interpolateNoise();
            }
        private:
            // Maintain internal noise map
            typedef TileMap<float, ceilDivide<rowsAtCompileTime, scale>(), ceilDivide<colsAtCompileTime, scale>()> NoiseMapType;
            NoiseMapType noiseMap;
            // Initialize with random values according to provided distribution
            template <typename Generator, typename Distribution>
            void initializeNoiseMap(Distribution& distribution, Generator& generator) {
                for (int i = 0; i < NoiseMapType::rows; ++i) {
                    for (int j = 0; j < NoiseMapType::cols; ++j) {
                        noiseMap.at(i, j) = distribution(generator);
                    }
                }
            }
            // Create the smoothed noise
            void interpolateNoise() {
                for (int i = 0; i < StealthNoise::rows; ++i) {
                    for (int j = 0; j < StealthNoise::cols; ++j) {
                        // Figure out where the point lies and its distance to points on the noiseMap
                        this -> at(i, j) = interpolatePoint(i, j);
                    }
                }
            }
            // Interpolate a single point inside a triangle from the noiseMap
            std::string interpolatePoint(int i, int j) {
                float downsampledX = j / (float) scale;
                float downsampledY = i / (float) scale;
                // Compute a relative location.
                float interpolationOffsetX = downsampledX - floor(downsampledX) + 0.5 * 1 / scale;
                float interpolationOffsetY = downsampledY - floor(downsampledY) + 0.5 * 1 / scale;
                // Compute distances to diagonals
                float bottomLeftDist = sqrt(pow(interpolationOffsetX, 2) + pow(1 - interpolationOffsetY, 2));
                float topRightDist = sqrt(pow(1 - interpolationOffsetX, 2) + pow(interpolationOffsetY, 2));
                if (interpolationOffsetX >= 1 - interpolationOffsetY) {
                    // Point lies in the bottom right - use top-right, bottom-left and bottom-right points.
                    float bottomRightDist = sqrt(pow(1 - interpolationOffsetX , 2) + pow(1 - interpolationOffsetY , 2));
                    // return std::to_string(bottomRightDist + bottomLeftDist);
                } else {
                    // Point lies in the top left - use top-right, bottom-left and top-left points.

                }
                // return "0.000000";
                return "(" + std::to_string(interpolationOffsetX) + ", " + std::to_string(interpolationOffsetY) + ")";
            }

    };
} /* StealthWorldGenerator */

#endif
