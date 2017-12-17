#ifndef STEALTH_INTERPOLATION_H
#define STEALTH_INTERPOLATION_H
#include "TileMap.hpp"
#include <cmath>

namespace StealthWorldGenerator {
    class InterpolationDistances : public TileMap<float> {
        friend std::string to_string(const InterpolationDistances& tile);
        public:
            InterpolationDistances(std::initializer_list<float> init = {0.0f, 0.0f, 0.0f, 0.0f}) : TileMap<float>{2, 2} {
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

    std::string to_string(const InterpolationDistances& tile) {
        return "(" + std::to_string(tile.at(0, 0)) + ", " +
            std::to_string(tile.at(0, 1)) + ", " +
            std::to_string(tile.at(1, 0)) + ", " +
            std::to_string(tile.at(1, 1)) + ")";
    }

    // Maintains a cache of distances to use for each possible location of a pixel.
    // Stored in Column-major order
    template <int scale = 1>
    class InterpolationKernel : public TileMap<InterpolationDistances> {
        public:
            InterpolationKernel() : TileMap<InterpolationDistances>{scale, scale} {
                initializeKernel();
            }

            const InterpolationDistances& getDistanceAt(int row, int col) const {
                return this -> at(row % scale, col % scale);
            }
        private:
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
                        this -> at(row, col) = generatePoint(row, col);
                    }
                }
            }

            void reflectDiagonal(int quadrantBound) {
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = 0; col < row; ++col) {
                        this -> at(row, col) = this -> at(col, row).diagonalMirror();
                    }
                }
            }

            void reflectVertical(int quadrantBound) {
                for (int row = 0; row < quadrantBound; ++row) {
                    for (int col = quadrantBound; col < scale; ++col) {
                        this -> at(row, col) = this -> at(row, (scale - 1) - col).verticalMirror();
                    }
                }
            }

            void reflectHorizontal(int quadrantBound) {
                for (int row = quadrantBound; row < scale; ++row) {
                    for (int col = 0; col < scale; ++col) {
                        this -> at(row, col) = this -> at((scale - 1) - row, col).horizontalMirror();
                    }
                }
            }

            InterpolationDistances generatePoint(int row, int col) {
                // Compute a relative location.
                float interpolationOffsetX = (col / (float) scale) + 0.5 * 1 / scale;
                float interpolationOffsetY = (row / (float) scale) + 0.5 * 1 / scale;
                // Cache common suberowpressions
                float rowSq = pow(interpolationOffsetX, 2);
                float rowInvSq = pow(1.0f - interpolationOffsetX, 2);
                float colSq = pow(interpolationOffsetY, 2);
                float colInvSq = pow(1.0f - interpolationOffsetY, 2);
                // Compute distances to diagonals
                float topLeftDist = sqrt(rowSq + colSq);
                float topRightDist = sqrt(rowInvSq + colSq);
                float bottomLeftDist = sqrt(rowSq + colInvSq);
                float bottomRightDist = sqrt(rowInvSq + colInvSq);
                // Construct the distance struct
                return InterpolationDistances({topLeftDist, topRightDist, bottomLeftDist, bottomRightDist});
            }
    };
} /* StealthWorldGenerator */

#endif
