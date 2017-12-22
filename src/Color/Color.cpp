#include "Color/Color.hpp"

namespace StealthWorldGenerator {
    Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha) : r(r), g(g), b(b), alpha(alpha) { }

    uint32_t* Color::data() {
        return (uint32_t*) (&r);
    }

    const uint32_t* Color::data() const {
        return (const uint32_t*) (&r);
    }
} /* StealthWorldGenerator */
