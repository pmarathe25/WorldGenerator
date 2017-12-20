#ifndef UNARY_OP_H
#define UNARY_OP_H
#include "TileMap/TileMapBase.hpp"
#include "TileMap/Ops.hpp"

namespace StealthWorldGenerator {
    namespace internal {
        template <typename LHS, typename UnaryOperation>
        struct traits<UnaryOp<LHS, UnaryOperation>> {
            typedef typename std::invoke_result<UnaryOperation, typename internal::traits<LHS>::ScalarType>::type ScalarType;
            static constexpr int rows = internal::traits<LHS>::rows,
                cols = internal::traits<LHS>::cols,
                size = internal::traits<LHS>::size;
        };
    } /* internal */

    template <typename LHS, typename UnaryOperation>
    class UnaryOp : public TileMapBase<UnaryOp<LHS, UnaryOperation>> {
        public:
            typedef typename internal::traits<UnaryOp>::ScalarType ScalarType;
            static constexpr int rows = internal::traits<UnaryOp>::rows, cols = internal::traits<UnaryOp>::cols,
                size = internal::traits<UnaryOp>::size;

            constexpr UnaryOp(UnaryOperation& op, const LHS& lhs) : op(op), lhs(lhs) {

            }

            constexpr ScalarType operator[](int index) const {
                return op(lhs[index]);
            }

            constexpr ScalarType at(int i, int j) const {
                return op(lhs.at(i, j));
            }

        private:
            const LHS& lhs;
            UnaryOperation& op;
    };

    template <typename Derived, typename UnaryOperation>
    constexpr UnaryOp<Derived, UnaryOperation> apply(UnaryOperation& op, const Derived& lhs) {
        return UnaryOp<Derived, UnaryOperation>(op, lhs);
    }

} /* StealthWorldGenerator */

#endif
