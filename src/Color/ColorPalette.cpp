#include "Color/ColorPalette.hpp"

namespace StealthWorldGenerator {
    const Color& DiscreteColorPalette::operator()(float val) const {
        return (val >= 1.0f) ? (*this)[this -> size() - 1] : (*this)[(int) (this -> size() * val)];
    }

    GradientColorPalette::GradientColorPalette(Color colorA, Color colorB) noexcept
        : colorA(std::move(colorA)), colorB(std::move(colorB)) { }

    Color GradientColorPalette::operator()(float val) const noexcept {
        float inv = 1.0f - val;
        return Color{(uint8_t) (inv * colorA.r + val * colorB.r), (uint8_t) (inv * colorA.g + val * colorB.g),
            (uint8_t) (inv * colorA.b + val * colorB.b), (uint8_t) (inv * colorA.alpha + val * colorB.alpha)};
    }
} /* StealthWorldGenerator */
