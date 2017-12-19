#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H

namespace StealthWorldGenerator {
    namespace internal {
        template <typename T> struct traits;
    } /* internal */

    // TileMapBase
    template <typename Derived> class TileMapBase;

    // TileMap
    template <typename type, int rowsAtCompileTime, int colsAtCompileTime, int sizeAtCompileTime = rowsAtCompileTime * colsAtCompileTime>
    class TileMap;

    // Binary Op
    template <typename LHS, typename RHS, typename ret = LHS>
    using Operation = ret (*)(const LHS&, const RHS&);
    template <typename LHS, typename RHS,
        Operation<typename internal::traits<LHS>::ScalarType, typename internal::traits<RHS>::ScalarType> op,
        typename LHSType = typename internal::traits<LHS>::ScalarType, typename RHSType =
        typename internal::traits<RHS>::ScalarType, int rowsAtCompileTime =
        internal::traits<LHS>::rows, int colsAtCompileTime = internal::traits<LHS>::cols,
        int sizeAtCompileTime = internal::traits<LHS>::size>
    class BinaryOp;
} /* StealthWorldGenerator */

#endif
