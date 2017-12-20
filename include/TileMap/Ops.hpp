#ifndef OPS_H
#define OPS_H
#include <iostream>
#include <type_traits>

namespace StealthWorldGenerator {
    namespace internal {
        namespace ops {
            template <typename LHS, typename RHS>
            inline typename std::common_type<LHS, RHS>::type add(const LHS& lhs, const RHS& rhs) {
                return lhs + rhs;
            }

            template <typename LHS, typename RHS>
            inline typename std::common_type<LHS, RHS>::type subtract(const LHS& lhs, const RHS& rhs) {
                return lhs - rhs;
            }

            template <typename LHS, typename RHS>
            inline typename std::common_type<LHS, RHS>::type multiply(const LHS& lhs, const RHS& rhs) {
                return lhs * rhs;
            }

            template <typename LHS, typename RHS>
            inline typename std::common_type<LHS, RHS>::type divide(const LHS& lhs, const RHS& rhs) {
                return lhs / rhs;
            }
        } /* ops */
    } /* internal */
} /* StealthWorldGenerator */

#endif
