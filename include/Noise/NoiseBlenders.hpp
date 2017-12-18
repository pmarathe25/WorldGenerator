#ifndef NOISE_BLENDERS_H
#define NOISE_BLENDERS_H

const float PI = 3.1415927;

namespace StealthWorldGenerator {
    class GradientValue {
        public:
            GradientValue(float value = 0.0f) : value(value) {
                x = cos(value * 2 * PI);
                y = sin(value * 2 * PI);
            }
            float value, x, y;
    };

    class PolynomialBlender {
        public:
            PolynomialBlender(float distanceScaling = 1.414f)
                : distanceScaling(distanceScaling) { }

            inline constexpr float attenuatePolynomial(float distance) const {
                // Scale distance
                distance *= distanceScaling;
                // Clamp distance
                distance = std::max(std::min(distance, 1.0f), 0.0f);
                // Perform attenuation.
                float atten = (6 * pow(distance, 5) - 15 * pow(distance, 4) + 10 * pow(distance, 3));
                return 1.0f - atten;
            }

            inline float operator()(GradientValue gval, float distance) const {
                return gval.value * (attenuatePolynomial(distance));
            }
        private:
            float distanceScaling;
    };

    class QuadraticBlender {
        public:
            QuadraticBlender(float distanceScaling = 1.414f) : distanceScaling(distanceScaling) { }

            inline constexpr float attenuateQuadratic(float value, float distance) const {
                // Scale distance
                distance *= distanceScaling;
                float val = pow((value - pow(distance, 2)), 3);
                return std::max(std::min(val, 1.0f), 0.0f);
            }

            inline float operator()(GradientValue gval, float distance) const {
                return attenuateQuadratic(gval.value, distance);
            }
        private:
            float distanceScaling;
    };

    class LinearBlender {
        public:
            LinearBlender(float distanceScaling = 1.414f, float falloffDistance = 0.707f)
                : distanceScaling(distanceScaling), falloffDistance(falloffDistance) { }

            inline float operator()(GradientValue gval, float distance) const {
                // Scale distance
                distance *= distanceScaling;
                return (distance < falloffDistance) ? gval.value * (falloffDistance - distance) : 0.0f;
            }
        private:
            float distanceScaling, falloffDistance;
    };

} /* StealthWorldGenerator */

#endif
