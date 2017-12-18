#ifndef NOISE_INTERPOLATER_H
#define NOISE_INTERPOLATER_H
#include "Noise/NoiseBlenders.hpp"
#include <cmath>

namespace StealthWorldGenerator {
    template <typename Blender>
    class TriangularSumInterpolator {
        public:
            TriangularSumInterpolator(Blender scaleFunc) : scaleFunc(scaleFunc) { }

            inline float operator()(GradientValue topLeft, GradientValue topRight, GradientValue bottomLeft, GradientValue bottomRight,
                const InterpolationDistances& distances, const InterpolationPoint& point) const {
                float val = 0.0f;
                // If we're in the top-right diagonal.
                if (distances.at(0, 1) < distances.at(1, 0)) {
                    val = scaleFunc(topLeft, distances.at(0, 0)) + scaleFunc(topRight, distances.at(0, 1))
                        + scaleFunc(bottomRight, distances.at(1, 1));
                } else {
                    val = scaleFunc(topLeft, distances.at(0, 0)) + scaleFunc(bottomLeft, distances.at(1, 0))
                        + scaleFunc(bottomRight, distances.at(1, 1));
                }
                // Clamp value.
                return std::max(std::min(val, 1.0f), 0.0f);
            }
        private:
            Blender scaleFunc;
    };

    template <typename Blender>
    class RectangularSumInterpolater {
        public:
            RectangularSumInterpolater(Blender scaleFunc) : scaleFunc(scaleFunc) { }

            inline float operator()(GradientValue topLeft, GradientValue topRight, GradientValue bottomLeft, GradientValue bottomRight,
                const InterpolationDistances& distances, const InterpolationPoint& point) const {
                float val = 0.0f;
                // Add interpolations from four corner points.
                val = scaleFunc(topLeft, distances.at(0, 0)) + scaleFunc(topRight, distances.at(0, 1))
                    + scaleFunc(bottomLeft, distances.at(1, 0)) + scaleFunc(bottomRight, distances.at(1, 1));
                // Clamp value.
                return std::max(std::min(val, 1.0f), 0.0f);
            }
        private:
            Blender scaleFunc;
    };

    class TriangularGradientInterpolater {
        public:
            inline float operator()(GradientValue topLeft, GradientValue topRight, GradientValue bottomLeft, GradientValue bottomRight,
                const InterpolationDistances& distances, const InterpolationPoint& point) const {
                float val = 0.0f;
                if (point.col > point.row) {
                    // If we're in the top-right diagonal.
                    val = topRight.value + (topLeft.value - topRight.value) * (1.0f - point.col) + (bottomRight.value - topRight.value) * (point.row);
                } else {
                    val = bottomLeft.value + (bottomRight.value - bottomLeft.value) * (point.col) + (topLeft.value - bottomLeft.value) * (1.0f - point.row);
                }
                // Clamp value.
                return std::max(std::min(val, 1.0f), 0.0f);
            }
    };

    class RectangularGradientInterpolater {
        public:
            inline constexpr float attenuatePolynomial(float distance) const {
                return (6 * pow(distance, 5) - 15 * pow(distance, 4) + 10 * pow(distance, 3));
            }

            inline float operator()(GradientValue topLeft, GradientValue topRight, GradientValue bottomLeft, GradientValue bottomRight,
                const InterpolationDistances& distances, const InterpolationPoint& point) const {
                float n00 = topLeft.y * point.row + topLeft.x * point.col;
                float n01 = topRight.y * point.row + topRight.x * (point.col - 1.0f);
                float n10 = bottomLeft.y * (point.row - 1.0f) + bottomLeft.x * point.col;
                float n11 = bottomRight.y * (point.row - 1.0f) + bottomRight.x * (point.col - 1.0f);

                float nx0 = n00 * (1.0f - attenuatePolynomial(point.col)) + n01 * (attenuatePolynomial(point.col));
                float nx1 = n10 * (1.0f - attenuatePolynomial(point.col)) + n11 * (attenuatePolynomial(point.col));

                float nxy = nx0 * (1.0f - attenuatePolynomial(point.row)) + nx1 * (attenuatePolynomial(point.row));
                // Scale from (-1, 1) to (0, 1)
                return (nxy + 1.0f) * 0.5f;
            }
    };
} /* StealthWorldGenerator */

#endif
