#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H
#include "Noise/StealthNoiseGenerator.hpp"
#include "Terrain/TerrainMap.hpp"
#include "Vector2.hpp"
#include <random>

namespace StealthWorldGenerator {
    template <int rows, int cols, int scale, int numOctaves = 8, typename NoiseGen = StealthNoiseGenerator>
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

            constexpr TerrainMap<rows, cols> generate() noexcept {
                NoiseGen noiseGenerator;
                // Create land
                TerrainNoiseMap<rows, cols>&& elevation = noiseGenerator.template generateOctaves<rows, cols, scale, numOctaves>
                    (std::uniform_real_distribution(elevationBounds.x, elevationBounds.y));
                // Create water
                TerrainNoiseMap<rows, cols>&& waterTable = elevation <= waterLevel;
                // Create foliage where there's no water and the elevation is appropriate
                TerrainNoiseMap<rows, cols>&& foliage = noiseGenerator.template generateOctaves<rows, cols, scale, numOctaves>()
                    * !waterTable * ((elevation >= foliageElevationBounds.x) && (elevation <= foliageElevationBounds.y));
                return TerrainMap{elevation, waterTable, foliage};
            }
        private:
            float waterLevel;
            Vector2f elevationBounds = {0.0f, 1.0f}, foliageElevationBounds = {0.0f, 1.0f};
    };
} /* StealthWorldGenerator */

#endif /* end of include guard: TERRAIN_GENERATOR_H */