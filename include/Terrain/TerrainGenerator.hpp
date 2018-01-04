#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H
#include "NoiseGenerator/NoiseGenerator.hpp"
#include "Terrain/TerrainMap.hpp"
#include "Vector2.hpp"
#include <random>

namespace StealthWorldGenerator {
    class TerrainConfig {
        public:
            constexpr TerrainConfig() noexcept = default;

            constexpr TerrainConfig& setWaterLevel(float waterLevel) noexcept {
                this -> waterLevel = waterLevel;
                return *this;
            }

            constexpr TerrainConfig& setElevationBounds(float lower, float upper) noexcept {
                this -> elevationBounds = {lower, upper};
                return *this;
            }

            constexpr TerrainConfig& setFoliageElevationBounds(float lower, float upper) noexcept {
                this -> foliageElevationBounds = {lower, upper};
                return *this;
            }

            // Vector2f setters
            constexpr TerrainConfig& setElevationBounds(Vector2f elevationBounds) noexcept {
                this -> elevationBounds = std::move(elevationBounds);
                return *this;
            }

            constexpr TerrainConfig& setFoliageElevationBounds(Vector2f foliageElevationBounds) noexcept {
                this -> foliageElevationBounds = std::move(foliageElevationBounds);
                return *this;
            }

            float waterLevel = 0.0f;
            Vector2f elevationBounds = {0.0f, 1.0f}, foliageElevationBounds = {0.0f, 1.0f};
    };

    constexpr TerrainConfig createTerrainConfig() {
        return TerrainConfig{};
    }

    // 2D Terrain Maps
    template <int width, int length, int scaleX, int scaleY, int numOctaves = 8>
    constexpr TerrainMap<width, length> generateTerrainMap(const TerrainConfig& config) noexcept {
        // Create land
        StealthTileMap::TileMapF<width, length>&& elevation = StealthNoiseGenerator::generateOctaves<width, length, scaleX, scaleY, numOctaves>
            (std::uniform_real_distribution(config.elevationBounds.x, config.elevationBounds.y));
        // Create water
        StealthTileMap::TileMapF<width, length>&& waterTable = elevation <= config.waterLevel;
        // Create foliage where there's no water and the elevation is appropriate
        StealthTileMap::TileMapF<width, length>&& foliage = StealthNoiseGenerator::generateOctaves<width, length, scaleX, scaleY, numOctaves>()
            * !waterTable * ((elevation >= config.foliageElevationBounds.x) && (elevation <= config.foliageElevationBounds.y));
        return TerrainMap<width, length>{std::move(elevation), std::move(waterTable), std::move(foliage)};
    }

    // 3D Terrain Maps
    template <int width, int length, int layers, int scaleX, int scaleY, int erosionScale, int foliageGrowthScale, int numOctaves = 8>
    constexpr TerrainMap<width, length, layers> generateTerrainMap(const TerrainConfig& config) noexcept {
        // Create land
        StealthTileMap::TileMapF<width, length, layers>&& elevation = StealthNoiseGenerator::generateOctaves<width, length, layers, scaleX, scaleY, erosionScale, numOctaves>
            (std::uniform_real_distribution(config.elevationBounds.x, config.elevationBounds.y));
        // Create water
        StealthTileMap::TileMapF<width, length, layers>&& waterTable = elevation <= config.waterLevel;
        // Create foliage where there's no water and the elevation is appropriate
        StealthTileMap::TileMapF<width, length, layers>&& foliage = StealthNoiseGenerator::generateOctaves<width, length, layers, scaleX, scaleY, foliageGrowthScale, numOctaves>()
            * (!waterTable && (elevation >= config.foliageElevationBounds.x) && (elevation <= config.foliageElevationBounds.y));
        return TerrainMap<width, length, layers>{std::move(elevation), std::move(waterTable), std::move(foliage)};
    }
} /* StealthWorldGenerator */

#endif /* end of include guard: TERRAIN_GENERATOR_H */
