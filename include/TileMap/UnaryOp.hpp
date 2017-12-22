#ifndef UNARY_OP_H
#define UNARY_OP_H
#include "TileMap/TileMapBase.hpp"

namespace StealthWorldGenerator {
    namespace internal {
        template <typename LHS, typename UnaryOperation>
        struct traits<UnaryOp<LHS, UnaryOperation>> {
            typedef typename std::invoke_result<UnaryOperation, typename internal::traits<LHS>::ScalarType>::type ScalarType;
            // Dimensions
            static constexpr int rows = internal::traits<LHS>::rows,
                cols = internal::traits<LHS>::cols,
                size = internal::traits<LHS>::size;
        };
    } /* internal */

    template <typename LHS, typename UnaryOperation>
    class UnaryOp : public TileMapBase<UnaryOp<LHS, UnaryOperation>> {
        public:
            typedef typename internal::traits<UnaryOp>::ScalarType ScalarType;
            // Dimensions
            static constexpr int rows = internal::traits<UnaryOp>::rows, cols = internal::traits<UnaryOp>::cols,
                size = internal::traits<UnaryOp>::size;

            constexpr UnaryOp(const UnaryOperation& op, const LHS& lhs) noexcept
                : op(op), lhs(lhs) { }

            constexpr ScalarType operator[](int index) const {
                return op(lhs[index]);
            }

            constexpr ScalarType at(int i, int j) const {
                return op(lhs.at(i, j));
            }

            constexpr TileMap<ScalarType, rows, cols> eval() {
                return (*this);
            }
        private:
            const LHS& lhs;
            const UnaryOperation& op;
    };
} /* StealthWorldGenerator */

#endif
