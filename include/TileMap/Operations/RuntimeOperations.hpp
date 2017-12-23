#ifndef RUNTIME_OPERATIONS_H
#define RUNTIME_OPERATIONS_H
#include "TileMap/RuntimeOp.hpp"

namespace StealthWorldGenerator {
    template <typename Derived, typename RuntimeOperation>
    constexpr RuntimeOp<Derived, RuntimeOperation> apply(const RuntimeOperation& op, const Derived& lhs) noexcept {
        return RuntimeOp<Derived, RuntimeOperation>{op, lhs};
    }
} /* StealthWorldGenerator */

#endif
