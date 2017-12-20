#include "TileMap/TileMap.hpp"
#include "Noise/NoiseGenerator.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <functional>

const int WINDOW_X = 800;
const int WINDOW_Y = 800;


constexpr float doubleUp(float in) {
    return in * 2.0;
}

constexpr float threshold(float in, float threshold) {
    return (in > threshold) ? in : 0.0f;
}

template <int rows, int cols>
constexpr sf::Image imageFromNoise(const StealthWorldGenerator::TileMap<float, rows, cols>& noise) {
    sf::Image im;
    im.create(cols, rows);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Scale from (0, 1) to (0, 255)
            int color = (noise.at(i, j)) * 255.0f;
            im.setPixel(j, i, sf::Color(255, color, color));
        }
    }
    return im;
}

int main() {
    // Window
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Noise Test");
    StealthWorldGenerator::NoiseGenerator noiseGenerator;
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

        auto noise = noiseGenerator.generateOctaves<WINDOW_Y, WINDOW_X, 400, 8>();
        auto noise2 = noiseGenerator.generateOctaves<WINDOW_Y, WINDOW_X, 400, 8>();

        //
        // noise = (noise < noise2) + (noise > noise2); // Should be all 1s (white)
        // noise = noise && (noise < noise2);
        // noise = noise * (noise > noise2);
        // StealthWorldGenerator::TileMapF<WINDOW_Y, WINDOW_X> noiseTest = StealthWorldGenerator::apply(doubleUp, noise);
        // noiseTest = StealthWorldGenerator::apply(std::bind(threshold, std::placeholders::_1, 0.25f), noise);
        //
        auto end = std::chrono::steady_clock::now();
        totalTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Average Time:  " << (totalTime / (float) ++numFrames) << " milliseconds" << '\r';

        // Show noise on-screen.
        sf::Texture noiseTexture;
        noiseTexture.loadFromImage(imageFromNoise(noise));
        sf::Sprite noiseSprite;
        noiseSprite.setTexture(noiseTexture);
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
        // std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}
