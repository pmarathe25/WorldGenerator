#ifndef STEALTH_NOISE_GENERATOR_H
#define STEALTH_NOISE_GENERATOR_H
#define CURRENT_TIME std::chrono::system_clock::now().time_since_epoch().count()
#include "TileMap.hpp"
#include "Noise/Interpolation.hpp"
#include <chrono>
#include <random>

namespace StealthWorldGenerator {
    inline constexpr int ceilDivide(int numerator, int denominator) {
        return 1 + ((numerator - 1) / denominator);
    }

    /*
    Scale maps one pixel of the generated noise to n pixels of the output.
    */
    template <int scale = 1>
    class NoiseGenerator : public TileMap<float> {
        typedef TileMap<float> NoiseMapType;

        public:
            NoiseGenerator(int rows, int cols) : NoiseMapType(rows, cols) {
                randomize();
            }

            // Create the smoothed noise
            template <typename Distribution = std::normal_distribution<float>, typename Generator = std::default_random_engine>
            void randomize(Distribution distribution = std::normal_distribution<float>(0.5, 0.16667),
                Generator generator = std::default_random_engine(CURRENT_TIME)) {
                NoiseMapType internalNoiseMap = generateInternalNoiseMap(distribution, generator);
                for (int i = 0; i < rows(); ++i) {
                    for (int j = 0; j < cols(); ++j) {
                        // Figure out where the point lies and its distance to points on the internalNoiseMap
                        this -> at(i, j) = interpolatePoint(i, j);
                    }
                }
            }

        private:
            static const InterpolationKernel<scale> interpolationKernel;
            // Initialize with random values according to provided distribution
            template <typename Distribution, typename Generator>
            NoiseMapType generateInternalNoiseMap(Distribution& distribution, Generator& generator) {
                // Internal noise map should be large enough to fit tiles of size (scale, scale).
                NoiseMapType internalNoiseMap{ceilDivide(rows(), scale) + 1, ceilDivide(cols(), scale) + 1};
                for (int i = 0; i < internalNoiseMap.rows(); ++i) {
                    for (int j = 0; j < internalNoiseMap.cols(); ++j) {
                        internalNoiseMap.at(i, j) = distribution(generator);
                    }
                }
                return internalNoiseMap;
            }
            // // Interpolate a single point inside a triangle from the internalNoiseMap
            float interpolatePoint(int x, int y) {
                // return "";
                return 0.0f;
            }
    };
} /* StealthWorldGenerator */

#endif
