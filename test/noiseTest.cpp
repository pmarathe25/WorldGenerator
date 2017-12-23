#include "TileMap/TileMap.hpp"
#include "Noise/StealthNoiseGenerator.hpp"
#include "Noise/PerlinNoiseGenerator.hpp"
#include "Color/ColorPalette.hpp"
#include "config.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <chrono>
#include <thread>

using StealthWorldGenerator::Color, StealthWorldGenerator::applyPalette;

const StealthWorldGenerator::GradientColorPalette noisePalette{Color(0, 0, 0), Color(255, 255, 255)};

template <int rows, int cols>
constexpr sf::Sprite spriteFromColorMap(const StealthWorldGenerator::ColorMap<rows, cols>& colors, sf::Texture& texture) {
    sf::Image im;
    sf::Sprite sprite;
    im.create(rows, cols, (uint8_t*) colors.data());
    texture.loadFromImage(im);
    sprite.setTexture(texture);
    return sprite;
}

constexpr float doubleUp(float in) {
    return in * 2.0;
}

constexpr float threshold(float in, float threshold) {
    return (in > threshold) ? in : 0.0f;
}

int main() {
    // Window
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Noise Test");
    StealthWorldGenerator::StealthNoiseGenerator noiseGenerator;
    StealthWorldGenerator::PerlinNoiseGenerator perlinNoiseGenerator;
    // auto octave1 = noiseGenerator.generate<WINDOW_Y, WINDOW_X, 400>();
    // auto octave2 = noiseGenerator.generate<WINDOW_Y, WINDOW_X, 200>();
    // auto octave3 = noiseGenerator.generate<WINDOW_Y, WINDOW_X, 100>();
    // auto octave4 = noiseGenerator.generate<WINDOW_Y, WINDOW_X, 50>();
    // auto octave5 = noiseGenerator.generate<WINDOW_Y, WINDOW_X, 25>();

    // StealthWorldGenerator::TileMapF<WINDOW_Y, WINDOW_X> noise = 0.5f * octave1 + 0.25f
    //     * octave2 + 0.125f * octave3 + 0.0625f * octave4 + 0.03125f * octave5;

    unsigned long long int totalTime = 0;
    int numFrames = 0;

    while (window.isOpen()) {
        auto start = std::chrono::steady_clock::now();

        // auto noise = noiseGenerator.generate<WINDOW_Y, WINDOW_X, 80>();
        // auto noise = perlinNoiseGenerator.generate<WINDOW_Y, WINDOW_X, 80>();

        auto noise = noiseGenerator.generateOctaves<WINDOW_Y, WINDOW_X, 400, 8>();
        // auto noise = perlinNoiseGenerator.generateOctaves<WINDOW_Y, WINDOW_X, 400, 8>();

        // noise = (noise < noise2) + (noise > noise2); // Should be all 1s (white)
        // noise = noise && (noise < noise2);
        // noise = noise * (noise < noise2);
        // noise = noise2;
        // noise = -1.0f * noise2 + 1.0f;
        // noise = noise * (noise < (noise2 * 2));
        // noise = noise - noise2;
        // StealthWorldGenerator::TileMapF<WINDOW_Y, WINDOW_X> noiseTest = StealthWorldGenerator::apply(doubleUp, noise);
        // noiseTest = StealthWorldGenerator::apply(std::bind(threshold, std::placeholders::_1, 0.25f), noise);

        auto end = std::chrono::steady_clock::now();
        totalTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Average Time:  " << (totalTime / (float) ++numFrames) << " milliseconds" << '\r';

        // Show noise on-screen.
        sf::Texture noiseTexture;
        sf::Sprite noiseSprite = spriteFromColorMap(applyPalette(noisePalette, noise), noiseTexture);
        // Draw
        window.draw(noiseSprite);
        // Display.
        window.display();
        // Handle events.
        sf::Event event;
        while (window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
              window.close();
            }
        }

        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
