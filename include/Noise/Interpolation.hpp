#ifndef STEALTH_INTERPOLATION_H
#define STEALTH_INTERPOLATION_H
#include "TileMap.hpp"
#include <cmath>

namespace StealthWorldGenerator {
    inline constexpr float attenuationPolynomial(float distance) {
        return (6 * pow(distance, 5) - 15 * pow(distance, 4) + 10 * pow(distance, 3));
    }

    class Point {
        public:
            Point(float row = 0.0f, float col = 0.0f) : row(row), col(col) { }
            float row, col;
    };


    Point diagonalMirror(const Point& other) {
        return Point(other.col, other.row);
    }

    Point verticalMirror(const Point& other) {
        return Point(other.row, 1.0f - other.col);
    }

    Point horizontalMirror(const Point& other) {
        return Point(1.0f - other.row, other.col);
    }


    class PointAttenuation {
        public:
            PointAttenuation() {
                row = 0.0f;
                col = 0.0f;
            }

            PointAttenuation(const Point& other) {
                row = attenuationPolynomial(other.row);
                col = attenuationPolynomial(other.col);
            }

            PointAttenuation(float row, float col) {
                this -> row = attenuationPolynomial(row);
                this -> col = attenuationPolynomial(col);
            }

            float row, col;
    };

    // Maintains a cache of distances to use for each possible location of a pixel.
    // Stored in Column-major order
    template <int scale = 1>
    class InterpolationKernel {
        public:
            InterpolationKernel() {
                initializeKernel();
            }

            const TileMap<Point>& getPoints() const {
                return points;
            }

            const TileMap<PointAttenuation>& getPointAttenuations() const {
                return attenuations;
            }

        private:
            TileMap<Point> points{scale, scale};
            TileMap<PointAttenuation> attenuations{scale, scale};

            void initializeKernel() {
                // Optimally initialize kernel. Only need to compute 1/8th of the kernel.
                int quadrantBound = ceil(scale / 2.0f);
                initializeDiagonalQuadrant(quadrantBound);
                reflectDiagonal(quadrantBound);
                reflectVertical(quadrantBound);
                reflectHorizontal(quadrantBound);
            }

            void initializeDiagonalQuadrant(int quadrantBound) {
                // Compute top-right diagonal of top-left quadrant
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = row; col < quadrantBound; ++col) {
                        calculatePoint(row, col);
                    }
                }
            }

            void reflectDiagonal(int quadrantBound) {
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = 0; col < row; ++col) {
                        points.at(row, col) = diagonalMirror(points.at(col, row));
                        attenuations.at(row, col) = PointAttenuation(points.at(row, col));
                    }
                }
            }

            void reflectVertical(int quadrantBound) {
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = quadrantBound; col < scale; ++col) {
                        points.at(row, col) = verticalMirror(points.at(row, (scale - 1) - col));
                        attenuations.at(row, col) = PointAttenuation(points.at(row, col));
                    }
                }
            }

            void reflectHorizontal(int quadrantBound) {
                for (int row = quadrantBound; row < scale; ++row) {
                    for (int col = 0; col < scale; ++col) {
                        points.at(row, col) = horizontalMirror(points.at((scale - 1) - row, col));
                        attenuations.at(row, col) = PointAttenuation(points.at(row, col));
                    }
                }
            }

            void calculatePoint(int row, int col) {
                // Compute a relative location.
                float interpolationOffsetX = (col / (float) scale) + 0.5 * 1.0f / scale;
                float interpolationOffsetY = (row / (float) scale) + 0.5 * 1.0f / scale;
                // Construct the point and pointAttenuation TileMaps
                points.at(row, col) = Point(interpolationOffsetY, interpolationOffsetX);
                attenuations.at(row, col) = PointAttenuation(points.at(row, col));
            }
    };

    // Display functions
    std::string to_string(const Point& tile) {
        return "(" + std::to_string(tile.row) + ", " + std::to_string(tile.col) + ")";
    }

    std::string to_string(const PointAttenuation& tile) {
        return "(" + std::to_string(tile.row) + ", " + std::to_string(tile.col) + ")";
    }

} /* StealthWorldGenerator */

#endif
