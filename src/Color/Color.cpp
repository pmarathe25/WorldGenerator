#include "Color/Color.hpp"

namespace StealthWorldGenerator {
    Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha) noexcept
        : r(r), g(g), b(b), alpha(alpha) { }

    Color::Color(const Color& other) noexcept {
        // We can copy the whole Color in a single instruction
        *(this -> data()) = *(other.data());
    }

    uint32_t* Color::data() noexcept {
        return (uint32_t*) (&r);
    }

    const uint32_t* Color::data() const noexcept {
        return (const uint32_t*) (&r);
    }
} /* StealthWorldGenerator */
