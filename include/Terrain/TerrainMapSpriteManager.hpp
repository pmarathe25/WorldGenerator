#ifndef TERRAIN_MAP_SPRITE_MANAGER_H
#define TERRAIN_MAP_SPRITE_MANAGER_H
#include "TileMap/TileMap.hpp"
#include "Terrain/TerrainMap.hpp"
#include "Color/ColorPalette.hpp"
#include <SFML/Graphics.hpp>

namespace StealthWorldGenerator {
    using StealthColor::Color, StealthColor::GradientColorPalette;

    namespace {
        template <typename TileMapType>
        constexpr sf::Sprite spriteFromColorMap(const TileMapType& colors, sf::Texture& texture) {
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

            constexpr TerrainMapSpriteManager(const StealthTileMap::TileMapF<width, length, numLayers>& tileMap) { }

            template <typename Palette>
            constexpr TerrainMapSpriteManager& createColorMap(int index, const TerrainMap<width, length, numLayers>& terrainMap, const Palette& palette) {
                colorMaps[index] = applyPalette(palette, terrainMap[index]);
                return *this;
            }

            template <typename Palette>
            constexpr TerrainMapSpriteManager& createColorMap(int index, const StealthTileMap::TileMapF<width, length, numLayers>& tileMap, const Palette& palette) {
                colorMaps[index] = applyPalette(palette, tileMap);
                return *this;
            }

            constexpr sf::Sprite getSpriteFromLayer(int index, int layer = 0) noexcept {
                return spriteFromColorMap(StealthTileMap::layer(colorMaps[index], layer), textures[index]);
            }
        private:
            // Textures
            std::array<sf::Texture, NumTerrainMapMembers> textures;
            // Color Maps
            std::array<typename StealthTileMap::TileMap<Color, width, length, numLayers>, NumTerrainMapMembers> colorMaps;
    };
} /* StealthWorldGenerator */

#endif /* end of include guard: TERRAIN_MAP_SPRITE_MANAGER_H */
