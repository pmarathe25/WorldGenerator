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

    template <typename Tile>
    class TileMap : public TileMapBase<TileMap<Tile>> {
        public:
            typedef Tile TileType;

            TileMap(int rows, int cols) : tiles(rows * cols), mapRows(rows), mapCols(cols), mapSize(rows * cols) { }

            Tile& at(int row, int col) {
                return tiles[row * mapCols + col];
            }

            const Tile& at(int row, int col) const {
                return tiles[row * mapCols + col];
            }

            int rows() const {
                return mapRows;
            }

            int cols() const {
                return mapCols;
            }

            int size() const {
                return mapSize;
            }
        protected:
            std::vector<Tile> tiles;
            int mapRows, mapCols, mapSize;
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
