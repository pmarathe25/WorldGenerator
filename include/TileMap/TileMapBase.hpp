#ifndef TILE_MAP_BASE_H
#define TILE_MAP_BASE_H
#include "TileMap/ForwardDeclarations.hpp"
#include "TileMap/Ops.hpp"
#include <type_traits>

namespace StealthWorldGenerator {
    template <typename Derived>
    class TileMapBase {
        public:
            typedef typename internal::traits<Derived>::ScalarType ScalarType;

            static constexpr int rows = internal::traits<Derived>::rows,
                cols = internal::traits<Derived>::cols,
                size = internal::traits<Derived>::size;

            TileMapBase() { }

            constexpr ScalarType operator[](int index) {
                return static_cast<Derived*>(this) -> operator[](index);
            }

            constexpr ScalarType operator[](int index) const {
                return static_cast<const Derived*>(this) -> operator[](index);
            }

            constexpr ScalarType at(int i, int j) {
                return static_cast<Derived*>(this) -> at(i, j);
            }

            constexpr ScalarType at(int i, int j) const {
                return static_cast<const Derived*>(this) -> at(i, j);
            }
    };

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::add> operator+(const Derived& lhs, const OtherDerived& rhs);

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::subtract> operator-(const Derived& lhs, const OtherDerived& rhs);

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::multiply> operator*(const Derived& lhs, const OtherDerived& rhs);

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::divide> operator/(const Derived& lhs, const OtherDerived& rhs);
} /* StealthWorldGenerator */

#endif
