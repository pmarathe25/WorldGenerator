#ifndef UNARY_OPERATIONS_H
#define UNARY_OPERATIONS_H
#include "TileMap/UnaryOp.hpp"

namespace StealthWorldGenerator {
    template <typename Derived>
    constexpr UnaryOp<Derived, internal::ops::notOp> operator!(const Derived& lhs) noexcept {
        return UnaryOp<Derived, internal::ops::notOp>{lhs};
    }
} /* StealthWorldGenerator */

#endif /* end of include guard: UNARY_OPERATIONS_H */
