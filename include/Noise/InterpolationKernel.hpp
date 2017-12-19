#ifndef STEALTH_INTERPOLATION_H
#define STEALTH_INTERPOLATION_H
#include "TileMap.hpp"
#include "Utility.hpp"
#include "Noise/Point.hpp"

namespace StealthWorldGenerator {
    // Maintains a cache of points and attenuations to use for each possible location of a pixel.
    template <int scale = 1>
    class InterpolationKernel {
        public:
            InterpolationKernel() {
                initializeKernel();
            }

            const TileMap<Point>& getPoints() const {
                return points;
            }

            const TileMap<Point>& getAttenuations() const {
                return attenuations;
            }

        private:
            TileMap<Point> points{scale, scale};
            TileMap<Point> attenuations{scale, scale};

            inline void initializeKernel() {
                // Optimally initialize kernel. Only need to compute 1/8th of the kernel.
                int quadrantBound = ceilDivide(scale, 2);
                initializeDiagonalQuadrant(quadrantBound);
                reflectDiagonal(quadrantBound);
                reflectVertical(quadrantBound);
                reflectHorizontal(quadrantBound);
            }

            inline Point calculatePoint(int row, int col) {
                // Compute a relative location.
                float interpolationOffsetX = (col / (float) scale) + 0.5 * 1.0f / scale;
                float interpolationOffsetY = (row / (float) scale) + 0.5 * 1.0f / scale;
                // Construct the point and pointAttenuation TileMaps
                return Point(interpolationOffsetY, interpolationOffsetX);
            }

            inline void initializeDiagonalQuadrant(int quadrantBound) {
                // Compute top-right diagonal of top-left quadrant
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = row; col < quadrantBound; ++col) {
                        points.at(row, col) = calculatePoint(row, col);
                        attenuations.at(row, col) = Point(attenuationPolynomial(points.at(row, col).row),
                            attenuationPolynomial(points.at(row, col).col));
                    }
                }
            }

            inline void reflectDiagonal(int quadrantBound) {
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = 0; col < row; ++col) {
                        points.at(row, col) = diagonalMirror(points.at(col, row));
                        attenuations.at(row, col) = Point(attenuationPolynomial(points.at(row, col).row),
                            attenuationPolynomial(points.at(row, col).col));
                    }
                }
            }

            inline void reflectVertical(int quadrantBound) {
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = quadrantBound; col < scale; ++col) {
                        points.at(row, col) = verticalMirror(points.at(row, (scale - 1) - col));
                        attenuations.at(row, col) = Point(attenuationPolynomial(points.at(row, col).row),
                            attenuationPolynomial(points.at(row, col).col));
                    }
                }
            }

            inline void reflectHorizontal(int quadrantBound) {
                for (int row = quadrantBound; row < scale; ++row) {
                    for (int col = 0; col < scale; ++col) {
                        points.at(row, col) = horizontalMirror(points.at((scale - 1) - row, col));
                        attenuations.at(row, col) = Point(attenuationPolynomial(points.at(row, col).row),
                            attenuationPolynomial(points.at(row, col).col));
                    }
                }
            }
    };

    // Display functions
    std::string to_string(const Point& tile) {
        return "(" + std::to_string(tile.row) + ", " + std::to_string(tile.col) + ")";
    }

} /* StealthWorldGenerator */

#endif
