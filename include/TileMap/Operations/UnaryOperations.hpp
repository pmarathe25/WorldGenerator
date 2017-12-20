#ifndef UNARY_OPERATIONS_H
#define UNARY_OPERATIONS_H
#include "TileMap/UnaryOp.hpp"

namespace StealthWorldGenerator {
    template <typename Derived, typename UnaryOperation>
    constexpr UnaryOp<Derived, UnaryOperation> apply(const UnaryOperation& op, const Derived& lhs) {
        return UnaryOp<Derived, UnaryOperation>(op, lhs);
    }
} /* StealthWorldGenerator */

#endif
