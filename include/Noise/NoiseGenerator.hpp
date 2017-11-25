#ifndef STEALTH_NOISE_GENERATOR_H
#define STEALTH_NOISE_GENERATOR_H
#define CURRENT_TIME std::chrono::system_clock::now().time_since_epoch().count()
#include "TileMap.hpp"
#include "Noise/Interpolation.hpp"
#include <chrono>
#include <random>

namespace StealthWorldGenerator {
    template <int numerator, int denominator>
    constexpr inline int ceilDivide() {
        return 1 + ((numerator - 1) / denominator);
    }

    /*
    Scale maps one pixel of the generated noise to n pixels of the output.
    */
    template <int scale = 1>
    class NoiseGenerator : public TileMap<std::string> {
        public:
            template <typename Generator = std::default_random_engine, typename Distribution = std::normal_distribution<float>>
            NoiseGenerator(Distribution distribution = std::normal_distribution<float>(0.5, 0.16667),
                Generator generator = std::default_random_engine(CURRENT_TIME)) {
                // initializeNoiseMap(distribution, generator);
                // interpolateNoise();
            }
        private:
            // // Maintain internal noise map
            // typedef TileMap<float, ceilDivide<rowsAtCompileTime, scale>(), ceilDivide<colsAtCompileTime, scale>()> NoiseMapType;
            // NoiseMapType noiseMap;
            // // Initialize with random values according to provided distribution
            // template <typename Generator, typename Distribution>
            // void initializeNoiseMap(Distribution& distribution, Generator& generator) {
            //     for (int i = 0; i < NoiseMapType::rows; ++i) {
            //         for (int j = 0; j < NoiseMapType::cols; ++j) {
            //             noiseMap.at(i, j) = distribution(generator);
            //         }
            //     }
            // }
            // // Create the smoothed noise
            // void interpolateNoise() {
            //     for (int i = 0; i < NoiseGenerator::rows; ++i) {
            //         for (int j = 0; j < NoiseGenerator::cols; ++j) {
            //             // Figure out where the point lies and its distance to points on the noiseMap
            //             this -> at(i, j) = interpolatePoint(i, j);
            //         }
            //     }
            // }
            // // Interpolate a single point inside a triangle from the noiseMap
            // std::string interpolatePoint(int i, int j) {
            //     return "";
            // }

    };
} /* StealthWorldGenerator */

#endif
