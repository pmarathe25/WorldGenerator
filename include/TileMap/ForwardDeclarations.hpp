#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H
#include <type_traits>

namespace StealthWorldGenerator {
    namespace internal {
        template <typename T> struct traits {
            typedef T ScalarType;
            static constexpr int rows = 1, cols = 1, size = 1;
        };
    } /* internal */

    // TileMapBase
    template <typename Derived>
    class TileMapBase;

    // TileMap
    template <typename type, int rowsAtCompileTime, int colsAtCompileTime, int sizeAtCompileTime = rowsAtCompileTime * colsAtCompileTime>
    class TileMap;

    // Binary Op
    template <typename LHS, typename RHS>
    using BinaryOperation = typename std::common_type<LHS, RHS>::type (*)(const LHS&, const RHS&);
    template <typename LHS, typename RHS, BinaryOperation<typename internal::traits<LHS>::ScalarType, typename internal::traits<RHS>::ScalarType> op>
    class BinaryOp;

    // Unary Op
    template <typename LHS>
    using UnaryOperation = LHS (*)(const LHS&);
    template <typename LHS, UnaryOperation<typename internal::traits<LHS>::ScalarType> op>
    class UnaryOp;

    // Apply Op
    template <typename LHS, typename ApplyOperation>
    class ApplyOp;
} /* StealthWorldGenerator */

#endif
