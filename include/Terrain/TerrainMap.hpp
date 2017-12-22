#ifndef STEALTH_TERRAIN_MAP_H
#define STEALTH_TERRAIN_MAP_H
#include "Noise/NoiseGenerator.hpp"
#include "TileMap/TileMap.hpp"
#include "Color/Color.hpp"
#include "Color/ColorPalette.hpp"

namespace StealthWorldGenerator {
    template <int rowsAtCompileTime, int colsAtCompileTime>
    class TerrainMap {
        // Simple container for terrain related data.
        public:
            typedef TileMapF<rowsAtCompileTime, colsAtCompileTime> NoiseMap;

            TerrainMap(NoiseMap elevation) noexcept : elevation{std::move(elevation)} { }

            const NoiseMap& getElevationMap() const noexcept {
                return elevation;
            }

            TerrainMap& setElevationMap(NoiseMap elevation) noexcept {
                elevation = std::move(elevation);
                return *this;
            }
        private:
            NoiseMap elevation;
    };
} /* StealthWorldGenerator */

#endif
