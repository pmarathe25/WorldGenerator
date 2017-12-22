#ifndef UNARY_OP_H
#define UNARY_OP_H
#include "TileMap/TileMapBase.hpp"

namespace StealthWorldGenerator {
    namespace internal {
        template <typename LHS, UnaryOperation<typename internal::traits<LHS>::ScalarType> op>
        struct traits<UnaryOp<LHS, op>> {
            typedef typename internal::traits<LHS>::ScalarType ScalarType;
            // Dimensions
            static constexpr int rows = internal::traits<LHS>::rows,
                cols = internal::traits<LHS>::cols,
                size = internal::traits<LHS>::size;
        };
    } /* internal */

    template <typename LHS, UnaryOperation<typename internal::traits<LHS>::ScalarType> op>
    class UnaryOp : public TileMapBase<UnaryOp<LHS, op>> {
        public:
            typedef typename internal::traits<UnaryOp>::ScalarType ScalarType;
            // Dimensions
            static constexpr int rows = internal::traits<UnaryOp>::rows, cols = internal::traits<UnaryOp>::cols,
                size = internal::traits<UnaryOp>::size;

            constexpr UnaryOp(const LHS& lhs) noexcept
                : lhs(lhs) { }

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
    };
} /* StealthWorldGenerator */

#endif
