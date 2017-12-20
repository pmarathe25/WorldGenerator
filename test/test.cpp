#include "TileMap/TileMap.hpp"
#include "Noise/NoiseGenerator.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

const int WINDOW_X = 800;
const int WINDOW_Y = 800;

template <int rows, int cols>
constexpr sf::Image imageFromNoise(const StealthWorldGenerator::TileMap<float, rows, cols>& noise) {
    sf::Image im;
    im.create(cols, rows);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Scale from (-1, 1) to (0, 1)
            float color = (noise.at(i, j) + 1.0f) * 127.5f;
            im.setPixel(j, i, sf::Color(color, color, color));
        }
    }
    return im;
}

int main() {
    // Window
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Noise Test");
    StealthWorldGenerator::NoiseGenerator noiseGenerator;
    auto octave1 = noiseGenerator.generate<WINDOW_Y, WINDOW_X, 400>();
    auto octave2 = noiseGenerator.generate<WINDOW_Y, WINDOW_X, 200>();
    auto octave3 = noiseGenerator.generate<WINDOW_Y, WINDOW_X, 100>();
    auto octave4 = noiseGenerator.generate<WINDOW_Y, WINDOW_X, 50>();
    auto octave5 = noiseGenerator.generate<WINDOW_Y, WINDOW_X, 25>();

    StealthWorldGenerator::TileMapF<WINDOW_Y, WINDOW_X> noise = 0.5f * octave1 + 0.25f
        * octave2 + 0.125f * octave3 + 0.0625f * octave4 + 0.03125f * octave5;

    // Show noise on-screen.
    sf::Texture noiseTexture;
    noiseTexture.loadFromImage(imageFromNoise(noise));
    sf::Sprite noiseSprite;
    noiseSprite.setTexture(noiseTexture);
    // Draw
    window.draw(noiseSprite);
    // Display.
    window.display();
    while (window.isOpen()) {
        // Handle events.
        sf::Event event;
        while (window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
              window.close();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}
