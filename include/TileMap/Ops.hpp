#ifndef OPS_H
#define OPS_H
#include <iostream>

namespace StealthWorldGenerator {
    namespace BinaryOps {
        template <typename LHS, typename RHS, typename ret = LHS>
        inline ret add(const LHS& lhs, const RHS& rhs) {
            return lhs + rhs;
        }
    } /* BinaryOps */
} /* StealthWorldGenerator */

#endif
