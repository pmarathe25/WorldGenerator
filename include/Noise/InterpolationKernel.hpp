#ifndef STEALTH_INTERPOLATION_H
#define STEALTH_INTERPOLATION_H
#include "TileMap/TileMap.hpp"
#include "Utility.hpp"

namespace StealthWorldGenerator {
    class InterpolationKernelBase { };

    // Maintains a cache of points and attenuations to use for each possible location of a pixel.
    template <int scale = 1>
    class InterpolationKernel : public InterpolationKernelBase {
        public:
            constexpr InterpolationKernel() {
                initializeKernel();
            }

            constexpr InterpolationKernel(const InterpolationKernel& other) noexcept = default;

            constexpr InterpolationKernel(InterpolationKernel&& other) noexcept = default;

            constexpr const TileMap<float, 1, scale>& getPoints() const noexcept {
                return points;
            }

            constexpr const TileMap<float, 1, scale>& getAttenuations() const noexcept {
                return attenuations;
            }

        private:
            TileMap<float, 1, scale> points{};
            TileMap<float, 1, scale> attenuations{};

            constexpr void initializeKernel() noexcept {
                // Optimally initialize kernel. Only need to compute 1/8th of the kernel.
                constexpr int halfBound = ceilDivide(scale, 2);
                initializeHalf(halfBound);
                reflectHalf(halfBound);
            }

            constexpr float calculatePoint(int index) const noexcept {
                // Compute a relative location.
                return (index / (float) scale) + (0.5f / scale);
            }

            constexpr void initializeHalf(int halfBound) {
                for (int i = 0; i < halfBound; ++i) {
                    float point = calculatePoint(i);
                    points.at(0, i) = point;
                    attenuations.at(0, i) = attenuationPolynomial(point);
                }
            }

            constexpr void reflectHalf(int halfBound) {
                for (int i = scale - 1, index = 0; i >= halfBound; --i, ++index) {
                    float point = 1.0f - points[index];
                    points.at(0, i) = point;
                    attenuations.at(0, i) = attenuationPolynomial(point);
                }
            }
    };

} /* StealthWorldGenerator */

#endif
