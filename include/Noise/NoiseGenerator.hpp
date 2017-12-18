#ifndef STEALTH_NOISE_GENERATOR_H
#define STEALTH_NOISE_GENERATOR_H
#define CURRENT_TIME std::chrono::system_clock::now().time_since_epoch().count()
#include "TileMap.hpp"
#include "Noise/Interpolation.hpp"
#include "Noise/NoiseInterpolaters.hpp"
#include <chrono>
#include <random>
#include <algorithm>

namespace StealthWorldGenerator {
    inline constexpr int ceilDivide(int numerator, int denominator) {
        return 1 + ((numerator - 1) / denominator);
    }

    // Scale maps one pixel of the generated noise to n pixels of the output.
    template <int scale = 1>
    class NoiseGenerator {
        typedef TileMap<float> NoiseMapType;
        typedef TileMap<GradientValue> InternalNoiseMapType;

        public:
            NoiseGenerator() { }

            // Create the smoothed noise
            template <typename Interpolater = TriangularSumInterpolator<PolynomialBlender>, typename Distribution =  std::uniform_real_distribution<float>,
                typename Generator = std::default_random_engine>
            NoiseMapType generate(int rows, int cols, const Interpolater& interpolater = TriangularSumInterpolator{PolynomialBlender{}},
                Distribution distribution =  std::uniform_real_distribution(0.0f, 1.0f), Generator generator =
                std::default_random_engine(CURRENT_TIME)) {
                InternalNoiseMapType internalNoiseMap = generateInternalNoiseMap(rows, cols, distribution, generator);
                NoiseMapType generatedNoise{rows, cols};

                // DEBUG
                // display(internalNoiseMap, "Noise Map");
                // display(interpolationKernel.getPoints(), "Interpolation Kernel Points");
                // display(interpolationKernel.getDistances(), "Interpolation Kernel Distances");

                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        // Figure out where the point lies and its distance to points on the internalNoiseMap
                        generatedNoise.at(i, j) = interpolatePoint(i, j, internalNoiseMap, interpolater);
                    }
                }
                return generatedNoise;
            }

        private:
            static const InterpolationKernel<scale> interpolationKernel;

            // Initialize with random values according to provided distribution
            template <typename Distribution, typename Generator>
            InternalNoiseMapType generateInternalNoiseMap(int rows, int cols, Distribution& distribution, Generator& generator) {
                // Internal noise map should be large enough to fit tiles of size (scale, scale).
                InternalNoiseMapType internalNoiseMap{ceilDivide(rows, scale) + 1, ceilDivide(cols, scale) + 1};
                for (int i = 0; i < internalNoiseMap.rows(); ++i) {
                    for (int j = 0; j < internalNoiseMap.cols(); ++j) {
                        internalNoiseMap.at(i, j) = GradientValue{distribution(generator)};
                    }
                }
                return internalNoiseMap;
            }

            // Interpolate a single point inside a square from the internalNoiseMap
            template <typename Interpolater>
            float interpolatePoint(int row, int col, const InternalNoiseMapType& internalNoiseMap, const Interpolater& interpolater) {
                int scaledRow = row / scale;
                int scaledCol = col / scale;
                return interpolater(internalNoiseMap.at(scaledRow, scaledCol), internalNoiseMap.at(scaledRow, scaledCol + 1),
                    internalNoiseMap.at(scaledRow + 1, scaledCol), internalNoiseMap.at(scaledRow + 1, scaledCol + 1),
                    interpolationKernel.getDistances().at(row % scale, col % scale), interpolationKernel.getPoints().at(row % scale, col % scale));
            }
    };

    // Create static interpolation kernel for different scales.
    template <int scale>
    const InterpolationKernel<scale> NoiseGenerator<scale>::interpolationKernel{};
} /* StealthWorldGenerator */

#endif
