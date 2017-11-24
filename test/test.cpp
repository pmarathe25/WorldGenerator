#include "TileMap.hpp"

int main() {
    StealthWorldGenerator::TileMap<int, 5, 5> terrainMap{};
    display(terrainMap);
}
