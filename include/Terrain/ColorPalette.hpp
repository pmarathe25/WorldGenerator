#ifndef COLOR_PALETTE
#define COLOR_PALETTE
#include "TileMap/TileMap.hpp"
#include "Terrain/Color.hpp"
#include <vector>

namespace StealthWorldGenerator {
    class DiscreteColorPalette : public std::vector<Color> {
        public:
            const Color& operator()(float elevation) const {
                return (elevation == 1) ? (*this)[this -> size() - 1] : (*this)[(int) (this -> size() * elevation)];
            }
    };

    template <int rows, int cols, typename ScalarType, typename ColPal>
    ColorMap<rows, cols> applyPalette(const ColPal& palette, const TileMap<ScalarType, rows, cols>& map) {
        return apply(palette, map);
    }
} /* StealthWorldGenerator */

#endif
