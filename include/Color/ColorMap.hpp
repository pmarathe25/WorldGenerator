#ifndef COLOR_MAP_H
#define COLOR_MAP_H
#include "Color/Color.hpp"
#include "TileMap/TileMap.hpp"
#include <SFML/Graphics.hpp>

namespace StealthWorldGenerator {
    template <int rowsAtCompileTime, int colsAtCompileTime>
    using ColorMap = TileMap<Color, rowsAtCompileTime, colsAtCompileTime>;

    template <int rows, int cols>
    constexpr sf::Sprite spriteFromColorMap(const ColorMap<rows, cols>& colors, sf::Texture& texture) {
        sf::Image im;
        sf::Sprite sprite;
        im.create(rows, cols, (uint8_t*) colors.data());
        texture.loadFromImage(im);
        sprite.setTexture(texture);
        return sprite;
    }
} /* StealthWorldGenerator */

#endif
