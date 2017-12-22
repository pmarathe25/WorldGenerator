#ifndef COLOR_PALETTE
#define COLOR_PALETTE
#include "Color/Color.hpp"
#include "Color/ColorMap.hpp"
#include <vector>

namespace StealthWorldGenerator {
    class DiscreteColorPalette : public std::vector<Color> {
        public:
            template <typename ScalarType>
            const Color& operator()(ScalarType val) const {
                return (val >= ScalarType(1)) ? (*this)[this -> size() - 1] : (*this)[(int) (this -> size() * val)];
            }
    };

    class GradientColorPalette {
        public:
            GradientColorPalette(Color colorA, Color colorB) noexcept;

            template <typename ScalarType>
            Color operator()(ScalarType val) const noexcept {
                ScalarType inv = ScalarType(1) - val;
                return Color{(uint8_t) (inv * colorA.r + val * colorB.r), (uint8_t) (inv * colorA.g + val * colorB.g),
                    (uint8_t) (inv * colorA.b + val * colorB.b), (uint8_t) (inv * colorA.alpha + val * colorB.alpha)};
            }
        private:
            Color colorA, colorB;
    };

    template <int rows, int cols, typename ScalarType, typename ColorPalette>
    ColorMap<rows, cols> applyPalette(const ColorPalette& palette, const TileMap<ScalarType, rows, cols>& map) {
        return apply(palette, map);
    }
} /* StealthWorldGenerator */

#endif
