#ifndef STEALTH_TILEMAP_H
#define STEALTH_TILEMAP_H
#include <vector>
#include <iostream>

namespace StealthWorldGenerator {
    template <typename Derived>
    class TileMapBase {
        public:

        private:
    };

    template <typename Tile, int rowsAtCompileTime, int colsAtCompileTime, int sizeAtCompileTime = rowsAtCompileTime * colsAtCompileTime>
    class TileMap : public TileMapBase<TileMap<Tile, rowsAtCompileTime, colsAtCompileTime>> {
        public:
            typedef Tile TileType;

            enum {
                rows = rowsAtCompileTime,
                cols = colsAtCompileTime,
                size = sizeAtCompileTime
            };

            TileMap() : tiles(sizeAtCompileTime) { }

            Tile& at(int row, int col) {
                return tiles[row * cols + col];
            }

            const Tile& at(int row, int col) const {
                return tiles[row * cols + col];
            }

        protected:
            std::vector<Tile> tiles;
    };

    template <typename T>
    inline std::string to_string(const T& i) {
        return std::to_string(i);
    }

    template <>
    inline std::string to_string(const std::string& tile) {
        return tile;
    }

    template <typename TileMapType>
    void display(const TileMapType& tileMap, const std::string& title = "") {
        std::cout << title << (title != "" ? '\n' : '\0');
        for (int i = 0; i < tileMap.rows(); ++i) {
            for (int j = 0; j < tileMap.cols(); ++j) {
                std::cout << to_string(tileMap.at(i, j)) << " ";
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

} /* StealthWorldGenerator */

#endif
