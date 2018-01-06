#include "Terrain/TerrainGenerator.hpp"
#include "Terrain/TerrainMapSpriteManager.hpp"
#include "Color/ColorPalette.hpp"
#include "config.hpp"
#include <stealthutil>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>

class Benchmark {
    public:
        Benchmark() = default;

        ~Benchmark() {
            std::cout << std::endl;
        }

        void startFrame() {
            start = std::chrono::steady_clock::now();
        }

        void endFrame() {
            end = std::chrono::steady_clock::now();
            totalTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            ++numFrames;
        }

        void display() const {
            // Clear the line
            std::cout << std::string(100, ' ') << '\r';
            std::cout << "Average FrameTime:  " << (totalTime / (float) numFrames) << " milliseconds" << '\t';
            std::cout << "Average Framerate: " << (numFrames * 1000 / (float) totalTime) << " fps" << '\r' << std::flush;
        }
    private:
        std::chrono::time_point<std::chrono::steady_clock> start, end;
        long long totalTime = 0;
        int numFrames = 0;
} benchmark;

using StealthColor::Color, StealthWorldGenerator::TerrainConfig, StealthWorldGenerator::TerrainMap, StealthWorldGenerator::createTerrainConfig,
    StealthWorldGenerator::TerrainMapSpriteManager, StealthColor::DiscreteColorPalette, StealthColor::GradientColorPalette;

const std::unordered_map<sf::Keyboard::Key, int> keyBindings = {
    {sf::Keyboard::E, StealthWorldGenerator::Elevation},
    {sf::Keyboard::W, StealthWorldGenerator::WaterTable},
    {sf::Keyboard::F, StealthWorldGenerator::Foliage}
};

std::array<bool, StealthWorldGenerator::NumTerrainMapTypes> visibleLayers = {};

// Palettes
const DiscreteColorPalette elevationPalette{{Color(0, 0, 0), Color(36, 36, 36), Color(72, 72, 72),
    Color(98, 98, 98), Color(134, 134, 134), Color(170, 170, 170), Color(206, 206, 206), Color(255, 255, 255)}};
const GradientColorPalette waterLevelPalette{Color(0x0000FF00), Color(0x0000FF80)};
const GradientColorPalette foliagePalette{Color(0x77DD0000), Color(0x112200FF)};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Terrain Test");
    sf::Clock clock;
    // Configure the terrain generator
    auto temperateGrasslands = createTerrainConfig().setElevationBounds(0.20f, 0.75f).setWaterLevel(0.45f).setFoliageElevationBounds(0.45f, 0.60f);
    // Sprite manager
    TerrainMapSpriteManager<WINDOW_X, WINDOW_Y, NUM_TERRAIN_LAYERS> spriteManager{};
    // Generate! Erosion should be much slower (larger scale) than foliage growth
    auto terrainMap = StealthWorldGenerator::generateTerrainMap<WINDOW_X, WINDOW_Y, NUM_TERRAIN_LAYERS,
    SCALE_X, SCALE_X, EROSION_SCALE, FOLIAGE_GROWTH_SCALE, LOD>(temperateGrasslands);
    while (window.isOpen()) {
        auto currentFrameTime = clock.restart();
        // Begin benchmark
        benchmark.startFrame();
        // Create sprites from this terrainMap.
        spriteManager.createColorMap<StealthWorldGenerator::Elevation>(terrainMap, elevationPalette);
        spriteManager.createColorMap<StealthWorldGenerator::WaterTable>(terrainMap, waterLevelPalette);
        spriteManager.createColorMap<StealthWorldGenerator::Foliage>(terrainMap, foliagePalette);
        // Finish benchmark
        benchmark.endFrame();
        benchmark.display();
        for (int i = 0; i < NUM_TERRAIN_LAYERS; ++i) {
            // Clear
            window.clear();
            // Draw
            for (int mapType = 0; mapType < StealthWorldGenerator::NumTerrainMapTypes; ++mapType) {
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
                    if (keyBindings.count(event.key.code) > 0) {
                        visibleLayers[keyBindings.at(event.key.code)] ^= true;
                    }
                }
            }
            sleepMS((long) 1000.0f / FRAMERATE - currentFrameTime.asMilliseconds());
        }
    }
}
