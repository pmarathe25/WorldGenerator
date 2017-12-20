#ifndef UTILITY_H
#define UTILITY_H
#include <cmath>

const float TAU = 6.28318530718f;

namespace StealthWorldGenerator {
    constexpr int ceilDivide(int numerator, int denominator) {
        return 1 + ((numerator - 1) / denominator);
    }

    constexpr float attenuationPolynomial(float distance) {
        return (6 * pow(distance, 5) - 15 * pow(distance, 4) + 10 * pow(distance, 3));
    }
} /* StealthWorldGenerator */

#endif
