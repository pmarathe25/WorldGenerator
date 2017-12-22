#ifndef COLOR_H
#define COLOR_H

namespace StealthWorldGenerator {
    class Color {
        public:
            Color() { }

            Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha = 255) : r(r), g(g), b(b), alpha(alpha) { }

            uint32_t* data() {
                return (uint32_t*) (&r);
            }

            const uint32_t* data() const {
                return (const uint32_t*) (&r);
            }

            uint8_t r, g, b, alpha;
    };

    template <int rowsAtCompileTime, int colsAtCompileTime>
    using ColorMap = TileMap<Color, rowsAtCompileTime, colsAtCompileTime>;
} /* StealthWorldGenerator */

#endif
