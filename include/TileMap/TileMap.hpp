#ifndef STEALTH_TILEMAP_H
#define STEALTH_TILEMAP_H
#include "config.hpp"
#include "TileMap/TileMapBase.hpp"
#include "TileMap/Operations/BinaryOperations.hpp"
#include "TileMap/Operations/UnaryOperations.hpp"
#include "Utility.hpp"
#include <vector>
#include <iostream>
#include <thread>

namespace StealthWorldGenerator {
    namespace internal {
        template <typename type, int rowsAtCompileTime, int colsAtCompileTime, int sizeAtCompileTime>
        struct traits<TileMap<type, rowsAtCompileTime, colsAtCompileTime, sizeAtCompileTime>> {
            typedef type ScalarType;
            static constexpr int rows = rowsAtCompileTime, cols = colsAtCompileTime, size = sizeAtCompileTime;
        };
    } /* internal */

    template <typename type, int rowsAtCompileTime, int colsAtCompileTime, int sizeAtCompileTime>
    class TileMap : public TileMapBase<TileMap<type, rowsAtCompileTime, colsAtCompileTime>> {
        public:
            typedef type ScalarType;
            // Dimensions
            static constexpr int rows = rowsAtCompileTime, cols = colsAtCompileTime, size = sizeAtCompileTime;

            constexpr TileMap() : tiles(sizeAtCompileTime) { }

            // Copy
            template <typename OtherDerived>
            constexpr TileMap(const TileMapBase<OtherDerived>& other) : tiles(sizeAtCompileTime) {
                copyMultithreaded(other);
            }

            constexpr TileMap(const TileMap& other)  : tiles(sizeAtCompileTime){
                copyMultithreaded(other);
            }

            // Move
            constexpr TileMap(TileMap&& other) noexcept = default;

            // Assignment
            constexpr void operator=(const TileMap& other) noexcept {
                copyMultithreaded(other);
            }

            template <typename OtherDerived>
            constexpr void operator=(const TileMapBase<OtherDerived>& other) noexcept {
                copyMultithreaded(other);
            }

            constexpr ScalarType& at(int row, int col) {
                return tiles[row * cols + col];
            }

            constexpr const ScalarType& at(int row, int col) const {
                return tiles[row * cols + col];
            }

            constexpr ScalarType& operator[](int index) {
                return tiles[index];
            }

            constexpr const ScalarType& operator[](int index) const {
                return tiles[index];
            }

            constexpr const ScalarType* data() const noexcept {
                return tiles.data();
            }

            constexpr ScalarType* data() noexcept {
                return tiles.data();
            }

            constexpr typename std::vector<ScalarType>::iterator begin() noexcept {
                return tiles.begin();
            }

            constexpr typename std::vector<ScalarType>::const_iterator cbegin() const noexcept {
                return tiles.cbegin();
            }

            constexpr typename std::vector<ScalarType>::iterator end() noexcept {
                return tiles.end();
            }

            constexpr typename std::vector<ScalarType>::const_iterator cend() const noexcept {
                return tiles.cend();
            }
        private:
            std::vector<ScalarType> tiles;
            std::array<std::thread, NUM_THREADS> copyThreads;

            template <typename Derived>
            constexpr void copyPortion(const TileMapBase<Derived>* other, int id) {
                // Copy elements for a single portion of the TileMap.
                constexpr int portionSize = ceilDivide(size, NUM_THREADS);
                const int start = id * portionSize;
                const int end = std::min(start + portionSize, size);
                for (int i = start; i < end; ++i) {
                    tiles[i] = other -> operator[](i);
                }
            }

            template <typename Derived>
            constexpr void copyMultithreaded(const TileMapBase<Derived>& other) {
                // Make sure dimensions are compatible
                static_assert(internal::traits<Derived>::rows == rows && internal::traits<Derived>::cols == cols,
                    "Cannot copy incompatible TileMaps");
                // Create threads
                for (int i = 0; i < NUM_THREADS; ++i) {
                    copyThreads[i] = std::thread{&TileMap::copyPortion<Derived>, this, &other, i};
                }
                // Wait for all threads to finish
                for (auto& thread : copyThreads) {
                    thread.join();
                }
            }
    };

    // Specialization for boolean map
    template <int rowsAtCompileTime, int colsAtCompileTime, int sizeAtCompileTime>
    class TileMap<bool, rowsAtCompileTime, colsAtCompileTime, sizeAtCompileTime>
        : public TileMapBase<TileMap<bool, rowsAtCompileTime, colsAtCompileTime>> {
        public:
            static constexpr int rows = rowsAtCompileTime, cols = colsAtCompileTime, size = sizeAtCompileTime;

            constexpr TileMap() : tiles(sizeAtCompileTime) { }

            // Copy
            template <typename OtherDerived>
            constexpr TileMap(const TileMapBase<OtherDerived>& other) : tiles(sizeAtCompileTime) {
                copyMultithreaded(other);
            }

            constexpr TileMap(const TileMap& other)  : tiles(sizeAtCompileTime){
                copyMultithreaded(other);
            }

            // Move
            constexpr TileMap(TileMap&& other) noexcept = default;

            // Assignment
            constexpr void operator=(const TileMap& other) noexcept {
                copyMultithreaded(other);
            }

            template <typename OtherDerived>
            constexpr void operator=(const TileMapBase<OtherDerived>& other) noexcept {
                copyMultithreaded(other);
            }

            constexpr bool at(int row, int col) const {
                return tiles[row * cols + col];
            }

            constexpr bool operator[](int index) const {
                return tiles[index];
            }

            constexpr typename std::vector<bool>::iterator begin() noexcept {
                return tiles.begin();
            }

            constexpr typename std::vector<bool>::const_iterator cbegin() const noexcept {
                return tiles.cbegin();
            }

            constexpr typename std::vector<bool>::iterator end() noexcept {
                return tiles.end();
            }

            constexpr typename std::vector<bool>::const_iterator cend() const noexcept {
                return tiles.cend();
            }

        private:
            std::vector<bool> tiles;
            std::array<std::thread, NUM_THREADS> copyThreads;

            template <typename Derived>
            constexpr void copyPortion(const TileMapBase<Derived>* other, int id) {
                // Copy elements for a single portion of the TileMap.
                constexpr int portionSize = ceilDivide(size, NUM_THREADS);
                const int start = id * portionSize;
                const int end = std::min(start + portionSize, size);
                for (int i = start; i < end; ++i) {
                    tiles[i] = other -> operator[](i);
                }
            }

            template <typename Derived>
            constexpr void copyMultithreaded(const TileMapBase<Derived>& other) {
                // Make sure dimensions are compatible
                static_assert(internal::traits<Derived>::rows == rows && internal::traits<Derived>::cols == cols,
                    "Cannot copy incompatible TileMaps");
                // Create threads
                for (int i = 0; i < NUM_THREADS; ++i) {
                    copyThreads[i] = std::thread{&TileMap::copyPortion<Derived>, this, &other, i};
                }
                // Wait for all threads to finish
                for (auto& thread : copyThreads) {
                    thread.join();
                }
            }
    };

    template <typename T>
    constexpr std::string to_string(const T& i) {
        return std::to_string(i);
    }

    template <>
    inline std::string to_string(const std::string& tile) {
        return tile;
    }

    template <typename TileMapType>
    constexpr void display(const TileMapType& tileMap, const std::string& title = "") {
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

    template <int rowsAtCompileTime, int colsAtCompileTime>
    using TileMapD = TileMap<double, rowsAtCompileTime, colsAtCompileTime>;
} /* StealthWorldGenerator */

#endif
