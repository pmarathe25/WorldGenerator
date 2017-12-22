#ifndef BINARY_OPERATIONS_H
#define BINARY_OPERATIONS_H
#define CHECK_TILEMAP_COMPAT(Derived, OtherDerived) static_assert((internal::traits<Derived>::rows == internal::traits<OtherDerived>::rows \
    && internal::traits<Derived>::cols == internal::traits<OtherDerived>::cols) \
    || (internal::traits<Derived>::size == 1 || internal::traits<OtherDerived>::size == 1), "Cannot operate on incompatible arguments");
#include "TileMap/BinaryOp.hpp"
#include "TileMap/Operations/InternalOperations.hpp"

namespace StealthWorldGenerator {
    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::add> operator+(const Derived& lhs, const OtherDerived& rhs) noexcept {
        CHECK_TILEMAP_COMPAT(Derived, OtherDerived);
        return BinaryOp<Derived, OtherDerived, internal::ops::add>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::subtract> operator-(const Derived& lhs, const OtherDerived& rhs) noexcept {
        CHECK_TILEMAP_COMPAT(Derived, OtherDerived);
        return BinaryOp<Derived, OtherDerived, internal::ops::subtract>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::multiply> operator*(const Derived& lhs, const OtherDerived& rhs) noexcept {
        CHECK_TILEMAP_COMPAT(Derived, OtherDerived);
        return BinaryOp<Derived, OtherDerived, internal::ops::multiply>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::divide> operator/(const Derived& lhs, const OtherDerived& rhs) noexcept {
        CHECK_TILEMAP_COMPAT(Derived, OtherDerived);
        return BinaryOp<Derived, OtherDerived, internal::ops::divide>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::eq> operator==(const Derived& lhs, const OtherDerived& rhs) noexcept {
        CHECK_TILEMAP_COMPAT(Derived, OtherDerived);
        return BinaryOp<Derived, OtherDerived, internal::ops::eq>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::less> operator<(const Derived& lhs, const OtherDerived& rhs) noexcept {
        CHECK_TILEMAP_COMPAT(Derived, OtherDerived);
        return BinaryOp<Derived, OtherDerived, internal::ops::less>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::lessEq> operator<=(const Derived& lhs, const OtherDerived& rhs) noexcept {
        CHECK_TILEMAP_COMPAT(Derived, OtherDerived);
        return BinaryOp<Derived, OtherDerived, internal::ops::lessEq>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::greater> operator>(const Derived& lhs, const OtherDerived& rhs) noexcept {
        CHECK_TILEMAP_COMPAT(Derived, OtherDerived);
        return BinaryOp<Derived, OtherDerived, internal::ops::greater>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::greaterEq> operator>=(const Derived& lhs, const OtherDerived& rhs) noexcept {
        CHECK_TILEMAP_COMPAT(Derived, OtherDerived);
        return BinaryOp<Derived, OtherDerived, internal::ops::greaterEq>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::andOp> operator&&(const Derived& lhs, const OtherDerived& rhs) noexcept {
        CHECK_TILEMAP_COMPAT(Derived, OtherDerived);
        return BinaryOp<Derived, OtherDerived, internal::ops::andOp>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::orOp> operator||(const Derived& lhs, const OtherDerived& rhs) noexcept {
        CHECK_TILEMAP_COMPAT(Derived, OtherDerived);
        return BinaryOp<Derived, OtherDerived, internal::ops::orOp>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::min> min(const Derived& lhs, const OtherDerived& rhs) noexcept {
        CHECK_TILEMAP_COMPAT(Derived, OtherDerived);
        return BinaryOp<Derived, OtherDerived, internal::ops::min>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::max> max(const Derived& lhs, const OtherDerived& rhs) noexcept {
        CHECK_TILEMAP_COMPAT(Derived, OtherDerived);
        return BinaryOp<Derived, OtherDerived, internal::ops::max>{lhs, rhs};
    }
} /* StealthWorldGenerator */

#endif
