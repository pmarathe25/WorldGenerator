#include "Terrain/TerrainGenerator.hpp"
#include "Terrain/TerrainMapSpriteManager.hpp"
#include "Color/ColorPalette.hpp"
#include "config.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

inline void sleepMS(long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

using StealthColor::Color, StealthWorldGenerator::TerrainConfig, StealthWorldGenerator::TerrainMap,
    StealthWorldGenerator::TerrainMapSpriteManager, StealthColor::DiscreteColorPalette, StealthColor::GradientColorPalette;

const DiscreteColorPalette elevationPalette{{Color(0, 0, 0), Color(36, 36, 36), Color(72, 72, 72),
    Color(98, 98, 98), Color(134, 134, 134), Color(170, 170, 170), Color(206, 206, 206), Color(255, 255, 255)}};

const GradientColorPalette waterLevelPalette{Color(0x0000FF00), Color(0x0000FF80)};

const GradientColorPalette foliagePalette{Color(0x33660000), Color(0x336600FF)};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Terrain Test");
    // Configure the terrain generator
    TerrainConfig temperateGrasslands = TerrainConfig::create().setElevationBounds(0.20f, 0.75f).setWaterLevel(0.45f).setFoliageElevationBounds(0.45f, 0.60f);
    // Sprite manager
    TerrainMapSpriteManager<WINDOW_X, WINDOW_Y, NUM_TERRAIN_LAYERS> spriteManager{};
    while (window.isOpen()) {
        // Generate!
        auto terrain = StealthWorldGenerator::generateTerrain<WINDOW_X, WINDOW_Y, NUM_TERRAIN_LAYERS, 100, 100, 400>(temperateGrasslands);
        // Create sprites from this terrain.
        spriteManager.createColorMap<StealthWorldGenerator::Elevation>(terrain, elevationPalette);
        spriteManager.createColorMap<StealthWorldGenerator::WaterTable>(terrain, waterLevelPalette);
        spriteManager.createColorMap<StealthWorldGenerator::Foliage>(terrain, foliagePalette);
        for (int i = 0; i < NUM_TERRAIN_LAYERS; ++i) {
            // Clear
            window.clear();
            // Draw
            window.draw(spriteManager.getSpriteFromLayer<StealthWorldGenerator::Elevation>(i));
            window.draw(spriteManager.getSpriteFromLayer<StealthWorldGenerator::WaterTable>(i));
            window.draw(spriteManager.getSpriteFromLayer<StealthWorldGenerator::Foliage>(i));
            // Display.
            window.display();
            // Handle events.
            sf::Event event;
            while (window.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                  window.close();
                }
            }
            sleepMS((long) 1000.0f / FRAMERATE);
        }
    }
}
