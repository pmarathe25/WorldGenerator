#ifndef INTERNAL_OPERATIONS_H
#define INTERNAL_OPERATIONS_H
#include <algorithm>
#include <type_traits>

namespace StealthWorldGenerator::internal::ops {
    template <typename LHS, typename RHS>
    constexpr auto add(const LHS& lhs, const RHS& rhs) {
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
    constexpr typename std::common_type<LHS, RHS>::type eq(const LHS& lhs, const RHS& rhs) {
        return lhs == rhs;
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

    template <typename LHS, typename RHS>
    constexpr typename std::common_type<LHS, RHS>::type min(const LHS& lhs, const RHS& rhs) {
        return std::min(lhs, rhs);
    }

    template <typename LHS, typename RHS>
    constexpr typename std::common_type<LHS, RHS>::type max(const LHS& lhs, const RHS& rhs) {
        return std::max(lhs, rhs);
    }

    template <typename LHS>
    constexpr LHS notOp(const LHS& lhs) {
        return !lhs;
    }

} /* StealthWorldGenerator::internal::ops */

#endif
