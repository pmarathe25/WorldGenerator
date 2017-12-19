#ifndef POINT_H
#define POINT_H
#include "Utility.hpp"

namespace StealthWorldGenerator {
    class Point {
        public:
            Point(float row = 0.0f, float col = 0.0f) : row(row), col(col) { }
            float row, col;
    };

    inline Point diagonalMirror(const Point& other) {
        return Point(other.col, other.row);
    }

    inline Point verticalMirror(const Point& other) {
        return Point(other.row, 1.0f - other.col);
    }

    inline Point horizontalMirror(const Point& other) {
        return Point(1.0f - other.row, other.col);
    }

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
