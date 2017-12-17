#include "TileMap.hpp"
#include "Noise/NoiseGenerator.hpp"

int main() {
    StealthWorldGenerator::TileMap<int> terrainMap{5, 5};
    // display(terrainMap);
    std::cout << '\n';
    StealthWorldGenerator::NoiseGenerator<4> noise{4, 4};
    // display(noise);
    std::cout << '\n';
    StealthWorldGenerator::InterpolationKernel<2> kernel;
    // display(kernel);
    display(noise);
    std::cout << '\n';
    // std::cout << to_string(kernel.getDistanceAt(4, 1)) << '\n';
}
