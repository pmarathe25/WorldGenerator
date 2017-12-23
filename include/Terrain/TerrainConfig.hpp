#ifndef TERRAIN_CONFIG_H
#define TERRAIN_CONFIG_H
#include "Noise/StealthNoiseGenerator.hpp"
#include "Vector2.hpp"

namespace StealthWorldGenerator {
    class TerrainGenerator;

    template <int rows, int cols, int scale, int numOctaves = 8, typename NoiseGen = StealthNoiseGenerator>
    class TerrainConfig {
        friend class TerrainGenerator;

        public:
            TerrainConfig() noexcept = default;

            TerrainConfig& setWaterLevel(float waterLevel) noexcept {
                this -> waterLevel = waterLevel;
                return *this;
            }

            TerrainConfig& setElevationBounds(float lower, float upper) noexcept {
                this -> elevationBounds = {lower, upper};
                return *this;
            }

            TerrainConfig& setElevationBounds(Vector2f elevationBounds) noexcept {
                this -> elevationBounds = std::move(elevationBounds);
                return *this;
            }

            TerrainConfig& setFoliageElevationBounds(float lower, float upper) noexcept {
                this -> foliageElevationBounds = {lower, upper};
                return *this;
            }

            TerrainConfig& setFoliageElevationBounds(Vector2f foliageElevationBounds) noexcept {
                this -> foliageElevationBounds = std::move(foliageElevationBounds);
                return *this;
            }
        private:
            float waterLevel;
            Vector2f elevationBounds = {0.0f, 1.0f}, foliageElevationBounds = {0.0f, 1.0f};
    };
} /* StealthWorldGenerator */

#endif /* end of include guard: TERRAIN_CONFIG_H */
