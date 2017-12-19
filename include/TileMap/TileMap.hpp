#ifndef STEALTH_TILEMAP_H
#define STEALTH_TILEMAP_H
#include "TileMap/TileMapBase.hpp"
#include "TileMap/BinaryOp.hpp"
#include <vector>
#include <iostream>

namespace StealthWorldGenerator {
    namespace internal {
        template <typename type, int rowsAtCompileTime, int colsAtCompileTime, int sizeAtCompileTime>
        struct traits<TileMap<type, rowsAtCompileTime, colsAtCompileTime, sizeAtCompileTime>> {
            typedef type ScalarType;

            enum {
                rows = rowsAtCompileTime,
                cols = colsAtCompileTime,
                size = sizeAtCompileTime
            };
        };
    } /* internal */


    template <typename type, int rowsAtCompileTime, int colsAtCompileTime, int sizeAtCompileTime>
    class TileMap : public TileMapBase<TileMap<type, rowsAtCompileTime, colsAtCompileTime>> {
        public:
            typedef type ScalarType;

            enum {
                rows = rowsAtCompileTime,
                cols = colsAtCompileTime,
                size = sizeAtCompileTime
            };

            TileMap() : tiles(sizeAtCompileTime) { }

            template <typename OtherDerived>
            TileMap(const TileMapBase<OtherDerived>& other) {
                tiles = std::vector<ScalarType>(sizeAtCompileTime);
                *this = other;
            }

            void operator=(const TileMap& other) {
                copy(other);
            }

            template <typename Derived>
            void operator=(const TileMapBase<Derived>& other) {
                copy(other);
            }

            inline ScalarType& at(int row, int col) {
                return tiles[row * cols + col];
            }

            inline const ScalarType& at(int row, int col) const {
                return tiles[row * cols + col];
            }

            inline ScalarType& operator[](int index) {
                return tiles[index];
            }

            inline const ScalarType& operator[](int index) const {
                return tiles[index];
            }

        protected:
            std::vector<ScalarType> tiles;

            template <typename Derived>
            void copy(const TileMapBase<Derived>& other) {
                for (int i = 0; i < size; ++i) {
                    tiles[i] = other[i];
                }
            }
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
        for (int i = 0; i < TileMapType::rows; ++i) {
            for (int j = 0; j < TileMapType::cols; ++j) {
                std::cout << to_string(tileMap.at(i, j)) << " ";
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    template <int rowsAtCompileTime, int colsAtCompileTime>
    using TileMapF = TileMap<float, rowsAtCompileTime, colsAtCompileTime>;

} /* StealthWorldGenerator */

#endif
