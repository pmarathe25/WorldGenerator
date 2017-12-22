#ifndef BINARY_OP_H
#define BINARY_OP_H
#include "TileMap/TileMapBase.hpp"
#include <type_traits>

namespace StealthWorldGenerator {
    namespace internal {
        template <typename LHS, typename RHS, BinaryOperation<typename internal::traits<LHS>::ScalarType,
            typename internal::traits<RHS>::ScalarType> op>
        struct traits<BinaryOp<LHS, RHS, op>> {
            typedef typename internal::traits<LHS>::ScalarType ScalarTypeLHS;
            typedef typename internal::traits<RHS>::ScalarType ScalarTypeRHS;
            typedef typename std::common_type<ScalarTypeLHS, ScalarTypeRHS>::type ScalarType;
            // Dimensions
            static constexpr int rows = (std::is_scalar<LHS>::value ? internal::traits<RHS>::rows : internal::traits<LHS>::rows),
                cols = (std::is_scalar<LHS>::value ? internal::traits<RHS>::cols : internal::traits<LHS>::cols),
                size = (std::is_scalar<LHS>::value ? internal::traits<RHS>::size : internal::traits<LHS>::size);
        };
    } /* internal */

    template <typename LHS, typename RHS, BinaryOperation<typename internal::traits<LHS>::ScalarType,
        typename internal::traits<RHS>::ScalarType> op>
    class BinaryOp : public TileMapBase<BinaryOp<LHS, RHS, op>> {
        public:
            typedef typename internal::traits<BinaryOp>::ScalarType ScalarType;
            typedef typename internal::traits<BinaryOp>::ScalarTypeLHS ScalarTypeLHS;
            typedef typename internal::traits<BinaryOp>::ScalarTypeRHS ScalarTypeRHS;
            // Dimensions
            static constexpr int rows = internal::traits<BinaryOp>::rows, cols = internal::traits<BinaryOp>::cols,
                size = internal::traits<BinaryOp>::size;

            constexpr BinaryOp(const LHS& lhs, const RHS& rhs) noexcept
                : lhs(lhs), rhs(rhs) { }

            constexpr ScalarType operator[](int index) const {
                if constexpr (std::is_scalar<RHS>::value) {
                    return op(lhs[index], rhs);
                } else if constexpr (std::is_scalar<LHS>::value) {
                    return op(lhs, rhs[index]);
                } else {
                    return op(lhs[index], rhs[index]);
                }
            }

            constexpr ScalarType at(int i, int j) const {
                if constexpr (std::is_scalar<RHS>::value) {
                    return op(lhs.at(i, j), rhs);
                } else if constexpr (std::is_scalar<LHS>::value) {
                    return op(lhs, rhs.at(i, j));
                } else {
                    return op(lhs.at(i, j), rhs.at(i, j));
                }
            }

            constexpr TileMap<ScalarType, rows, cols> eval() {
                return (*this);
            }
        private:
            const LHS& lhs;
            const RHS& rhs;
    };
} /* StealthWorldGenerator */

#endif
