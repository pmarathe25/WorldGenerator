#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H

namespace StealthWorldGenerator {
    namespace internal {
        template <typename T> struct traits {
            typedef T ScalarType;
            static constexpr int rows = 1, cols = 1, size = 1;
        };
    } /* internal */

    // TileMapBase
    template <typename Derived> class TileMapBase;

    // TileMap
    template <typename type, int rowsAtCompileTime, int colsAtCompileTime, int sizeAtCompileTime = rowsAtCompileTime * colsAtCompileTime>
    class TileMap;

    // Binary Op
    template <typename LHS, typename RHS, typename ret = LHS>
    using BinaryOperation = ret (*)(const LHS&, const RHS&);

    template <typename LHS, typename RHS,
        BinaryOperation<typename internal::traits<LHS>::ScalarType, typename internal::traits<RHS>::ScalarType> op>
    class BinaryOp;
} /* StealthWorldGenerator */

#endif
