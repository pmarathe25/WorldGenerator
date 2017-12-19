#ifndef STEALTH_NOISE_GENERATOR_H
#define STEALTH_NOISE_GENERATOR_H
#define CURRENT_TIME std::chrono::system_clock::now().time_since_epoch().count()
#include "TileMap.hpp"
#include "Utility.hpp"
#include "Noise/InterpolationKernel.hpp"
#include <chrono>
#include <random>

namespace StealthWorldGenerator {
    typedef TileMap<float> NoiseMap;
    typedef TileMap<RandomGradient> InternalNoiseMap;

    // Initialize with random values according to provided distribution
    template <typename Distribution, typename Generator>
    InternalNoiseMap generateInternalNoiseMap(int internalRows, int internalCols, Distribution& distribution, Generator& generator) {
        // Internal noise map should be large enough to fit tiles of size (scale, scale).
        InternalNoiseMap internalNoiseMap{internalRows, internalCols};
        for (int i = 0; i < internalNoiseMap.rows(); ++i) {
            for (int j = 0; j < internalNoiseMap.cols(); ++j) {
                internalNoiseMap.at(i, j) = RandomGradient{distribution(generator)};
            }
        }
        return internalNoiseMap;
    }

    inline float interpolatePerlin(const RandomGradient& topLeft, const RandomGradient& topRight, const RandomGradient& bottomLeft,
        const RandomGradient& bottomRight, const Point& point, const Point& attenuation) {
        // Compute dot products.
        float n00 = topLeft.y * point.row + topLeft.x * point.col;
        float n01 = topRight.y * point.row + topRight.x * (point.col - 1.0f);
        float n10 = bottomLeft.y * (point.row - 1.0f) + bottomLeft.x * point.col;
        float n11 = bottomRight.y * (point.row - 1.0f) + bottomRight.x * (point.col - 1.0f);
        // Interpolate horizontally
        float nx0 = n00 * (1.0f - attenuation.col) + n01 * (attenuation.col);
        float nx1 = n10 * (1.0f - attenuation.col) + n11 * (attenuation.col);
        // Interpolate vertically
        float nxy = nx0 * (1.0f - attenuation.row) + nx1 * (attenuation.row);
        // Scale from (-1, 1) to (0, 1)
        return (nxy + 1.0f) * 0.5f;
    }

    // Scale maps one pixel of the generated noise to n pixels of the output.
    template <int scale = 1>
    class NoiseGenerator {
        public:
            NoiseGenerator() { }

            // Create the smoothed noise
            template <typename Distribution = std::uniform_real_distribution<float>, typename Generator = std::default_random_engine>
            NoiseMap generate(int rows, int cols, Distribution distribution = std::uniform_real_distribution(0.0f, 1.0f), Generator generator =
                std::default_random_engine(CURRENT_TIME)) {
                // Generate a new internal noise map.
                InternalNoiseMap internalNoiseMap = generateInternalNoiseMap(ceilDivide(rows, scale) + 1, ceilDivide(cols, scale) + 1, distribution, generator);
                // Return noise map.
                NoiseMap generatedNoise{rows, cols};
                for (int i = 0; i < internalNoiseMap.rows() - 1; ++i) {
                    for (int j = 0; j < internalNoiseMap.cols() - 1; ++j) {
                        // Fill a tile of size scale x scale.
                        fillTile(i, j, internalNoiseMap, generatedNoise);
                    }
                }
                return generatedNoise;
            }

        private:
            void fillTile(int internalRow, int internalCol, const InternalNoiseMap& internalNoise, NoiseMap& generatedNoise) {
                // Coordinates on generated noise.
                const int scaledRow = internalRow * scale;
                const int scaledCol = internalCol * scale;
                // Only fill the part of the tile that is valid.
                const int maxValidRow = generatedNoise.rows() - scaledRow;
                const int maxValidCol = generatedNoise.cols() - scaledCol;
                // Loop over one interpolation kernel tile.
                for (int row = 0; row < scale && row < maxValidRow; ++row) {
                    for (int col = 0; col < scale && col < maxValidCol; ++col) {
                        // Interpolate based on the 4 surrounding internal noise points.
                        generatedNoise.at(scaledRow + row, scaledCol + col)
                            = interpolatePerlin(internalNoise.at(internalRow, internalCol), internalNoise.at(internalRow, internalCol + 1),
                            internalNoise.at(internalRow + 1, internalCol), internalNoise.at(internalRow + 1, internalCol + 1),
                            interpolationKernel.getPoints().at(row, col), interpolationKernel.getAttenuations().at(row, col));
                    }
                }
            }

            static const InterpolationKernel<scale> interpolationKernel;
    };

    // Create static interpolation kernel for different scales.
    template <int scale>
    const InterpolationKernel<scale> NoiseGenerator<scale>::interpolationKernel{};
} /* StealthWorldGenerator */

#endif
