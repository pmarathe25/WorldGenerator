#ifndef VECTOR_2_H
#define VECTOR_2_H
#include "Utility.hpp"

namespace StealthWorldGenerator {
    template <typename ScalarType>
    class Vector2 {
        public:
            Vector2(ScalarType x = 0.0f, ScalarType y = 0.0f) : x(x), y(y) { }
            ScalarType x, y;
    };

    // Display functions
    template <typename ScalarType>
    std::string to_string(const Vector2<ScalarType>& tile) {
        return "(" + std::to_string(tile.x) + ", " + std::to_string(tile.y) + ")";
    }

    using Vector2f = Vector2<float>;
} /* StealthWorldGenerator */

#endif
