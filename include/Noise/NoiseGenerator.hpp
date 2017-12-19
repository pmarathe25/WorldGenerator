#ifndef STEALTH_NOISE_GENERATOR_H
#define STEALTH_NOISE_GENERATOR_H
#define CURRENT_TIME std::chrono::system_clock::now().time_since_epoch().count()
#include "TileMap.hpp"
#include "Utility.hpp"
#include "Noise/InterpolationKernel.hpp"
#include <chrono>
#include <random>
#include <unordered_map>

namespace StealthWorldGenerator {
    typedef TileMap<float> NoiseMap;
    typedef TileMap<Vector2f> InternalNoiseMap;

    // Initialize with random values according to provided distribution
    template <typename Distribution, typename Generator>
    InternalNoiseMap generateInternalNoiseMap(int internalRows, int internalCols, Distribution& distribution, Generator& generator) {
        // Internal noise map should be large enough to fit tiles of size (scale, scale).
        InternalNoiseMap internalNoiseMap{internalRows, internalCols};
        for (int i = 0; i < internalNoiseMap.rows(); ++i) {
            for (int j = 0; j < internalNoiseMap.cols(); ++j) {
                float angle = distribution(generator) * 2 * PI;
                internalNoiseMap.at(i, j) = Vector2f{(float) cos(angle), (float) sin(angle)};
            }
        }
        return internalNoiseMap;
    }

    inline float interpolatePerlin(const Vector2f& topLeft, const Vector2f& topRight, const Vector2f& bottomLeft,
        const Vector2f& bottomRight, const Vector2f& point, const Vector2f& attenuation) {
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
        // Scale from (-1, 1) to (0, 1)
        return (nxy + 1.0f) * 0.5f;
    }

    // Scale maps one pixel of the generated noise to n pixels of the output.
    class NoiseGenerator {
        public:
            NoiseGenerator() { }

            // Create the smoothed noise
            template <typename Distribution = std::uniform_real_distribution<float>, typename Generator = std::default_random_engine>
            NoiseMap generate(int rows, int cols, int scale = 1, Distribution distribution = std::uniform_real_distribution(0.0f, 1.0f), Generator generator =
                std::default_random_engine(CURRENT_TIME)) {
                // Generate a new interpolation kernel if one does not exist.
                if (kernels.count(scale) < 1) {
                    kernels.emplace(scale, InterpolationKernel{scale});
                }
                // Generate a new internal noise map.
                InternalNoiseMap internalNoiseMap = generateInternalNoiseMap(ceilDivide(rows, scale) + 1, ceilDivide(cols, scale) + 1, distribution, generator);
                // Return noise map.
                NoiseMap generatedNoise{rows, cols};
                for (int i = 0; i < internalNoiseMap.rows() - 1; ++i) {
                    for (int j = 0; j < internalNoiseMap.cols() - 1; ++j) {
                        // Fill a tile of size scale x scale.
                        fillTile(i, j, scale, internalNoiseMap, generatedNoise);
                    }
                }
                return generatedNoise;
            }

        private:
            void fillTile(int internalRow, int internalCol, int scale, const InternalNoiseMap& internalNoise, NoiseMap& generatedNoise) {
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
                            kernels.at(scale).getPoints().at(row, col), kernels.at(scale).getAttenuations().at(row, col));
                    }
                }
            }

            // Maintain a cache of interpolation kernels of different sizes
            static std::unordered_map<int, const InterpolationKernel> kernels;
    };

    std::unordered_map<int, const InterpolationKernel> NoiseGenerator::kernels{};
} /* StealthWorldGenerator */

#endif
