#include "Terrain/TerrainGenerator.hpp"
#include "Terrain/TerrainMapSpriteManager.hpp"
#include "Color/ColorPalette.hpp"
#include "config.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

class Benchmark {
    public:
        Benchmark() {
            std::cout << '\n';
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
            std::cout << "Average FrameTime:  " << (totalTime / (float) numFrames) << " milliseconds" << '\t';
            std::cout << "Average Framerate: " << (numFrames / (float) totalTime) << " fps" << '\r' << std::endl;
        }
    private:
        std::chrono::time_point<std::chrono::steady_clock> start, end;
        long long totalTime = 0;
        int numFrames = 0;
} benchmark;

inline void sleepMS(long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

using StealthColor::Color, StealthWorldGenerator::TerrainConfig, StealthWorldGenerator::TerrainMap, StealthWorldGenerator::createTerrainConfig,
    StealthWorldGenerator::TerrainMapSpriteManager, StealthColor::DiscreteColorPalette, StealthColor::GradientColorPalette;

const DiscreteColorPalette elevationPalette{{Color(0, 0, 0), Color(36, 36, 36), Color(72, 72, 72),
    Color(98, 98, 98), Color(134, 134, 134), Color(170, 170, 170), Color(206, 206, 206), Color(255, 255, 255)}};

const GradientColorPalette waterLevelPalette{Color(0x0000FF00), Color(0x0000FF80)};

const GradientColorPalette foliagePalette{Color(0x77DD0000), Color(0x112200FF)};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Terrain Test");
    // Configure the terrain generator
    auto temperateGrasslands = createTerrainConfig().setElevationBounds(0.20f, 0.75f).setWaterLevel(0.45f).setFoliageElevationBounds(0.45f, 0.60f);
    // Sprite manager
    TerrainMapSpriteManager<WINDOW_X, WINDOW_Y, NUM_TERRAIN_LAYERS> spriteManager{};
    while (window.isOpen()) {
        // Begin benchmark
        benchmark.startFrame();
        // Generate! Erosion should be much slower (larger scale) than foliage growth
        auto terrainMap = StealthWorldGenerator::generateTerrainMap<WINDOW_X, WINDOW_Y, NUM_TERRAIN_LAYERS,
            SCALE_X, SCALE_X, EROSION_SCALE, FOLIAGE_GROWTH_SCALE, LOD>(temperateGrasslands);
        // Finish benchmark
        benchmark.endFrame();
        benchmark.display();
        // Create sprites from this terrainMap.
        spriteManager.createColorMap<StealthWorldGenerator::Elevation>(terrainMap, elevationPalette);
        spriteManager.createColorMap<StealthWorldGenerator::WaterTable>(terrainMap, waterLevelPalette);
        spriteManager.createColorMap<StealthWorldGenerator::Foliage>(terrainMap, foliagePalette);
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
            // sleepMS((long) 1000.0f / FRAMERATE);
        }
    }
}
