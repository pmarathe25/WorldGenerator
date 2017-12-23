#include "TileMap/TileMap.hpp"
#include "Noise/NoiseGenerator.hpp"
#include "Terrain/TerrainMap.hpp"
#include "Terrain/TerrainGenerator.hpp"
#include "Terrain/TerrainMapSpriteManager.hpp"
#include "Color/ColorPalette.hpp"
#include "config.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <chrono>
#include <thread>

using StealthWorldGenerator::Color, StealthWorldGenerator::TerrainGenerator, StealthWorldGenerator::TerrainMapSpriteManager;

const StealthWorldGenerator::DiscreteColorPalette elevationPalette{{Color(0, 0, 0), Color(36, 36, 36), Color(72, 72, 72),
    Color(98, 98, 98), Color(134, 134, 134), Color(170, 170, 170), Color(206, 206, 206), Color(255, 255, 255)}};

const StealthWorldGenerator::GradientColorPalette waterLevelPalette{Color(0x0000FF00), Color(0x0000FF80)};

const StealthWorldGenerator::GradientColorPalette foliagePalette{Color(0x33660000), Color(0x336600FF)};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Terrain Test");
    // Configure the terrain generator
    TerrainGenerator terrainGenerator{};
    terrainGenerator.setElevationBounds(0.0f, 1.0f).setWaterLevel(0.40f).setFoliageElevationBounds(0.45f, 0.65f);
    // Sprite manager
    TerrainMapSpriteManager spriteManager{elevationPalette, waterLevelPalette, foliagePalette};
    while (window.isOpen()) {
        // Generate!
        auto terrain = terrainGenerator.generate<WINDOW_Y, WINDOW_X, 100, 8>();
        // Show terrain on-screen.
        spriteManager.setTerrainMap(terrain);
        // Clear
        window.clear();
        // Draw
        window.draw(spriteManager.getElevationSprite());
        window.draw(spriteManager.getWaterTableSprite());
        window.draw(spriteManager.getFoliageSprite());
        // Display.
        window.display();
        // Handle events.
        sf::Event event;
        while (window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
              window.close();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
