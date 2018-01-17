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

            constexpr TerrainConfig& setSeaLevel(float seaLevel) noexcept {
                this -> seaLevel = seaLevel;
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

            float seaLevel = 0.0f;
            Vector2f elevationBounds = {0.0f, 1.0f}, foliageElevationBounds = {0.0f, 1.0f};
    };

    constexpr TerrainConfig createTerrainConfig() {
        return TerrainConfig{};
    }

    template <int width, int length, int layers, int scaleX, int scaleY, int erosionScale, int temperatureScale, int foliageGrowthScale, int numOctaves = 8>
    constexpr TerrainMap<width, length, layers> generateTerrainMap(const TerrainConfig& config) noexcept {
        // Create land
        StealthTileMap::TileMapF<width, length, layers> elevation;
        StealthNoiseGenerator::generateOctaves<width, length, layers, scaleX, scaleY, erosionScale, numOctaves>
            (elevation, std::uniform_real_distribution(config.elevationBounds.x, config.elevationBounds.y));
        // Temperature
        StealthTileMap::TileMapF<width, length, layers> temperature;
        StealthNoiseGenerator::generateOctaves<width, length, layers, scaleX * 2, scaleY * 2, temperatureScale, numOctaves>
            (temperature);
        // Create water
        StealthTileMap::TileMapF<width, length, layers> waterTable = elevation <= config.seaLevel;
        // Create foliage where there's no water and the elevation is appropriate
        StealthTileMap::TileMapF<width, length, layers> foliage;
        StealthNoiseGenerator::generateOctaves<width, length, layers, scaleX, scaleY, foliageGrowthScale, numOctaves>(foliage);
        foliage *= (!waterTable && (elevation >= config.foliageElevationBounds.x) && (elevation <= config.foliageElevationBounds.y));
        return TerrainMap<width, length, layers>{}
            .set(Elevation, std::move(elevation))
            .set(Temperature, std::move(temperature))
            .set(WaterTable, std::move(waterTable))
            .set(Foliage, std::move(foliage));
    }

    template <int width, int length, int scaleX, int scaleY, int numOctaves = 8>
    constexpr TerrainMap<width, length> generateTerrainMap(const TerrainConfig& config) noexcept {
        return generateTerrainMap<width, length, 1, scaleX, scaleY, 1, 1, 1, numOctaves>(config);
    }
} /* StealthWorldGenerator */

#endif /* end of include guard: TERRAIN_GENERATOR_H */
