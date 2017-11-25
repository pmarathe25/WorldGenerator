#include "TileMap.hpp"
#include "Noise/NoiseGenerator.hpp"

int main() {
    StealthWorldGenerator::TileMap<int> terrainMap{5, 5};
    display(terrainMap);
    std::cout << '\n';
    // StealthWorldGenerator::StealthNoiseGenerator<8, 8> noise{};
    // display(noise);
    StealthWorldGenerator::InterpolationKernel kernel{3};
    display(kernel);
}
