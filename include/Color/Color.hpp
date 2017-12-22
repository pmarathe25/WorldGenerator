#ifndef COLOR_H
#define COLOR_H
#include <cstdint>

namespace StealthWorldGenerator {
    class Color {
        public:
            // Constructors
            Color() noexcept = default;
            Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha = 255) noexcept;
            // Copy the whole color in a single instruction.
            Color(const Color& other) noexcept;
            // Data
            uint32_t* data() noexcept;
            const uint32_t* data() const noexcept;
            uint8_t r, g, b, alpha;
    };
} /* StealthWorldGenerator */

#endif
