#ifndef VECTOR_2_H
#define VECTOR_2_H

namespace StealthWorldGenerator {
    template <typename ScalarType>
    class Vector2 {
        public:
            Vector2() { }

            Vector2(ScalarType x, ScalarType y) : x(x), y(y) { }
            ScalarType x, y;
    };

    // Display functions
    template <typename ScalarType>
    constexpr std::string to_string(const Vector2<ScalarType>& tile) {
        return "(" + std::to_string(tile.x) + ", " + std::to_string(tile.y) + ")";
    }

    using Vector2f = Vector2<float>;
} /* StealthWorldGenerator */

#endif
