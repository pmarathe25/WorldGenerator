#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H
#include "NoiseGenerator/NoiseGenerator.hpp"
#include "Terrain/TerrainMap.hpp"
#include "Vector2.hpp"
#include <random>

namespace StealthWorldGenerator {
    class TerrainConfig : public std::array<Vector2f, NumTerrainMapMembers> {
        public:
            typedef typename std::array<Vector2f, NumTerrainMapMembers> super;

            TerrainConfig(long sd = StealthNoiseGenerator::getCurrentTime()) noexcept : sd{sd}, super{} {
                // Set default values.
                set(Temperature, 0.5f, 0.167f);
            }

            constexpr TerrainConfig& set(int mapNum, float lower = 0.0f, float upper = 1.0f) noexcept {
                this -> operator[](mapNum) = {lower, upper};
                return *this;
            }

            // Vector2f setters
            constexpr TerrainConfig& set(int mapNum, Vector2f input = {0.0f, 1.0f}) noexcept {
                this -> operator[](mapNum) = std::move(input);
                return *this;
            }

            constexpr void seed(long sd) {
                this -> sd = sd;
            }

            constexpr const long& seed() const {
                return sd;
            }

            constexpr long& seed() {
                return sd;
            }
        private:
            long sd;
    };

    template <int scaleX, int scaleY, int erosionScale, int temperatureScale, int foliageGrowthScale, int numOctaves = 8, int width, int length, int layers>
    constexpr TerrainMap<width, length, layers>& generateTerrainMap(TerrainMap<width, length, layers>& terrainMap, const TerrainConfig& config) noexcept {
        // Create land
        StealthNoiseGenerator::generateOctaves<width, length, layers, scaleX, scaleY, erosionScale, numOctaves>
            (terrainMap[Elevation], std::uniform_real_distribution(config[Elevation].x, config[Elevation].y));
        // Temperature
        StealthNoiseGenerator::generateOctaves<width, length, layers, scaleX * 2, scaleY * 2, temperatureScale, numOctaves>
            (terrainMap[Temperature], std::normal_distribution(config[Temperature].x, config[Temperature].y));
        // Create water
        terrainMap[WaterTable] = terrainMap[Elevation] <= config[WaterTable].x;
        // Create foliage where there's no water and the elevation is appropriate
        terrainMap[Foliage] = StealthNoiseGenerator::generateOctaves<width, length, layers, scaleX, scaleY,
            foliageGrowthScale, numOctaves>(terrainMap[Foliage]) *= (!terrainMap[WaterTable] 
            && (terrainMap[Elevation] >= config[Foliage].x) && (terrainMap[Elevation] <= config[Foliage].y));
        // Give it back!
        return terrainMap;
    }

    template <int scaleX, int scaleY, int numOctaves = 8, int width, int length>
    constexpr TerrainMap<width, length>& generateTerrainMap(TerrainMap<width, length>& terrainMap, const TerrainConfig& config) noexcept {
        return generateTerrainMap<scaleX, scaleY, 1, 1, 1, numOctaves>(config);
    }
} /* StealthWorldGenerator */

#endif /* end of include guard: TERRAIN_GENERATOR_H */
