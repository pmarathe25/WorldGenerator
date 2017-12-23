#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H
#include "Terrain/TerrainConfig.hpp"
#include "Terrain/TerrainMap.hpp"
#include <random>

namespace StealthWorldGenerator {
    class TerrainGenerator {
        public:
            TerrainGenerator() noexcept = default;

            template <int rows, int cols, int scale, int numOctaves, typename NoiseGen>
            TerrainMap<rows, cols> generate(const TerrainConfig<rows, cols, scale, numOctaves, NoiseGen>& config) {
                NoiseGen noiseGenerator;
                // Create land
                TerrainNoiseMap<rows, cols>&& elevation = noiseGenerator.template generateOctaves<rows, cols, scale, numOctaves>
                    (std::uniform_real_distribution(config.elevationBounds.x, config.elevationBounds.y));
                // Create water
                TerrainNoiseMap<rows, cols>&& waterTable = elevation <= config.waterLevel;
                // Create foliage where there's no water and the elevation is appropriate
                TerrainNoiseMap<rows, cols>&& foliage = noiseGenerator.template generateOctaves<rows, cols, scale, numOctaves>()
                    * !waterTable * ((elevation >= config.foliageElevationBounds.x) && (elevation <= config.foliageElevationBounds.y));
                return TerrainMap{elevation, waterTable, foliage};
            }
    };
} /* StealthWorldGenerator */

#endif /* end of include guard: TERRAIN_GENERATOR_H */
