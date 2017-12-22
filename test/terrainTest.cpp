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

const StealthWorldGenerator::DiscreteColorPalette elevationDiscrete{{Color(0, 0, 0), Color(36, 36, 36), Color(72, 72, 72),
    Color(98, 98, 98), Color(134, 134, 134), Color(170, 170, 170), Color(206, 206, 206), Color(255, 255, 255)}};

const StealthWorldGenerator::GradientColorPalette elevationGradient{Color(0, 0, 0), Color(255, 255, 255)};

template <int rows, int cols>
constexpr sf::Image imageFromColorMap(const StealthWorldGenerator::ColorMap<rows, cols>& colors) {
    sf::Image im;
    im.create(rows, cols, (uint8_t*) colors.data());
    return im;
}

template <int rowsAtCompileTime, int colsAtCompileTime, int scale, int numOctaves = 8>
StealthWorldGenerator::TerrainMap<rowsAtCompileTime, colsAtCompileTime> generateTerrain() {
    StealthWorldGenerator::NoiseGenerator noiseGenerator;
    auto&& elevation = noiseGenerator.generateOctaves<rowsAtCompileTime, colsAtCompileTime, scale, numOctaves>();
    return StealthWorldGenerator::TerrainMap{elevation};
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Terrain Test");
    auto terrain = generateTerrain<WINDOW_Y, WINDOW_X, 200>();

    // Show terrain on-screen.
    sf::Texture terrainTexture;
    terrainTexture.loadFromImage(imageFromColorMap(StealthWorldGenerator::applyPalette(elevationDiscrete, terrain.getElevationMap())));
    sf::Sprite terrainSprite;
    terrainSprite.setTexture(terrainTexture);
    // Draw
    window.draw(terrainSprite);
    // Display.
    window.display();

    while (window.isOpen()) {
        // Handle events.
        sf::Event event;
        while (window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
              window.close();
            }
        }
    }
}
