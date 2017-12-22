#ifndef COLOR_H
#define COLOR_H
#include <cstdint>

namespace StealthWorldGenerator {
    class Color {
        public:
            // Constructors
            // Color() = default;
            Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha = 255);
            // Data
            uint32_t* data();
            const uint32_t* data() const;
            uint8_t r, g, b, alpha;
    };
} /* StealthWorldGenerator */

#endif
