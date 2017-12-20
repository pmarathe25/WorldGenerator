#ifndef OPS_H
#define OPS_H
#include <iostream>

namespace StealthWorldGenerator {
    namespace internal {
        namespace ops {
            template <typename LHS, typename RHS, typename ret = LHS>
            inline ret add(const LHS& lhs, const RHS& rhs) {
                return lhs + rhs;
            }
            
            template <typename LHS, typename RHS, typename ret = LHS>
            inline ret add(const LHS& lhs, const RHS& rhs) {
                return lhs + rhs;
            }

            template <typename LHS, typename RHS, typename ret = LHS>
            inline ret multiply(const LHS& lhs, const RHS& rhs) {
                return lhs * rhs;
            }
        } /* ops */
    } /* internal */
} /* StealthWorldGenerator */

#endif
