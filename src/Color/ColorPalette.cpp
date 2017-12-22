#include "Color/ColorPalette.hpp"

namespace StealthWorldGenerator {
    GradientColorPalette::GradientColorPalette(Color colorA, Color colorB) noexcept
        : colorA(std::move(colorA)), colorB(std::move(colorB)) { }
} /* StealthWorldGenerator */
