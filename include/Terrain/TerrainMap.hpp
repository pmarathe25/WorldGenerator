#ifndef STEALTH_TERRAIN_MAP_H
#define STEALTH_TERRAIN_MAP_H
#include "TileMap/TileMap.hpp"
#include <unordered_map>

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
    class TerrainMap : public std::unordered_map<int, typename StealthTileMap::TileMapF<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>> {
        // Simple container for terrain related data.
        public:
            typedef typename StealthTileMap::TileMapF<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime> InternalTerrainNoiseMap;
            typedef typename std::unordered_map<int, typename StealthTileMap::TileMapF<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>> super;

            constexpr TerrainMap() noexcept : super{} { }

            template <typename... Args>
            constexpr TerrainMap(InternalTerrainNoiseMap&& map, Args&&... args) noexcept {
                setMaps(std::forward<InternalTerrainNoiseMap&&>(map), std::forward<Args&&>(args)...);
            }

            template <int index = 0, typename... Args>
            constexpr TerrainMap& setMaps(InternalTerrainNoiseMap&& map, Args&&... args) {
                this -> operator[](index) = std::move(map);
                if constexpr (sizeof...(args) != 0) return setMaps<index + 1>(std::forward<Args&&>(args)...);
                else return *this;
            }

            constexpr TerrainMap& set(int index, InternalTerrainNoiseMap terrainMap) noexcept {
                this -> operator[](index) = std::move(terrainMap);
                return *this;
            }

            constexpr const InternalTerrainNoiseMap& operator[](int index) const {
                return super::at(index);
            }

            constexpr InternalTerrainNoiseMap& operator[](int index) {
                return super::operator[](index);
            }

            int numMapTypes() const {
                return super::size();
            }
    };
} /* StealthWorldGenerator */

#endif
