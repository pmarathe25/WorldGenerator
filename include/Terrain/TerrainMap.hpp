#ifndef STEALTH_TERRAIN_MAP_H
#define STEALTH_TERRAIN_MAP_H
#include "Noise/NoiseGenerator.hpp"
#include "TileMap/TileMap.hpp"
#include "Color/Color.hpp"
#include "Color/ColorPalette.hpp"

namespace StealthWorldGenerator {
    template <int rowsAtCompileTime, int colsAtCompileTime>
    class TerrainMap {
        public:
            typedef TileMapF<rowsAtCompileTime, colsAtCompileTime> NoiseMap;

            TerrainMap(NoiseMap elevation) : elevation(elevation) { }

            const TileMapF<rowsAtCompileTime, colsAtCompileTime>& getElevationMap() {
                return elevation;
            }
        private:
            TileMapF<rowsAtCompileTime, colsAtCompileTime> elevation;
    };
} /* StealthWorldGenerator */

#endif
