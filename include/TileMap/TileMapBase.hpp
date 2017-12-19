#ifndef TILE_MAP_BASE_H
#define TILE_MAP_BASE_H
#include "TileMap/ForwardDeclarations.hpp"
#include "TileMap/Ops.hpp"

namespace StealthWorldGenerator {
    template <typename Derived>
    class TileMapBase {
        public:
            typedef typename internal::traits<Derived>::ScalarType ScalarType;

            enum {
                rows = internal::traits<Derived>::rows,
                cols = internal::traits<Derived>::cols,
                size = internal::traits<Derived>::size
            };

            TileMapBase() { }

            inline ScalarType operator[](int index) {
                return static_cast<Derived*>(this) -> operator[](index);
            }

            inline const ScalarType operator[](int index) const {
                return static_cast<const Derived*>(this) -> operator[](index);
            }

            inline ScalarType at(int i, int j) {
                return static_cast<Derived*>(this) -> at(i, j);
            }

            template <typename OtherDerived>
            // friend BinaryOp<Derived, OtherDerived, BinaryOps::add> operator+(const TileMapBase<Derived>& lhs, const TileMapBase<OtherDerived>& rhs);
            BinaryOp<Derived, OtherDerived, BinaryOps::add> operator+(const TileMapBase<OtherDerived>& rhs);
    };
} /* StealthWorldGenerator */

#endif
