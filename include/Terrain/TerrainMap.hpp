#ifndef STEALTH_TERRAIN_MAP_H
#define STEALTH_TERRAIN_MAP_H
#include "Noise/NoiseMap.hpp"
#include "TileMap/TileMap.hpp"
#include "Color/Color.hpp"
#include "Color/ColorPalette.hpp"

namespace StealthWorldGenerator {
    template <int rowsAtCompileTime, int colsAtCompileTime>
    using TerrainNoiseMap = NoiseMap<rowsAtCompileTime, colsAtCompileTime>;

    template <int rowsAtCompileTime, int colsAtCompileTime>
    class TerrainMap {
        // Simple container for terrain related data.
        public:
            typedef TerrainNoiseMap<rowsAtCompileTime, colsAtCompileTime> InternalTerrainNoiseMap;

            TerrainMap() noexcept = default;

            TerrainMap(InternalTerrainNoiseMap elevation, InternalTerrainNoiseMap waterTable, InternalTerrainNoiseMap foliage)
                noexcept : elevation{std::move(elevation)}, waterTable{std::move(waterTable)}, foliage{std::move(foliage)} {
            }

            const InternalTerrainNoiseMap& getElevationMap() const noexcept {
                return elevation;
            }

            TerrainMap& setElevationMap(InternalTerrainNoiseMap elevation) noexcept {
                this -> elevation = std::move(elevation);
                return *this;
            }

            const InternalTerrainNoiseMap& getFoliageMap() const noexcept {
                return foliage;
            }

            TerrainMap& setFoliageMap(InternalTerrainNoiseMap foliage) noexcept {
                this -> foliage = std::move(foliage);
                return *this;
            }

            const InternalTerrainNoiseMap& getWaterTable() const noexcept {
                return waterTable;
            }

            TerrainMap& setWaterTable(InternalTerrainNoiseMap waterTable) noexcept {
                this -> waterTable = std::move(waterTable);
                return *this;
            }
        private:
            InternalTerrainNoiseMap elevation, foliage, waterTable;
    };
} /* StealthWorldGenerator */

#endif
