#ifndef STEALTH_INTERPOLATION_H
#define STEALTH_INTERPOLATION_H
#include <cmath>

namespace StealthWorldGenerator {
    class InterpolationWeights {
        friend std::string to_string(const InterpolationWeights& tile);
        public:
            InterpolationWeights(float topLeft = 0.0f, float topRight = 0.0f, float bottomLeft = 0.0f, float bottomRight = 0.0f) :
                topLeft(topLeft), topRight(topRight), bottomLeft(bottomLeft), bottomRight(bottomRight) { }

            InterpolationWeights diagonalMirror() {
                return InterpolationWeights{topLeft, bottomLeft, topRight, bottomRight};
            }

            InterpolationWeights verticalMirror() {
                return InterpolationWeights{topRight, topLeft, bottomRight, bottomLeft};
            }

            InterpolationWeights horizontalMirror() {
                return InterpolationWeights{bottomLeft, bottomRight, topLeft, topRight};
            }
        private:
            // Data
            float topLeft, topRight, bottomLeft, bottomRight;
    };

    std::string to_string(const InterpolationWeights& tile) {
        return "(" + std::to_string(tile.topLeft) + ", " +
            std::to_string(tile.topRight) + ", " +
            std::to_string(tile.bottomLeft) + ", " +
            std::to_string(tile.bottomRight) + ")";
    }

    /*
    Maintains a cache of weights to use for each possible location of a pixel.
    */
    class InterpolationKernel : public TileMap<InterpolationWeights> {
        public:
            InterpolationKernel(int scale = 1) : scale(scale), TileMap<InterpolationWeights>{scale, scale} {
                initializeKernel();
            }

        private:
            const int scale;

            void initializeKernel() {
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

            InterpolationWeights generatePoint(int row, int col) {
                float downsampledX = col / (float) scale;
                float downsampledY = row / (float) scale;
                // Compute a relative location.
                float interpolationOffsetX = downsampledX + 0.5 * 1 / scale;
                float interpolationOffsetY = downsampledY + 0.5 * 1 / scale;
                // Cache common subexpressions
                float xSq = pow(interpolationOffsetX, 2);
                float xInvSq = pow(1.0f - interpolationOffsetX, 2);
                float ySq = pow(interpolationOffsetY, 2);
                float yInvSq = pow(1.0f - interpolationOffsetY, 2);
                // Compute distances to diagonals
                float topLeftDist = sqrt(xSq + ySq);
                float topRightDist = sqrt(xInvSq + ySq);
                float bottomLeftDist = sqrt(xSq + yInvSq);
                float bottomRightDist = sqrt(xInvSq + yInvSq);
                // Construct the weight struct
                return InterpolationWeights(topLeftDist, topRightDist, bottomLeftDist, bottomRightDist);
            }
    };
} /* StealthWorldGenerator */

#endif
