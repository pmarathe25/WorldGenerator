#ifndef APPLY_OPERATIONS_H
#define APPLY_OPERATIONS_H
#include "TileMap/ApplyOp.hpp"

namespace StealthWorldGenerator {
    template <typename Derived, typename ApplyOperation>
    constexpr ApplyOp<Derived, ApplyOperation> apply(const ApplyOperation& op, const Derived& lhs) noexcept {
        return ApplyOp<Derived, ApplyOperation>{op, lhs};
    }
} /* StealthWorldGenerator */

#endif
