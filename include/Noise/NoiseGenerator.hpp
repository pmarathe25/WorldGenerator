#ifndef STEALTH_NOISE_GENERATOR_H
#define STEALTH_NOISE_GENERATOR_H
#define CURRENT_TIME std::chrono::system_clock::now().time_since_epoch().count()
#include "TileMap.hpp"
#include "Noise/Interpolation.hpp"
#include <chrono>
#include <random>
#include <algorithm>

const float PI = 3.14159265f;

namespace StealthWorldGenerator {
    class Gradient {
        public:
            Gradient(float value = 0.0f) {
                x = cos(value * 2 * PI);
                y = sin(value * 2 * PI);
            }
            float x, y;
    };

    inline float interpolateGradients(Gradient topLeft, Gradient topRight, Gradient bottomLeft,
        Gradient bottomRight, const Point& point, const PointAttenuation& attenuation) {
        float n00 = topLeft.y * point.row + topLeft.x * point.col;
        float n01 = topRight.y * point.row + topRight.x * (point.col - 1.0f);
        float n10 = bottomLeft.y * (point.row - 1.0f) + bottomLeft.x * point.col;
        float n11 = bottomRight.y * (point.row - 1.0f) + bottomRight.x * (point.col - 1.0f);

        float nx0 = n00 * (1.0f - attenuation.col) + n01 * (attenuation.col);
        float nx1 = n10 * (1.0f - attenuation.col) + n11 * (attenuation.col);

        float nxy = nx0 * (1.0f - attenuation.row) + nx1 * (attenuation.row);
        // Scale from (-1, 1) to (0, 1)
        return (nxy + 1.0f) * 0.5f;
    }

    inline constexpr int ceilDivide(int numerator, int denominator) {
        return 1 + ((numerator - 1) / denominator);
    }

    // Scale maps one pixel of the generated noise to n pixels of the output.
    template <int scale = 1>
    class NoiseGenerator {
        typedef TileMap<float> NoiseMapType;
        typedef TileMap<Gradient> InternalNoiseMapType;

        public:
            NoiseGenerator() { }

            // Create the smoothed noise
            template <typename Distribution = std::uniform_real_distribution<float>, typename Generator = std::default_random_engine>
            NoiseMapType generate(int rows, int cols, Distribution distribution =  std::uniform_real_distribution(0.0f, 1.0f), Generator generator =
                std::default_random_engine(CURRENT_TIME)) {
                InternalNoiseMapType internalNoiseMap = generateInternalNoiseMap(rows, cols, distribution, generator);
                NoiseMapType generatedNoise{rows, cols};
                // DEBUG
                // display(internalNoiseMap, "Noise Map");
                // display(interpolationKernel.getPoints(), "Interpolation Kernel Points");
                // display(interpolationKernel.getDistances(), "Interpolation Kernel Distances");
                for (int i = 0; i < internalNoiseMap.rows() - 1; ++i) {
                    for (int j = 0; j < internalNoiseMap.cols() - 1; ++j) {
                        // Fill a tile of size scale x scale.
                        fillTile(i, j, internalNoiseMap, generatedNoise);
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
                        internalNoiseMap.at(i, j) = Gradient{distribution(generator)};
                    }
                }
                return internalNoiseMap;
            }

            void fillTile(int internalRow, int internalCol, const InternalNoiseMapType& internalNoise, NoiseMapType& generatedNoise) {
                for (int row = 0; row < scale; ++row) {
                    for (int col = 0; col < scale; ++col) {
                        generatedNoise.at(internalRow * scale + row, internalCol * scale + col)
                            = interpolateGradients(internalNoise.at(internalRow, internalCol), internalNoise.at(internalRow, internalCol + 1),
                            internalNoise.at(internalRow + 1, internalCol), internalNoise.at(internalRow + 1, internalCol + 1),
                            interpolationKernel.getPoints().at(row, col), interpolationKernel.getPointAttenuations().at(row, col));
                    }
                }
            }

            // Interpolate a single point inside a square from the internalNoiseMap
            // float interpolatePoint(int row, int col, const InternalNoiseMapType& internalNoiseMap, const Interpolater& interpolater) {
            //     int scaledRow = row / scale;
            //     int scaledCol = col / scale;
            //     return interpolater(internalNoiseMap.at(scaledRow, scaledCol), internalNoiseMap.at(scaledRow, scaledCol + 1),
            //         internalNoiseMap.at(scaledRow + 1, scaledCol), internalNoiseMap.at(scaledRow + 1, scaledCol + 1),
            //         interpolationKernel.getDistances().at(row % scale, col % scale), interpolationKernel.getPoints().at(row % scale, col % scale));
            // }
    };

    // Create static interpolation kernel for different scales.
    template <int scale>
    const InterpolationKernel<scale> NoiseGenerator<scale>::interpolationKernel{};
} /* StealthWorldGenerator */

#endif
