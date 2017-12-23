#ifndef NOISE_MAP_H
#define NOISE_MAP_H
#include "TileMap/TileMap.hpp"

namespace StealthWorldGenerator {
    template <int rows, int cols>
    using NoiseMap = TileMapF<rows, cols>;
} /* StealthWorldGenerator */

#endif /* end of include guard: NOISE_MAP_H */
