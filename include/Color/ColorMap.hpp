#ifndef COLOR_MAP_H
#define COLOR_MAP_H
#include "Color/Color.hpp"
#include "TileMap/TileMap.hpp"

namespace StealthWorldGenerator {
    template <int rowsAtCompileTime, int colsAtCompileTime>
    using ColorMap = TileMap<Color, rowsAtCompileTime, colsAtCompileTime>;
} /* StealthWorldGenerator */

#endif
