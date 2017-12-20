#ifndef INTERNAL_OPERATIONS_H
#define INTERNAL_OPERATIONS_H
#include <type_traits>

namespace StealthWorldGenerator {
    namespace internal {
        namespace ops {
            template <typename LHS, typename RHS>
            constexpr typename std::common_type<LHS, RHS>::type add(const LHS& lhs, const RHS& rhs) {
                return lhs + rhs;
            }

            template <typename LHS, typename RHS>
            constexpr typename std::common_type<LHS, RHS>::type subtract(const LHS& lhs, const RHS& rhs) {
                return lhs - rhs;
            }

            template <typename LHS, typename RHS>
            constexpr typename std::common_type<LHS, RHS>::type multiply(const LHS& lhs, const RHS& rhs) {
                return lhs * rhs;
            }

            template <typename LHS, typename RHS>
            constexpr typename std::common_type<LHS, RHS>::type divide(const LHS& lhs, const RHS& rhs) {
                return lhs / rhs;
            }

            template <typename LHS, typename RHS>
            constexpr typename std::common_type<LHS, RHS>::type less(const LHS& lhs, const RHS& rhs) {
                return lhs < rhs;
            }

            template <typename LHS, typename RHS>
            constexpr typename std::common_type<LHS, RHS>::type lessEq(const LHS& lhs, const RHS& rhs) {
                return lhs <= rhs;
            }

            template <typename LHS, typename RHS>
            constexpr typename std::common_type<LHS, RHS>::type greater(const LHS& lhs, const RHS& rhs) {
                return lhs > rhs;
            }

            template <typename LHS, typename RHS>
            constexpr typename std::common_type<LHS, RHS>::type greaterEq(const LHS& lhs, const RHS& rhs) {
                return lhs >= rhs;
            }

            template <typename LHS, typename RHS>
            constexpr typename std::common_type<LHS, RHS>::type andOp(const LHS& lhs, const RHS& rhs) {
                return lhs && rhs;
            }

            template <typename LHS, typename RHS>
            constexpr typename std::common_type<LHS, RHS>::type orOp(const LHS& lhs, const RHS& rhs) {
                return lhs || rhs;
            }
        } /* ops */
    } /* internal */
} /* StealthWorldGenerator */

#endif
