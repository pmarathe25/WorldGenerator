#pragma once
#include <Tensor3>
#include <Color>
#include "Terrain/TerrainMap.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace Stealth::World {
    using StealthColor::Color, StealthColor::GradientColorPalette;

    namespace {
        template <typename TileMapType>
        sf::Sprite spriteFromColorMap(const TileMapType& colors, sf::Texture& texture) {
            sf::Image im;
            sf::Sprite sprite;
            im.create(colors.width(), colors.length(), (uint8_t*) colors.data());
            texture.loadFromImage(im);
            sprite.setTexture(texture);
            return sprite;
        }
    }

    template <int width, int length, int numLayers = 1>
    class TerrainMapSpriteManager {
        public:
            constexpr TerrainMapSpriteManager() = default;

            constexpr TerrainMapSpriteManager(const TerrainMap<width, length, numLayers>& terrainMap) { }

            constexpr TerrainMapSpriteManager(const Tensor::Tensor3F<width, length, numLayers>& tileMap) { }

            template <typename Palette>
            constexpr TerrainMapSpriteManager& createColorMap(int index, const TerrainMap<width, length, numLayers>& terrainMap, const Palette& palette) {
                colorMaps[index] = applyPalette(palette, terrainMap[index]);
                return *this;
            }

            template <typename Palette>
            constexpr TerrainMapSpriteManager& createColorMap(int index, const Tensor::Tensor3F<width, length, numLayers>& tileMap, const Palette& palette) {
                colorMaps[index] = applyPalette(palette, tileMap);
                return *this;
            }

            sf::Sprite getSpriteFromLayer(int index, int layer = 0) noexcept {
                return spriteFromColorMap(Tensor::layer(colorMaps[index], layer), textures[index]);
            }
        private:
            // Textures
            std::unordered_map<int, sf::Texture> textures;
            // Color Maps
            std::unordered_map<int, typename Tensor::Tensor3<Color, width, length, numLayers>> colorMaps;
    };
} /* Stealth::World */
