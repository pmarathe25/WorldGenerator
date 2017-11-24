#ifndef STEALTH_TILEMAP_H
#define STEALTH_TILEMAP_H
#include <vector>
#include <iostream>

namespace StealthWorldGenerator {
    template <typename Tile, int rowsAtCompileTime, int colsAtCompileTime>
    class TileMap{
        public:
            typedef Tile TileType;

            enum {
                rows = rowsAtCompileTime,
                cols = colsAtCompileTime,
                size = rows * cols
            };

            TileMap() : tiles(rowsAtCompileTime * colsAtCompileTime) { }

            template <int row, int col>
            Tile& at() {
                return tiles[row * colsAtCompileTime + col];
            }

            template <int row, int col>
            const Tile& at() const {
                return tiles[row * colsAtCompileTime + col];
            }
        private:
            std::vector<Tile> tiles;
    };

    void display(int i) {
        std::cout << i << " ";
    }

    template <typename TileMapType, int currentRow = 0, int currentCol = 0>
    void display(const TileMapType& tileMap) {
        // Don't display out of bounds tiles
        if constexpr (currentRow < TileMapType::rows) {
            display(tileMap.template at<currentRow, currentCol>());
            // End of column
            if constexpr (currentCol >= TileMapType::cols) {
                std::cout << '\n';
                display<TileMapType, currentRow + 1, 0>(tileMap);
            } else {
                display<TileMapType, currentRow, currentCol + 1>(tileMap);
            }
        }
    }

} /* StealthWorldGenerator */

#endif
