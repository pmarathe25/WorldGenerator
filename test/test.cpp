#include "TileMap.hpp"
#include "Noise/NoiseGenerator.hpp"

int main() {
    StealthWorldGenerator::TileMap<int> terrainMap{5, 5};
    // display(terrainMap);
    std::cout << '\n';
    StealthWorldGenerator::NoiseGenerator noise{4, 4, 2};
    display(noise.generateScaledNoiseMap());
    std::cout << '\n';
    StealthWorldGenerator::InterpolationKernel kernel{4};
    display(kernel);
    std::cout << '\n';
    std::cout << to_string(kernel.getWeights(4, 1)) << '\n';
}
