#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H
#include "Noise/NoiseGenerator.hpp"
#include "Terrain/TerrainMap.hpp"
#include "Vector2.hpp"
#include <random>

namespace StealthWorldGenerator {
    class TerrainGenerator {
        public:
            constexpr TerrainGenerator() noexcept = default;

            constexpr TerrainGenerator& setWaterLevel(float waterLevel) noexcept {
                this -> waterLevel = waterLevel;
                return *this;
            }

            constexpr TerrainGenerator& setElevationBounds(float lower, float upper) noexcept {
                this -> elevationBounds = {lower, upper};
                return *this;
            }

            constexpr TerrainGenerator& setElevationBounds(Vector2f elevationBounds) noexcept {
                this -> elevationBounds = std::move(elevationBounds);
                return *this;
            }

            constexpr TerrainGenerator& setFoliageElevationBounds(float lower, float upper) noexcept {
                this -> foliageElevationBounds = {lower, upper};
                return *this;
            }

            constexpr TerrainGenerator& setFoliageElevationBounds(Vector2f foliageElevationBounds) noexcept {
                this -> foliageElevationBounds = std::move(foliageElevationBounds);
                return *this;
            }

            template <int width, int length, int scale, int numOctaves = 8>
            constexpr TerrainMap<width, length> generate() const noexcept {
                NoiseGenerator noiseGenerator;
                // Create land
                StealthTileMap::TileMapF<width, length>&& elevation = noiseGenerator.template generateOctaves<width, length, scale, numOctaves>
                    (std::uniform_real_distribution(elevationBounds.x, elevationBounds.y));
                // Create water
                StealthTileMap::TileMapF<width, length>&& waterTable = elevation <= waterLevel;
                // Create foliage where there's no water and the elevation is appropriate
                StealthTileMap::TileMapF<width, length>&& foliage = noiseGenerator.template generateOctaves<width, length, scale, numOctaves>()
                    * !waterTable * ((elevation >= foliageElevationBounds.x) && (elevation <= foliageElevationBounds.y));
                return TerrainMap{std::move(elevation), std::move(waterTable), std::move(foliage)};
            }
        private:
            float waterLevel = 0.0f;
            Vector2f elevationBounds = {0.0f, 1.0f}, foliageElevationBounds = {0.0f, 1.0f};
    };
} /* StealthWorldGenerator */

#endif /* end of include guard: TERRAIN_GENERATOR_H */
