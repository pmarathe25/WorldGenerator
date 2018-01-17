#ifndef STEALTH_TERRAIN_MAP_H
#define STEALTH_TERRAIN_MAP_H
#include "TileMap/TileMap.hpp"

namespace StealthWorldGenerator {
    struct TerrainMapMembers {
        enum {
            Elevation = 0,
            WaterTable,
            Foliage,
            Temperature,
            Moisture,
            NumTerrainMapMembers
        };
    };

    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime = 1>
    class TerrainMap : public std::array<typename StealthTileMap::TileMapF<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>,
        TerrainMapMembers::NumTerrainMapMembers> {
        // Simple container for terrain related data.
        public:
            typedef typename StealthTileMap::TileMapF<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime> InternalTerrainNoiseMap;
            typedef typename std::array<typename StealthTileMap::TileMapF<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>,
                TerrainMapMembers::NumTerrainMapMembers> super;

            constexpr TerrainMap() noexcept : super{} { }

            template <typename... Args>
            constexpr TerrainMap(InternalTerrainNoiseMap&& map, Args&&... args) noexcept {
                setMaps(std::forward<InternalTerrainNoiseMap&&>(map), std::forward<Args&&>(args)...);
            }

            template <int mapNum = 0, typename... Args>
            constexpr TerrainMap& setMaps(InternalTerrainNoiseMap&& map, Args&&... args) {
                this -> operator[](mapNum) = std::move(map);
                if constexpr (sizeof...(args) != 0) return setMaps<mapNum + 1>(std::forward<Args&&>(args)...);
                else return *this;
            }

            constexpr TerrainMap& set(int mapNum, InternalTerrainNoiseMap terrainMap) noexcept {
                this -> operator[](mapNum) = std::move(terrainMap);
                return *this;
            }
    };
} /* StealthWorldGenerator */

#endif
