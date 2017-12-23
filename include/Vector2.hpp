#ifndef VECTOR_2_H
#define VECTOR_2_H

namespace StealthWorldGenerator {
    template <typename ScalarType>
    class Vector2 {
        public:
            constexpr Vector2() noexcept = default;
            constexpr Vector2(ScalarType x, ScalarType y) noexcept : x(x), y(y) { }
            ScalarType x, y;
    };

    // Display functions
    template <typename ScalarType>
    constexpr std::string to_string(const Vector2<ScalarType>& tile) {
        return "(" + std::to_string(tile.x) + ", " + std::to_string(tile.y) + ")";
    }

    using Vector2i = Vector2<int>;
    using Vector2f = Vector2<float>;
    using Vector2d = Vector2<double>;
} /* StealthWorldGenerator */

#endif
