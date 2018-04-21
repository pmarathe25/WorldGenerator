#pragma once
#include <Stealth/NoiseGenerator>
#include "Terrain/TerrainMap.hpp"
#include <random>

namespace Stealth::World {
    struct TerrainSetting {
        enum {
            Elevation = 0,
            WaterTable,
            Foliage,
            Temperature,
            TerrainSettingSize
        };
    };

    class TerrainConfig : public std::array<Tensor::VectorF<2>, TerrainSetting::TerrainSettingSize> {
        public:
            using super = std::array<Tensor::VectorF<2>, TerrainSetting::TerrainSettingSize>;

            constexpr TerrainConfig() noexcept : super{} { }

            constexpr TerrainConfig& set(int mapNum, float lower = 0.0f, float upper = 1.0f) noexcept {
                (*this)[mapNum] = Tensor::VectorF<2>({lower, upper});
                return *this;
            }

            // Tensor::VectorF<2> setter
            constexpr TerrainConfig& set(int mapNum, Tensor::VectorF<2> input = {0.0f, 1.0f}) noexcept {
                (*this)[mapNum] = std::move(input);
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
        std::mt19937 SeedGenerator{seed};
        // Create land
        Noise::generateOctaves<width, length, layers, scaleX, scaleY, erosionScale, numOctaves>
            (terrainMap[TerrainMember::Elevation], std::uniform_real_distribution{config[TerrainSetting::Elevation](0),
            config[TerrainSetting::Elevation](1)}, SeedGenerator());
        // Temperature
        Noise::generateOctaves<width, length, layers, scaleX * 2, scaleY * 2, temperatureScale, numOctaves>
            (terrainMap[TerrainMember::Temperature], std::normal_distribution(config[TerrainSetting::Temperature](0),
            config[TerrainSetting::Temperature](1)), SeedGenerator());
        // Create water
        terrainMap[TerrainMember::WaterTable] = terrainMap[TerrainMember::Elevation] <= config[TerrainSetting::WaterTable](0);
        // Create plant life
        Noise::generateOctaves<width, length, layers, scaleX, scaleY, foliageGrowthScale, numOctaves>
            (terrainMap[TerrainMember::Foliage], Noise::DefaultDistribution{0.f, 1.f}, SeedGenerator());
        // Filter out foliage where there's water or the elevation is out of bounds
        auto foliageMask = (!terrainMap[TerrainMember::WaterTable])
            && (terrainMap[TerrainMember::Elevation] >= config[TerrainSetting::Foliage](0))
            && (terrainMap[TerrainMember::Elevation] <= config[TerrainSetting::Foliage](1));
        terrainMap[TerrainMember::Foliage] = Tensor::hadamard(terrainMap[TerrainMember::Foliage], foliageMask);
        // Give it back!
        return terrainMap;
    }

    // Overloaded function for 2D.
    template <int scaleX, int scaleY, int numOctaves = 8, int width, int length>
    constexpr TerrainMap<width, length>& generateTerrainMap(TerrainMap<width, length>& terrainMap,
        const TerrainConfig& config, const TerrainScaleConfig<scaleX, scaleY, 1, 1, 1, numOctaves>& dim, long seed = 0) noexcept {
        return generateTerrainMap<scaleX, scaleY, 1, 1, 1, numOctaves>(terrainMap, config, dim, seed);
    }
} /* Stealth::World */
