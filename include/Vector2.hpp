#ifndef VECTOR_2_H
#define VECTOR_2_H
#include "Utility.hpp"

namespace StealthWorldGenerator {
    template <typename ScalarType>
    class Vector2 {
        public:
            Vector2(ScalarType x = ScalarType(), ScalarType y = ScalarType()) : x(x), y(y) { }
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
