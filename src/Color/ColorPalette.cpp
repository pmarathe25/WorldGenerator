#include "Color/ColorPalette.hpp"

namespace StealthWorldGenerator {
    const Color& DiscreteColorPalette::operator()(float val) const {
        return (val == 1.0f) ? (*this)[this -> size() - 1] : (*this)[(int) (this -> size() * val)];
    }

    GradientColorPalette::GradientColorPalette(const Color& colorA, const Color& colorB) : colorA(colorA), colorB(colorB) { }

    Color GradientColorPalette::operator()(float val) const {
        float inv = 1.0f - val;
        return Color((inv * colorA.r + val * colorB.r), (inv * colorA.g + val * colorB.g),
            (inv * colorA.b + val * colorB.b), (inv * colorA.alpha + val * colorB.alpha));
    }
} /* StealthWorldGenerator */
