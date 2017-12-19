#ifndef BINARY_OP_H
#define BINARY_OP_H
#include "TileMap/TileMapBase.hpp"
#include "TileMap/Ops.hpp"

namespace StealthWorldGenerator {
    namespace internal {
        template <typename LHS, typename RHS,
            Operation<typename internal::traits<LHS>::ScalarType, typename internal::traits<RHS>::ScalarType> op,
            typename LHSType, typename RHSType, int rowsAtCompileTime, int colsAtCompileTime,
            int sizeAtCompileTime>
        struct traits<BinaryOp<LHS, RHS, op, LHSType, RHSType, rowsAtCompileTime, colsAtCompileTime, sizeAtCompileTime>> {
            typedef LHSType ScalarType;
            typedef LHSType ScalarTypeLHS;
            typedef RHSType ScalarTypeRHS;

            enum {
                rows = rowsAtCompileTime,
                cols = colsAtCompileTime,
                size = sizeAtCompileTime
            };
        };
    } /* internal */

    template <typename LHS, typename RHS,
        Operation<typename internal::traits<LHS>::ScalarType, typename internal::traits<RHS>::ScalarType> op,
        typename LHSType, typename RHSType, int rowsAtCompileTime, int colsAtCompileTime, int sizeAtCompileTime>
    class BinaryOp : public TileMapBase<BinaryOp<LHS, RHS, op>> {
        public:
            typedef LHSType ScalarType;
            typedef LHSType ScalarTypeLHS;
            typedef RHSType ScalarTypeRHS;

            BinaryOp(const TileMapBase<LHS>* lhs, const TileMapBase<RHS>* rhs)
                : lhs(static_cast<const LHS*>(lhs)), rhs(static_cast<const RHS*>(rhs)) { }

            inline ScalarType operator[](int index) const {
                return op((*lhs)[index], (*rhs)[index]);
            }

            inline ScalarType at(int i, int j) const {
                return op(lhs -> at(i, j), rhs -> at(i, j));
            }

        private:
            const LHS* lhs;
            const RHS* rhs;
    };

    template <typename Derived>
    template <typename OtherDerived>
    BinaryOp<Derived, OtherDerived, BinaryOps::add> TileMapBase<Derived>::operator+(const TileMapBase<OtherDerived>& rhs) {
        return BinaryOp<Derived, OtherDerived, BinaryOps::add>{this, &rhs};
    }

} /* StealthWorldGenerator */

#endif
