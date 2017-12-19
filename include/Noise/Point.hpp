#ifndef POINT_H
#define POINT_H
#include "Utility.hpp"

namespace StealthWorldGenerator {
    class Point {
        public:
            Point(float row = 0.0f, float col = 0.0f) : row(row), col(col) { }
            float row, col;
    };

    class RandomGradient {
        public:
            RandomGradient(float seed = 0.0f) {
                x = cos(seed * 2 * PI);
                y = sin(seed * 2 * PI);
            }
            float x, y;
    };
} /* StealthWorldGenerator */

#endif
