#include "Noise/NoiseGenerator.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

const int WINDOW_X = 10;
const int WINDOW_Y = 10;

template <int rows, int cols>
sf::Image imageFromNoise(const StealthWorldGenerator::TileMap<float, rows, cols>& noise) {
    sf::Image im;
    im.create(cols, rows);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
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
    auto noise = noiseGenerator.generate<WINDOW_Y, WINDOW_X, 8>();
    auto noise2 = noiseGenerator.generate<WINDOW_Y, WINDOW_X, 2>();

    display(noise);
    display(noise2);
    display(noise + noise2);

    StealthWorldGenerator::TileMapF<WINDOW_Y, WINDOW_X> test = noise + noise2;
    display(test);
    // StealthWorldGenerator::TileMap<float, 10, 10> noise;
    // StealthWorldGenerator::TileMap<float> noise = noiseGenerator.generate(10, 10);
    // display(noise);

    // Show noise on-screen.
    // sf::Texture noiseTexture;
    // noiseTexture.loadFromImage(imageFromNoise(noise));
    // sf::Sprite noiseSprite;
    // noiseSprite.setTexture(noiseTexture);
    // // Draw
    // window.draw(noiseSprite);
    // // Display.
    // window.display();
    // while (window.isOpen()) {
    //     // Handle events.
    //     sf::Event event;
    //     while (window.pollEvent(event)) {
    //         if(event.type == sf::Event::Closed) {
    //           window.close();
    //         }
    //     }
    //     std::this_thread::sleep_for(std::chrono::milliseconds(250));
    // }
}
