#ifndef STEALTH_TILEMAP_H
#define STEALTH_TILEMAP_H
#include <vector>
#include <iostream>

namespace StealthWorldGenerator {
    template <typename Tile, int rowsAtCompileTime, int colsAtCompileTime>
    class TileMap {
        public:
            typedef Tile TileType;

            enum {
                rows = rowsAtCompileTime,
                cols = colsAtCompileTime,
                size = rows * cols
            };

            TileMap() : tiles(rowsAtCompileTime * colsAtCompileTime, Tile()) { }

            Tile& at(int row, int col) {
                return tiles[row * colsAtCompileTime + col];
            }

            const Tile& at(int row, int col) const {
                return tiles[row * colsAtCompileTime + col];
            }
        private:
            std::vector<Tile> tiles;
    };

    template <typename T>
    inline void displayTile(T i) {
        std::cout << i << " ";
    }

    template <typename TileMapType>
    void display(const TileMapType& tileMap) {
        for (int i = 0; i < TileMapType::rows; ++i) {
            for (int j = 0; j < TileMapType::cols; ++j) {
                displayTile(tileMap.at(i, j));
            }
            std::cout << '\n';
        }
    }

} /* StealthWorldGenerator */

#endif
