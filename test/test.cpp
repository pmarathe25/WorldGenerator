#include "TileMap.hpp"
#include "StealthNoise.hpp"

int main() {
    StealthWorldGenerator::TileMap<int, 5, 5> terrainMap{};
    display(terrainMap);
    std::cout << '\n';
    StealthWorldGenerator::StealthNoise<8, 8> noise{};
    display(noise);
}
