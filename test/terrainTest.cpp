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

        Benchmark& startFrame() {
            start = std::chrono::steady_clock::now();
            return *this;
        }

        Benchmark& endFrame() {
            end = std::chrono::steady_clock::now();
            currentFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            totalTime += currentFrameTime;
            ++numFrames;
            return *this;
        }

        long getCurrentFrameTimeMS() {
            return currentFrameTime;
        }

        void display() const {
            // Clear the line
            std::cout << std::string(100, ' ') << '\r';
            std::cout << "Average FrameTime:  " << (totalTime / (float) numFrames) << " milliseconds" << '\t';
            std::cout << "Average Framerate: " << (numFrames * 1000 / (float) totalTime) << " fps" << '\r' << std::flush;
        }
    private:
        std::chrono::time_point<std::chrono::steady_clock> start, end;
        long currentFrameTime = 0;
        long long totalTime = 0;
        int numFrames = 0;
} benchmark;

using StealthColor::Color, StealthWorldGenerator::TerrainConfig, StealthWorldGenerator::TerrainMap, StealthWorldGenerator::createTerrainConfig,
    StealthWorldGenerator::TerrainMapSpriteManager, StealthColor::DiscreteColorPalette, StealthColor::GradientColorPalette;

const std::unordered_map<sf::Keyboard::Key, int> keyBindings = {
    {sf::Keyboard::E, StealthWorldGenerator::Elevation},
    {sf::Keyboard::T, StealthWorldGenerator::Temperature},
    {sf::Keyboard::W, StealthWorldGenerator::WaterTable},
    {sf::Keyboard::F, StealthWorldGenerator::Foliage}
};

std::array<bool, StealthWorldGenerator::NumTerrainMapTypes> visibleLayers = {};

// Palettes
const DiscreteColorPalette elevationPalette{
    {Color(0x000000FF), Color(0x242424FF), Color(0x484848FF),
    Color(0x6C6C6CFF), Color(0x909090FF), Color(0xB4B4B4FF),
    Color(0xD8D8D8FF), Color(0xFFFFFFFF)}
};
const GradientColorPalette temperaturePalette{Color(0x1530FFA0), Color(0xFF3015A0)};
const GradientColorPalette seaLevelPalette{Color(0x0000FF00), Color(0x0000FF80)};
const GradientColorPalette foliagePalette{Color(0x77DD0000), Color(0x112200FF)};

// Configure the terrain generator
constexpr auto temperateGrasslands = TerrainConfig().setElevationBounds(0.20f, 0.80f).setSeaLevel(0.45f).setFoliageElevationBounds(0.25f, 0.60f);

auto doGenerateTerrainMap() {
    return StealthWorldGenerator::generateTerrainMap<WINDOW_X, WINDOW_Y, NUM_TERRAIN_LAYERS,
        SCALE_X, SCALE_X, EROSION_SCALE, TEMPERATURE_SCALE, FOLIAGE_GROWTH_SCALE, LOD>(temperateGrasslands);
}

template <typename TerrainMapType, typename SpriteManagerType>
constexpr void updateColorMaps(const TerrainMapType& terrainMap, SpriteManagerType& spriteManager) {
    spriteManager.createColorMap(StealthWorldGenerator::Elevation, terrainMap, elevationPalette)
        .createColorMap(StealthWorldGenerator::Temperature, terrainMap, temperaturePalette)
        .createColorMap(StealthWorldGenerator::WaterTable, terrainMap, seaLevelPalette)
        .createColorMap(StealthWorldGenerator::Foliage, terrainMap, foliagePalette);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Terrain Test");
    sf::Clock clock;
    // Generate! Erosion should be much slower (larger scale) than foliage growth
    auto terrainMap = doGenerateTerrainMap();
    // Sprite manager
    TerrainMapSpriteManager spriteManager{terrainMap};
    // Create sprites from this terrainMap.
    updateColorMaps(terrainMap, spriteManager);
    long currentFrameTime = 0;
    while (window.isOpen()) {
        for (int i = 0; i < NUM_TERRAIN_LAYERS; ++i) {
            // Begin benchmark
            benchmark.startFrame();
            auto frameStart = std::chrono::steady_clock::now();
            // Clear
            window.clear(sf::Color(0x808080FF));
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
                    // Enable/disable layers based on key presses.
                    if (keyBindings.count(event.key.code) > 0) {
                        visibleLayers[keyBindings.at(event.key.code)] ^= true;
                    } else if (event.key.code == sf::Keyboard::Right) {
                        terrainMap = doGenerateTerrainMap();
                        updateColorMaps(terrainMap, spriteManager);
                    }
                }
            }
            auto frameEnd = std::chrono::steady_clock::now();
            sleepMS((long) 1000.0f / FRAMERATE - (std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count()));
            benchmark.endFrame();
        }
        // Finish benchmark
        benchmark.display();
    }
}
