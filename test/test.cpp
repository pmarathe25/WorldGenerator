#include "TileMap.hpp"
#include "Noise/NoiseGenerator.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

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
    // StealthWorldGenerator::TileMap<int> terrainMap{5, 5};
    // display(terrainMap);
    std::cout << '\n';
    StealthWorldGenerator::NoiseGenerator<100> noiseGenerator;
    StealthWorldGenerator::TileMap<float> noise = noiseGenerator.generate(900, 1600);
    // display(noise);
    std::cout << '\n';
    StealthWorldGenerator::InterpolationKernel<2> kernel;
    // display(kernel);
    // display(noise);
    std::cout << '\n';
    // std::cout << to_string(kernel.getDistanceAt(4, 1)) << '\n';
    // Main Loop.
    sf::Clock clock;
    sf::Texture noiseTexture;
    noiseTexture.loadFromImage(imageFromNoise(noise));
    sf::Sprite noiseSprite;
    noiseSprite.setTexture(noiseTexture);
    while (window.isOpen()) {
        // Handle events.
        sf::Event event;
        while (window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
              window.close();
            }
        }
        // Calculate frametime
        float frametime = clock.restart().asSeconds();
        // Clear previous frame.
        window.clear(sf::Color::White);
        // Draw
        window.draw(noiseSprite);
        // Display.
        window.display();
    }
}
