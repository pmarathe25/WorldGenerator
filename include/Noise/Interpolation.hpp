#ifndef STEALTH_INTERPOLATION_H
#define STEALTH_INTERPOLATION_H
#include "TileMap.hpp"
#include <cmath>

namespace StealthWorldGenerator {
    class InterpolationPoint {
        public:
            InterpolationPoint(float row = 0.0f, float col = 0.0f) : row(row), col(col) { }

            InterpolationPoint diagonalMirror() {
                return InterpolationPoint(col, row);
            }

            InterpolationPoint verticalMirror() {
                return InterpolationPoint(row, 1.0f - col);
            }

            InterpolationPoint horizontalMirror() {
                return InterpolationPoint(1.0f - row, col);
            }

            float row, col;
    };

    class InterpolationDistances : public TileMap<float> {
        friend std::string to_string(const InterpolationDistances& tile);
        public:
            InterpolationDistances(std::initializer_list<float> init = {0.0f, 0.0f, 0.0f, 0.0f}) : TileMap<float>{2, 2} {
                tiles = init;
            }

            void operator=(std::initializer_list<float> init) {
                tiles = init;
            }

            InterpolationDistances diagonalMirror() {
                return InterpolationDistances{{at(0, 0), at(1, 0), at(0, 1), at(1, 1)}};
            }

            InterpolationDistances verticalMirror() {
                return InterpolationDistances{{at(0, 1), at(0, 0), at(1, 1), at(1, 0)}};
            }

            InterpolationDistances horizontalMirror() {
                return InterpolationDistances{{at(1, 0), at(1, 1), at(0, 0), at(0, 1)}};
            }
    };

    // Maintains a cache of distances to use for each possible location of a pixel.
    // Stored in Column-major order
    template <int scale = 1>
    class InterpolationKernel {
        public:
            InterpolationKernel() {
                initializeKernel();
            }

            const TileMap<InterpolationDistances>& getDistances() const {
                return distances;
            }

            const TileMap<InterpolationPoint>& getPoints() const {
                return points;
            }

        private:
            TileMap<InterpolationDistances> distances{scale, scale};
            TileMap<InterpolationPoint> points{scale, scale};

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
                        distances.at(row, col) = distances.at(col, row).diagonalMirror();
                        points.at(row, col) = points.at(col, row).diagonalMirror();
                    }
                }
            }

            void reflectVertical(int quadrantBound) {
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = quadrantBound; col < scale; ++col) {
                        distances.at(row, col) = distances.at(row, (scale - 1) - col).verticalMirror();
                        points.at(row, col) = points.at(row, (scale - 1) - col).verticalMirror();
                    }
                }
            }

            void reflectHorizontal(int quadrantBound) {
                for (int row = quadrantBound; row < scale; ++row) {
                    for (int col = 0; col < scale; ++col) {
                        distances.at(row, col) = distances.at((scale - 1) - row, col).horizontalMirror();
                        points.at(row, col) = points.at((scale - 1) - row, col).horizontalMirror();
                    }
                }
            }

            void calculatePoint(int row, int col) {
                // Compute a relative location.
                float interpolationOffsetX = (col / (float) scale) + 0.5 * 1 / scale;
                float interpolationOffsetY = (row / (float) scale) + 0.5 * 1 / scale;
                // Cache common subexpressions
                float ySq = pow(interpolationOffsetX, 2);
                float yInvSq = pow(1.0f - interpolationOffsetX, 2);
                float xSq = pow(interpolationOffsetY, 2);
                float xInvSq = pow(1.0f - interpolationOffsetY, 2);
                // Compute distances to diagonals
                float topLeftDist = sqrt(ySq + xSq);
                float topRightDist = sqrt(yInvSq + xSq);
                float bottomLeftDist = sqrt(ySq + xInvSq);
                float bottomRightDist = sqrt(yInvSq + xInvSq);
                // Construct the distance and point TileMaps
                distances.at(row, col) = {topLeftDist, topRightDist, bottomLeftDist, bottomRightDist};
                points.at(row, col) = InterpolationPoint(interpolationOffsetY, interpolationOffsetX);
            }
    };

    // Display functions
    std::string to_string(const InterpolationPoint& tile) {
        return "(" + std::to_string(tile.row) + ", " + std::to_string(tile.col) + ")";
    }

    std::string to_string(const InterpolationDistances& tile) {
        return "(" + std::to_string(tile.at(0, 0)) + ", " +
            std::to_string(tile.at(0, 1)) + ", " +
            std::to_string(tile.at(1, 0)) + ", " +
            std::to_string(tile.at(1, 1)) + ")";
    }

} /* StealthWorldGenerator */

#endif
