#ifndef STEALTH_INTERPOLATION_H
#define STEALTH_INTERPOLATION_H
#include "TileMap/TileMap.hpp"
#include "Utility.hpp"
#include "Vector2.hpp"

namespace StealthWorldGenerator {
    // Maintains a cache of points and attenuations to use for each possible location of a pixel.
    template <int scale = 1>
    class InterpolationKernel {
        public:
            InterpolationKernel() {
                initializeKernel();
            }

            const TileMap<Vector2f, scale, scale>& getPoints() const {
                return points;
            }

            const TileMap<Vector2f, scale, scale>& getAttenuations() const {
                return attenuations;
            }

        private:
            TileMap<Vector2f, scale, scale> points{};
            TileMap<Vector2f, scale, scale> attenuations{};

            inline void initializeKernel() {
                // Optimally initialize kernel. Only need to compute 1/8th of the kernel.
                constexpr int quadrantBound = ceilDivide(scale, 2);
                initializeDiagonalQuadrant(quadrantBound);
                reflectDiagonal(quadrantBound);
                reflectVertical(quadrantBound);
                reflectHorizontal(quadrantBound);
            }

            inline constexpr Vector2f calculatePoint(int row, int col) const {
                // Compute a relative location.
                float interpolationOffsetX = (col / (float) scale) + 0.5f * 1.0f / scale;
                float interpolationOffsetY = (row / (float) scale) + 0.5f * 1.0f / scale;
                return Vector2f(interpolationOffsetX, interpolationOffsetY);
            }

            inline constexpr Vector2f diagonallyMirror(const Vector2f& other) const {
                return Vector2f(other.y, other.x);
            }

            inline constexpr Vector2f verticallyMirror(const Vector2f& other) const {
                return Vector2f(1.0f - other.x, other.y);
            }

            inline constexpr Vector2f horizontallyMirror(const Vector2f& other) const {
                return Vector2f(other.x, 1.0f - other.y);
            }

            inline constexpr void initializeDiagonalQuadrant(int quadrantBound) {
                // Compute top-right diagonal of top-left quadrant
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = row; col < quadrantBound; ++col) {
                        points.at(row, col) = calculatePoint(row, col);
                        attenuations.at(row, col) = Vector2f(attenuationPolynomial(points.at(row, col).x),
                            attenuationPolynomial(points.at(row, col).y));
                    }
                }
            }

            inline constexpr void reflectDiagonal(int quadrantBound) {
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = 0; col < row; ++col) {
                        points.at(row, col) = diagonallyMirror(points.at(col, row));
                        attenuations.at(row, col) = Vector2f(attenuationPolynomial(points.at(row, col).x),
                            attenuationPolynomial(points.at(row, col).y));
                    }
                }
            }

            inline constexpr void reflectVertical(int quadrantBound) {
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = quadrantBound; col < scale; ++col) {
                        points.at(row, col) = verticallyMirror(points.at(row, (scale - 1) - col));
                        attenuations.at(row, col) = Vector2f(attenuationPolynomial(points.at(row, col).x),
                            attenuationPolynomial(points.at(row, col).y));
                    }
                }
            }

            inline constexpr void reflectHorizontal(int quadrantBound) {
                for (int row = quadrantBound; row < scale; ++row) {
                    for (int col = 0; col < scale; ++col) {
                        points.at(row, col) = horizontallyMirror(points.at((scale - 1) - row, col));
                        attenuations.at(row, col) = Vector2f(attenuationPolynomial(points.at(row, col).x),
                            attenuationPolynomial(points.at(row, col).y));
                    }
                }
            }
    };

} /* StealthWorldGenerator */

#endif
