#ifndef TERRAIN_MAP_SPRITE_MANAGER_H
#define TERRAIN_MAP_SPRITE_MANAGER_H
#include "Terrain/TerrainMap.hpp"
#include "Color/ColorPalette.hpp"
#include <SFML/Graphics.hpp>

namespace StealthWorldGenerator {
    namespace {
        template <int rows, int cols>
        constexpr sf::Sprite spriteFromColorMap(const TileMap<Color, rows, cols>& colors, sf::Texture& texture) {
            sf::Image im;
            sf::Sprite sprite;
            im.create(rows, cols, (uint8_t*) colors.data());
            texture.loadFromImage(im);
            sprite.setTexture(texture);
            return sprite;
        }
    }

    template <typename ElevationPalette, typename WaterLevelPalette, typename FoliagePalette>
    class TerrainMapSpriteManager {
        public:
            TerrainMapSpriteManager(ElevationPalette elevationPalette, WaterLevelPalette waterLevelPalette,
                FoliagePalette foliagePalette) : elevationPalette{std::move(elevationPalette)},
                waterLevelPalette{std::move(waterLevelPalette)}, foliagePalette{std::move(foliagePalette)} { }

            template <int rows, int cols>
            constexpr TerrainMapSpriteManager& setTerrainMap(const TerrainMap<rows, cols>& terrainMap) {
                elevationSprite = spriteFromColorMap(applyPalette(elevationPalette, terrainMap.getElevationMap()), elevationTexture);
                waterTableSprite = spriteFromColorMap(applyPalette(waterLevelPalette, terrainMap.getWaterTable()), waterTableTexture);
                foliageSprite = spriteFromColorMap(applyPalette(foliagePalette, terrainMap.getFoliageMap()), foliageTexture);
                return *this;
            }

            constexpr const sf::Sprite getElevationSprite() const noexcept {
                return elevationSprite;
            }

            constexpr const sf::Sprite getWaterTableSprite() const noexcept {
                return waterTableSprite;
            }

            constexpr const sf::Sprite getFoliageSprite() const noexcept {
                return foliageSprite;
            }
        private:
            // Textures
            sf::Texture elevationTexture, waterTableTexture, foliageTexture;
            // Sprites
            sf::Sprite elevationSprite, waterTableSprite, foliageSprite;
            // Color palettes
            ElevationPalette elevationPalette;
            WaterLevelPalette waterLevelPalette;
            FoliagePalette foliagePalette;
    };
} /* StealthWorldGenerator */

#endif /* end of include guard: TERRAIN_MAP_SPRITE_MANAGER_H */
