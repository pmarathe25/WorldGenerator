#include "TileMap/TileMap.hpp"
#include "Noise/StealthNoiseGenerator.hpp"
#include "Noise/PerlinNoiseGenerator.hpp"
#include "Terrain/TerrainMap.hpp"
#include "Terrain/TerrainGenerator.hpp"
#include "Color/ColorPalette.hpp"
#include "config.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <chrono>
#include <thread>

using StealthWorldGenerator::Color, StealthWorldGenerator::applyPalette, StealthWorldGenerator::spriteFromColorMap;

const StealthWorldGenerator::DiscreteColorPalette elevationPalette{{Color(0, 0, 0), Color(36, 36, 36), Color(72, 72, 72),
    Color(98, 98, 98), Color(134, 134, 134), Color(170, 170, 170), Color(206, 206, 206), Color(255, 255, 255)}};

const StealthWorldGenerator::GradientColorPalette waterLevelPalette{Color(0, 0, 255, 0), Color(0, 0, 255, 127)};

const StealthWorldGenerator::GradientColorPalette foliagePalette{Color(0, 255, 0, 0), Color(0, 255, 0, 255)};

// int testTerrainValueRange() {
//     // Try to find the minimum and maximum possible outputs from terrain generation.
//     float min = 1.0f, max, current;
//     while (min >= 0.0f && max <= 1.0f) {
//         auto terrain = generateTerrain<WINDOW_Y, WINDOW_X, 800>();
//         current = *std::min_element(terrain.getElevationMap().cbegin(), terrain.getElevationMap().cend());
//         min = (current < min) ? current : min;
//         current = *std::max_element(terrain.getElevationMap().cbegin(), terrain.getElevationMap().cend());
//         max = (current > max) ? current : max;
//         std::cout << "Min: " << min << "\tMax: " << max << '\r';
//     }
//     std::cout << "\nMin: " << min << "\tMax: " << max << '\n';
//     return 0;
// }

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Terrain Test");
    // Configure the terrain generator
    StealthWorldGenerator::TerrainGenerator<WINDOW_Y, WINDOW_X, 100> terrainGenerator{};
    terrainGenerator.setElevationBounds(0.0f, 1.0f).setWaterLevel(0.40f).setFoliageElevationBounds(0.45f, 0.65f);
    while (window.isOpen()) {
        // Generate!
        auto terrain = terrainGenerator.generate();
        // Show terrain on-screen.
        sf::Texture elevationTexture, waterTableTexture, foliageTexture;
        sf::Sprite elevationSprite = spriteFromColorMap(applyPalette(elevationPalette, terrain.getElevationMap()), elevationTexture);
        sf::Sprite waterTableSprite = spriteFromColorMap(applyPalette(waterLevelPalette, terrain.getWaterTable()), waterTableTexture);
        sf::Sprite foliageSprite = spriteFromColorMap(applyPalette(foliagePalette, terrain.getFoliageMap()), foliageTexture);
        // Draw
        window.draw(elevationSprite);
        window.draw(waterTableSprite);
        window.draw(foliageSprite);
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
