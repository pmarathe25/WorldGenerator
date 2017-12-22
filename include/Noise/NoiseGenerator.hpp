#ifndef NOISE_GENERATOR_H
#define NOISE_GENERATOR_H
#include <unordered_map>
#include <thread>
#include <any>

namespace StealthWorldGenerator {
    class NoiseGenerator {
        public:
        protected:
            std::array<std::thread, NUM_THREADS> noiseThreads;
            // Maintain a cache of interpolation kernels of different sizes
            static std::unordered_map<int, std::any> kernels;
    };

    std::unordered_map<int, std::any> NoiseGenerator::kernels{};
} /* StealthWorldGenerator */

#endif /* end of include guard: STEALTH_NOISE_GENERATOR_H */
