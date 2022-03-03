#include "Terrain/TerrainGenerator.hpp"
#include "Terrain/TerrainMapSpriteManager.hpp"
#include "config.hpp"
#include <Color>
#include <chrono>
#include <thread>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>
#include <string>

using StealthColor::Color, Stealth::World::TerrainMap, Stealth::World::TerrainConfig,
    Stealth::World::TerrainScaleConfig, Stealth::World::TerrainSetting,
    Stealth::World::TerrainMember, Stealth::World::TerrainMapSpriteManager,
    StealthColor::DiscreteColorPalette, StealthColor::GradientColorPalette;

const std::unordered_map<sf::Keyboard::Key, int> keyBindings = {
    {sf::Keyboard::E, TerrainMember::Elevation},
    {sf::Keyboard::T, TerrainMember::Temperature},
    {sf::Keyboard::W, TerrainMember::WaterTable},
    {sf::Keyboard::F, TerrainMember::Foliage},
};

std::array<bool, TerrainMember::SIZE> visibleLayers{
    /*Elevation=*/ true,
    /*WaterTable=*/ true,
    /*Foliage=*/ true,
    /*Temperature=*/ false,
    /*Moisture=*/ false,
};


// Palettes
const DiscreteColorPalette elevationPalette{
    {
        Color(12, 6, 0, 255), 
        Color(24, 18, 12, 255), 
        Color(48, 36, 17, 255), 
        Color(68, 54, 17, 255), 
        Color(92, 72, 44, 255),
        Color(110, 96, 60, 255),
        Color(136, 108, 70, 255), 
        Color(147, 126, 88, 255), 
        Color(165, 144, 105, 255), 
        Color(195, 162, 125, 255), 
        Color(202, 180, 165, 255),
        Color(212, 198, 181, 255),
        Color(220, 216, 200, 255), 
        Color(240, 234, 216, 255), 
        Color(255, 255, 255, 255)
    }
};
const GradientColorPalette seaLevelPalette{
        Color(0, 0, 0, 0), 
        Color(20, 40, 255, 127)
};
const GradientColorPalette foliagePalette{
    Color(64, 255, 0, 0), 
    Color(17, 34, 0, 255)
};
const GradientColorPalette temperaturePalette{
    Color(21, 48, 255, 200), 
    Color(255, 48, 21, 200)
};

// Configure the terrain generator. This can change the overall look of the map
constexpr auto temperateGrasslands = TerrainConfig()
    // Mean/Std-dev of elevations to generate
    .set(TerrainSetting::Elevation, 0.4f, 0.25f)
    // Water table height. Anything below this will be submerged in water.
    .set(TerrainSetting::WaterTable, 0.32f)
    // Elevation range in which foliage can grow 
    .set(TerrainSetting::Foliage, 0.3f, 0.5f)
    // Mean/Std-dev of temperature
    .set(TerrainSetting::Temperature, 0.5f, 0.3f);

template <typename TerrainMapMember, typename SpriteManagerType>
constexpr void updateColorMaps(const TerrainMapMember& terrainMap, SpriteManagerType& spriteManager) {
    spriteManager.createColorMap(TerrainMember::Elevation, terrainMap, elevationPalette)
        .createColorMap(TerrainMember::WaterTable, terrainMap, seaLevelPalette)
        .createColorMap(TerrainMember::Foliage, terrainMap, foliagePalette)
        .createColorMap(TerrainMember::Temperature, terrainMap, temperaturePalette)
    ;
}

int main() {
    long currentSeed = 0;

    // Generate! Erosion should be much slower (larger scale) than foliage growth
    TerrainMap<WINDOW_X, WINDOW_Y, NUM_TERRAIN_LAYERS> terrainMap;
    TerrainScaleConfig<SCALE_X, SCALE_Y, EROSION_SCALE, TEMPERATURE_SCALE, FOLIAGE_GROWTH_SCALE, LOD> terrainScaleConfig;

    Stealth::World::generateTerrainMap(terrainMap, temperateGrasslands, terrainScaleConfig, currentSeed);

    // Sprite manager
    TerrainMapSpriteManager spriteManager{terrainMap};
    // Create sprites from this terrainMap.
    updateColorMaps(terrainMap, spriteManager);
    long currentFrameTime = 0;

    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Terrain Test");
    while (window.isOpen()) {
        decltype(std::chrono::steady_clock::now()) frameStart, frameEnd;
        for (int i = 0; i < NUM_TERRAIN_LAYERS; ++i) {
            frameStart = std::chrono::steady_clock::now();
            // Clear
            window.clear(sf::Color(0x808080FF));
            // Draw
            for (int mapType = 0; mapType < terrainMap.numMembers(); ++mapType) {
                if (visibleLayers[mapType]) window.draw(spriteManager.getSpriteFromLayer(mapType, i));
            }
            // Display.
            window.display();
            // Handle events.
            sf::Event event;
            while (window.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                  window.close();
                } else if (event.type == sf::Event::KeyPressed) {
                    // Enable/disable layers based on key presses.
                    if (keyBindings.count(event.key.code) > 0) {
                        visibleLayers[keyBindings.at(event.key.code)] ^= true;
                    } else if (event.key.code == sf::Keyboard::Right) {
                        ++currentSeed;
                        Stealth::World::generateTerrainMap(terrainMap, temperateGrasslands, terrainScaleConfig, currentSeed);
                        updateColorMaps(terrainMap, spriteManager);
                    } else if (event.key.code == sf::Keyboard::Left) {
                        --currentSeed;
                        Stealth::World::generateTerrainMap(terrainMap, temperateGrasslands, terrainScaleConfig, currentSeed);
                        updateColorMaps(terrainMap, spriteManager);
                    }
                }
            }
            frameEnd = std::chrono::steady_clock::now();
            if constexpr (FRAMERATE > 0)
            {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(
                        (long) 1000.0f / FRAMERATE
                    )
                );
            } 
        }
    }
}
