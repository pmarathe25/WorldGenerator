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

            TerrainMap() noexcept = default;

            TerrainMap(NoiseMap elevation) : elevation(elevation) { }

            const TileMapF<rowsAtCompileTime, colsAtCompileTime>& getElevationMap() {
                return elevation;
            }
        private:
            TileMapF<rowsAtCompileTime, colsAtCompileTime> elevation;
    };

    template <int rowsAtCompileTime, int colsAtCompileTime, int scale, int numOctaves = 8>
    TerrainMap<rowsAtCompileTime, colsAtCompileTime> generateTerrain() {
        NoiseGenerator noiseGenerator;
        return TerrainMap{noiseGenerator.generateOctaves<rowsAtCompileTime, colsAtCompileTime, scale, numOctaves>()};
    }
} /* StealthWorldGenerator */

#endif
