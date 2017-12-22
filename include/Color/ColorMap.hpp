#ifndef COLOR_MAP_H
#define COLOR_MAP_H
#include "Color/Color.hpp"
#include "TileMap/TileMap.hpp"
#include <SFML/Graphics.hpp>

namespace StealthWorldGenerator {
    template <int rowsAtCompileTime, int colsAtCompileTime>
    using ColorMap = TileMap<Color, rowsAtCompileTime, colsAtCompileTime>;

    template <int rows, int cols>
    constexpr sf::Image imageFromColorMap(const ColorMap<rows, cols>& colors) {
        sf::Image im;
        im.create(rows, cols, (uint8_t*) colors.data());
        return im;
    }
} /* StealthWorldGenerator */

#endif
