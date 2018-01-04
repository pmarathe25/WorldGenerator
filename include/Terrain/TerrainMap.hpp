#ifndef STEALTH_TERRAIN_MAP_H
#define STEALTH_TERRAIN_MAP_H
#include "TileMap/TileMap.hpp"

namespace StealthWorldGenerator {
    enum TerrainMapType {
        Elevation = 0,
        WaterTable,
        Foliage,
        NUM_MAP_TYPES
    };

    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime = 1>
    class TerrainMap : public std::array<typename StealthTileMap::TileMapF<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>, NUM_MAP_TYPES> {
        // Simple container for terrain related data.
        public:
            typedef typename StealthTileMap::TileMapF<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime> InternalTerrainNoiseMap;

            constexpr TerrainMap() noexcept = default;

            template <typename... Args>
            constexpr TerrainMap(Args&&... args) noexcept {
                setMaps(std::forward<Args&&>(args)...);
            }

            template <int mapNum = 0, typename Map, typename... Args>
            constexpr TerrainMap& setMaps(Map map, Args&&... args) {
                this -> operator[](mapNum) = std::move(map);
                if constexpr (sizeof...(args) != 0) return setMaps<mapNum + 1>(std::forward<Args&&>(args)...);
                else return *this;
            }

            template <int mapNum>
            constexpr const InternalTerrainNoiseMap& get() const noexcept {
                return this -> operator[](mapNum);
            }

            template <int mapNum>
            constexpr TerrainMap& set(InternalTerrainNoiseMap terrainMap) noexcept {
                this -> operator[](mapNum) = std::move(terrainMap);
                return *this;
            }
    };
} /* StealthWorldGenerator */

#endif
