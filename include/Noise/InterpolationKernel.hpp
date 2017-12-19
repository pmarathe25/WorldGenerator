#ifndef STEALTH_INTERPOLATION_H
#define STEALTH_INTERPOLATION_H
#include "TileMap.hpp"
#include "Utility.hpp"
#include "Vector2.hpp"

namespace StealthWorldGenerator {
    // Maintains a cache of points and attenuations to use for each possible location of a pixel.
    class InterpolationKernel {
        public:
            InterpolationKernel(int scale) : scale(scale) {
                initializeKernel();
            }

            const TileMap<Vector2f>& getPoints() const {
                return points;
            }

            const TileMap<Vector2f>& getAttenuations() const {
                return attenuations;
            }

        private:
            const int scale;
            TileMap<Vector2f> points{scale, scale};
            TileMap<Vector2f> attenuations{scale, scale};

            inline void initializeKernel() {
                // Optimally initialize kernel. Only need to compute 1/8th of the kernel.
                int quadrantBound = ceilDivide(scale, 2);
                initializeDiagonalQuadrant(quadrantBound);
                reflectDiagonal(quadrantBound);
                reflectVertical(quadrantBound);
                reflectHorizontal(quadrantBound);
            }

            inline Vector2f calculatePoint(int row, int col) {
                // Compute a relative location.
                float interpolationOffsetX = (col / (float) scale) + 0.5f * 1.0f / scale;
                float interpolationOffsetY = (row / (float) scale) + 0.5f * 1.0f / scale;
                return Vector2f(interpolationOffsetX, interpolationOffsetY);
            }

            inline Vector2f diagonallyMirror(const Vector2f& other) {
                return Vector2f(other.y, other.x);
            }

            inline Vector2f verticallyMirror(const Vector2f& other) {
                return Vector2f(1.0f - other.x, other.y);
            }

            inline Vector2f horizontallyMirror(const Vector2f& other) {
                return Vector2f(other.x, 1.0f - other.y);
            }

            inline void initializeDiagonalQuadrant(int quadrantBound) {
                // Compute top-right diagonal of top-left quadrant
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = row; col < quadrantBound; ++col) {
                        points.at(row, col) = calculatePoint(row, col);
                        attenuations.at(row, col) = Vector2f(attenuationPolynomial(points.at(row, col).x),
                            attenuationPolynomial(points.at(row, col).y));
                    }
                }
            }

            inline void reflectDiagonal(int quadrantBound) {
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = 0; col < row; ++col) {
                        points.at(row, col) = diagonallyMirror(points.at(col, row));
                        attenuations.at(row, col) = Vector2f(attenuationPolynomial(points.at(row, col).x),
                            attenuationPolynomial(points.at(row, col).y));
                    }
                }
            }

            inline void reflectVertical(int quadrantBound) {
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = quadrantBound; col < scale; ++col) {
                        points.at(row, col) = verticallyMirror(points.at(row, (scale - 1) - col));
                        attenuations.at(row, col) = Vector2f(attenuationPolynomial(points.at(row, col).x),
                            attenuationPolynomial(points.at(row, col).y));
                    }
                }
            }

            inline void reflectHorizontal(int quadrantBound) {
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
