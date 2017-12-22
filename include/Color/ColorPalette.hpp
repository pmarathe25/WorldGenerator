#ifndef COLOR_PALETTE
#define COLOR_PALETTE
#include "TileMap/TileMap.hpp"
#include "Color/Color.hpp"
#include <vector>

namespace StealthWorldGenerator {
    class DiscreteColorPalette : public std::vector<Color> {
        public:
            const Color& operator()(float val) const {
                return (val == 1.0f) ? (*this)[this -> size() - 1] : (*this)[(int) (this -> size() * val)];
            }
    };

    class GradientColorPalette {
        public:
            GradientColorPalette(const Color& colorA, const Color& colorB) : colorA(colorA), colorB(colorB) { }

            Color operator()(float val) const {
                float inv = 1.0f - val;
                return Color((inv * colorA.r + val * colorB.r), (inv * colorA.g + val * colorB.g),
                    (inv * colorA.b + val * colorB.b), (inv * colorA.alpha + val * colorB.alpha));
            }
        private:
            Color colorA, colorB;
    };

    template <int rows, int cols, typename ScalarType, typename ColPal>
    ColorMap<rows, cols> applyPalette(const ColPal& palette, const TileMap<ScalarType, rows, cols>& map) {
        return apply(palette, map);
    }
} /* StealthWorldGenerator */

#endif
