#ifndef COLOR_PALETTE
#define COLOR_PALETTE
#include "Color/Color.hpp"
#include "TileMap/TileMap.hpp"
#include <vector>

namespace StealthWorldGenerator {
    class DiscreteColorPalette : public std::vector<Color> {
        public:
            DiscreteColorPalette(const DiscreteColorPalette& other) = default;

            DiscreteColorPalette(DiscreteColorPalette&& other) noexcept = default;

            DiscreteColorPalette& operator=(DiscreteColorPalette&& other) noexcept = default;

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

    template <int width, int length, typename ScalarType, typename ColorPalette>
    StealthTileMap::TileMap<Color, width, length> applyPalette(const ColorPalette& palette, const StealthTileMap::TileMap<ScalarType, width, length>& map) {
        return apply(palette, map);
    }
} /* StealthWorldGenerator */

#endif
