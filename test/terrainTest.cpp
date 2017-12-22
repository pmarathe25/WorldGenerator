#include "TileMap/TileMap.hpp"
#include "Noise/NoiseGenerator.hpp"
#include "Terrain/TerrainMap.hpp"
#include "Color/ColorPalette.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <functional>

const int WINDOW_X = 800;
const int WINDOW_Y = 800;

using StealthWorldGenerator::Color;

const StealthWorldGenerator::DiscreteColorPalette elevationDiscrete{{Color(0, 0, 0), Color(10, 10, 10), Color(20, 20, 20),
    Color(30, 30, 30), Color(40, 40, 40), Color(50, 50, 50), Color(60, 60, 60), Color(255, 255, 255)}};

const StealthWorldGenerator::GradientColorPalette elevationGradient{Color(0, 0, 0), Color(255, 255, 255)};

template <int rows, int cols>
constexpr sf::Image imageFromColorMap(const StealthWorldGenerator::ColorMap<rows, cols>& colors) {
    sf::Image im;
    im.create(rows, cols, (uint8_t*) colors.data());
    return im;
}

int main() {
    // sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Terrain Test");
    auto terrain = StealthWorldGenerator::generateTerrain<WINDOW_Y, WINDOW_X, 200>();

    float min = 1.0f, max, current;
    while (min >= 0.0f && max <= 1.0f) {
        auto terrain = StealthWorldGenerator::generateTerrain<WINDOW_Y, WINDOW_X, 200>();
        current = *std::min_element(terrain.getElevationMap().begin(), terrain.getElevationMap().end());
        min = (current < min) ? current : min;
        current = *std::max_element(terrain.getElevationMap().begin(), terrain.getElevationMap().end());
        max = (current > max) ? current : max;
        std::cout << "Min: " << min << "\tMax: " << max << '\r';
    }

    std::cout << "\nMin: " << min << "\tMax: " << max << '\n';

    // // Show terrain on-screen.
    // sf::Texture terrainTexture;
    // terrainTexture.loadFromImage(imageFromColorMap(StealthWorldGenerator::applyPalette(elevationDiscrete, terrain.getElevationMap())));
    // sf::Sprite terrainSprite;
    // terrainSprite.setTexture(terrainTexture);
    // // Draw
    // window.draw(terrainSprite);
    // // Display.
    // window.display();
    //
    // while (window.isOpen()) {
    //     // Handle events.
    //     sf::Event event;
    //     while (window.pollEvent(event)) {
    //         if(event.type == sf::Event::Closed) {
    //           window.close();
    //         }
    //     }
    // }
}
