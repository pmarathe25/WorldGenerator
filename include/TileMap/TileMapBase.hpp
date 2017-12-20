#ifndef TILE_MAP_BASE_H
#define TILE_MAP_BASE_H
#include "TileMap/ForwardDeclarations.hpp"
#include "TileMap/Operations/InternalOperations.hpp"

namespace StealthWorldGenerator {
    template <typename Derived>
    class TileMapBase {
        public:
            typedef typename internal::traits<Derived>::ScalarType ScalarType;
            // Dimensions
            static constexpr int rows = internal::traits<Derived>::rows,
                cols = internal::traits<Derived>::cols,
                size = internal::traits<Derived>::size;

            constexpr TileMapBase() = default;

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
} /* StealthWorldGenerator */

#endif
