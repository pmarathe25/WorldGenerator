#ifndef COLOR_PALETTE
#define COLOR_PALETTE
#include "Color/Color.hpp"
#include "Color/ColorMap.hpp"
#include <vector>

namespace StealthWorldGenerator {
    class DiscreteColorPalette : public std::vector<Color> {
        public:
            const Color& operator()(float val) const;
    };

    class GradientColorPalette {
        public:
            GradientColorPalette(Color colorA, Color colorB) noexcept;

            Color operator()(float val) const noexcept;
        private:
            Color colorA, colorB;
    };

    template <int rows, int cols, typename ScalarType, typename ColPal>
    ColorMap<rows, cols> applyPalette(const ColPal& palette, const TileMap<ScalarType, rows, cols>& map) {
        return apply(palette, map);
    }
} /* StealthWorldGenerator */

#endif
