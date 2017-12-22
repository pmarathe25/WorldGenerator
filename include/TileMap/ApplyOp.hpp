#ifndef APPLY_OP_H
#define APPLY_OP_H
#include "TileMap/TileMapBase.hpp"

namespace StealthWorldGenerator {
    namespace internal {
        template <typename LHS, typename ApplyOperation>
        struct traits<ApplyOp<LHS, ApplyOperation>> {
            typedef typename std::invoke_result<ApplyOperation, typename internal::traits<LHS>::ScalarType>::type ScalarType;
            // Dimensions
            static constexpr int rows = internal::traits<LHS>::rows,
                cols = internal::traits<LHS>::cols,
                size = internal::traits<LHS>::size;
        };
    } /* internal */

    template <typename LHS, typename ApplyOperation>
    class ApplyOp : public TileMapBase<ApplyOp<LHS, ApplyOperation>> {
        public:
            typedef typename internal::traits<ApplyOp>::ScalarType ScalarType;
            // Dimensions
            static constexpr int rows = internal::traits<ApplyOp>::rows, cols = internal::traits<ApplyOp>::cols,
                size = internal::traits<ApplyOp>::size;

            constexpr ApplyOp(const ApplyOperation& op, const LHS& lhs) noexcept
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
            const ApplyOperation& op;
    };
} /* StealthWorldGenerator */

#endif
