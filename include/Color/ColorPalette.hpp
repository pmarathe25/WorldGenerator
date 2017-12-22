#ifndef COLOR_PALETTE
#define COLOR_PALETTE
#include "TileMap/TileMap.hpp"
#include "Color/Color.hpp"
#include <vector>

namespace StealthWorldGenerator {
    class DiscreteColorPalette : public std::vector<Color> {
        public:
            const Color& operator()(float val) const;
    };

    class GradientColorPalette {
        public:
            GradientColorPalette(const Color& colorA, const Color& colorB);

            Color operator()(float val) const;
        private:
            Color colorA, colorB;
    };

    template <int rowsAtCompileTime, int colsAtCompileTime>
    using ColorMap = TileMap<Color, rowsAtCompileTime, colsAtCompileTime>;

    template <int rows, int cols, typename ScalarType, typename ColPal>
    ColorMap<rows, cols> applyPalette(const ColPal& palette, const TileMap<ScalarType, rows, cols>& map) {
        return apply(palette, map);
    }
} /* StealthWorldGenerator */

#endif
