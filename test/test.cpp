#include "TileMap.hpp"
#include "Noise/NoiseGenerator.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

const int WINDOW_X = 1600;
const int WINDOW_Y = 900;

sf::Image imageFromNoise(const StealthWorldGenerator::TileMap<float>& noise) {
    sf::Image im;
    im.create(noise.cols(), noise.rows());
    for (int i = 0; i < noise.rows(); ++i) {
        for (int j = 0; j < noise.cols(); ++j) {
            float color = noise.at(i, j) * 255.0f;
            im.setPixel(j, i, sf::Color(color, color, color));
        }
    }
    return im;
}

int main() {
    // Window
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Noise Test");
    StealthWorldGenerator::NoiseGenerator noiseGenerator;
    StealthWorldGenerator::TileMap<float> noise = noiseGenerator.generate(WINDOW_Y, WINDOW_X, 200);
    // StealthWorldGenerator::TileMap<float> noise = noiseGenerator.generate(10, 10);
    // display(noise);

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
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
}
