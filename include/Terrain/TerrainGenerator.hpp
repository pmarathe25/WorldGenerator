#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H
#include "NoiseGenerator/NoiseGenerator.hpp"
#include "Terrain/TerrainMap.hpp"
#include "Vector2.hpp"
#include <random>

#include <iostream>

namespace StealthWorldGenerator {
    struct TerrainMapSettings {
        enum {
            Elevation = 0,
            WaterTable,
            Foliage,
            Temperature,
            NumTerrainMapSettings
        };
    };

    namespace {
        static std::default_random_engine SeedGenerator{};
    }

    class TerrainConfig : public std::array<Vector2f, TerrainMapSettings::NumTerrainMapSettings> {
        public:
            typedef typename std::array<Vector2f, TerrainMapSettings::NumTerrainMapSettings> super;

            constexpr TerrainConfig() noexcept : super{} {
                // Set default values.
                set(TerrainMapSettings::Temperature, 0.5f, 0.167f);
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
    };

    template <int scaleX, int scaleY, int erosionScale, int temperatureScale, int foliageGrowthScale, int lod>
    struct TerrainScaleConfig { };

    template <int scaleX, int scaleY, int erosionScale, int temperatureScale, int foliageGrowthScale, int numOctaves = 8, int width, int length, int layers>
    constexpr TerrainMap<width, length, layers>& generateTerrainMap(TerrainMap<width, length, layers>& terrainMap,
        const TerrainConfig& config, const TerrainScaleConfig<scaleX, scaleY, erosionScale, temperatureScale,
        foliageGrowthScale, numOctaves>& dim, long seed = 0) noexcept {
        // Generate seeds
        SeedGenerator.seed(seed);
        // Create land
        StealthNoiseGenerator::generateOctaves<width, length, layers, scaleX, scaleY, erosionScale, numOctaves>
            (terrainMap[TerrainMapMembers::Elevation], std::uniform_real_distribution(config[TerrainMapSettings::Elevation].x,
            config[TerrainMapSettings::Elevation].y), SeedGenerator());
        // Temperature
        StealthNoiseGenerator::generateOctaves<width, length, layers, scaleX * 2, scaleY * 2, temperatureScale, numOctaves>
            (terrainMap[TerrainMapMembers::Temperature], std::normal_distribution(config[TerrainMapSettings::Temperature].x,
            config[TerrainMapSettings::Temperature].y), SeedGenerator());
        // Create water
        terrainMap[TerrainMapMembers::WaterTable] = terrainMap[TerrainMapMembers::Elevation] <= config[TerrainMapSettings::WaterTable].x;
        // Create plant life
        StealthNoiseGenerator::generateOctaves<width, length, layers, scaleX, scaleY, foliageGrowthScale, numOctaves>
            (terrainMap[TerrainMapMembers::Foliage], StealthNoiseGenerator::DefaultDistribution, SeedGenerator());
        // Filter out foliage where there's water or the elevation is out of bounds
        terrainMap[TerrainMapMembers::Foliage] *= (!terrainMap[TerrainMapMembers::WaterTable])
            && (terrainMap[TerrainMapMembers::Elevation] >= config[TerrainMapSettings::Foliage].x)
            && (terrainMap[TerrainMapMembers::Elevation] <= config[TerrainMapSettings::Foliage].y);
        // Give it back!
        return terrainMap;
    }

    template <int scaleX, int scaleY, int numOctaves = 8, int width, int length>
    constexpr TerrainMap<width, length>& generateTerrainMap(TerrainMap<width, length>& terrainMap,
        const TerrainConfig& config, const TerrainScaleConfig<scaleX, scaleY, 1, 1, 1, numOctaves>& dim, long seed = 0) noexcept {
        return generateTerrainMap<scaleX, scaleY, 1, 1, 1, numOctaves>(terrainMap, config, dim, seed);
    }
} /* StealthWorldGenerator */

#endif /* end of include guard: TERRAIN_GENERATOR_H */
