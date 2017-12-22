#ifndef STEALTH_TERRAIN_MAP_H
#define STEALTH_TERRAIN_MAP_H
#include "Noise/NoiseMap.hpp"
#include "TileMap/TileMap.hpp"
#include "Color/Color.hpp"
#include "Color/ColorPalette.hpp"

namespace StealthWorldGenerator {
    template <int rowsAtCompileTime, int colsAtCompileTime>
    class TerrainMap {
        // Simple container for terrain related data.
        public:
            typedef NoiseMap<rowsAtCompileTime, colsAtCompileTime> TerrainNoiseMap;
            typedef TileMap<bool, rowsAtCompileTime, colsAtCompileTime> WaterLevelMap;

            TerrainMap(TerrainNoiseMap elevation, float waterLevel = 0.5f) noexcept : elevation{std::move(elevation)} {
                setWaterLevel(waterLevel);
            }

            const TerrainNoiseMap& getElevationMap() const noexcept {
                return elevation;
            }

            const WaterLevelMap& getWaterTable() const noexcept {
                return waterTable;
            }

            TerrainMap& setElevationMap(TerrainNoiseMap elevation) noexcept {
                elevation = std::move(elevation);
                return *this;
            }

            TerrainMap& setWaterLevel(float newLevel) noexcept {
                waterTable = elevation < newLevel;
            }
        private:
            TerrainNoiseMap elevation;
            WaterLevelMap waterTable;
    };
} /* StealthWorldGenerator */

#endif
