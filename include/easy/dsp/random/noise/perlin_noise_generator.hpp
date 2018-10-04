/*
 * EasyDSP, A cross-platform Digital Signal Processing library written in modern C++.
 * Copyright (C) 2018 Mohammed Boujemaoui Boulaghmoudi, All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along withº
 * this program.  If not, see <http://www.gnu.org/licenses/>
 *
 * Filename: perlin_noise_generator.hpp
 * Author: Mohammed Boujemaoui
 * Date: 1/8/2018
 */
#ifndef EASYDSP_PERLIN_NOISE_GENERATOR_HPP
#define EASYDSP_PERLIN_NOISE_GENERATOR_HPP

#include <array>
#include <random>
#include <chrono>

namespace easy { namespace dsp { namespace random {

    inline namespace internal {
        template <typename T>
        constexpr T fade(T t) noexcept {
            return t * t * t * (t * (t * 6 - 15) + 10);
        }

        template <typename T>
        constexpr T lerp(T t, T a, T b) noexcept {
            return a + t * (b - a);
        }

        template <typename Integer, typename T>
        constexpr T grad(Integer hash, T x) noexcept {
            return (hash & 1) == 0 ? x : -x;
        }

    } // namespace internal

    /**
    * @class perlin_noise_generator
    * @brief This class implements a Perlin noise generator.
    */
    template <typename T, typename Engine = std::mt19937>
    struct perlin_noise_generator {
        using value_type = T;

        /**
         * @brief Creates a Perlin noise sequence generator.
         */
        inline perlin_noise_generator() :
            generator_(Engine(static_cast<std::size_t>(std::chrono::system_clock::now().time_since_epoch().count()))),
            distribution_(std::uniform_int_distribution<T>(0, 511)) {}

        /**
        * @brief Generates a random number following the noise distribution.
        * @return The generated random number.
        */
        inline value_type operator()() {
            const auto random_value = distribution_(generator_);
            const auto index        = static_cast<std::int64_t>(random_value) & 0xFF;
            const auto floor_value  = random_value - std::floor(random_value);
            const auto fade_value   = fade(random_value);
            return lerp(fade_value, grad(permutation[index], floor_value),
                        grad(permutation[index + 1], floor_value - 1)) *
                   2;
        }

    private:
        Engine generator_;
        std::uniform_int_distribution<T> distribution_;
        static constexpr const std::array<T, 512> permutation = {
            {151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,   225, 140, 36,  103, 30,
             69,  142, 8,   99,  37,  240, 21,  10,  23,  190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,
             94,  252, 219, 203, 117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136,
             171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158, 231, 83,  111, 229, 122,
             60,  211, 133, 230, 220, 105, 92,  41,  55,  46,  245, 40,  244, 102, 143, 54,  65,  25,  63,  161,
             1,   216, 80,  73,  209, 76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,
             164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,   202, 38,  147, 118, 126,
             255, 82,  85,  212, 207, 206, 59,  227, 47,  16,  58,  17,  182, 189, 28,  42,  223, 183, 170, 213,
             119, 248, 152, 2,   44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
             19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,  228, 251, 34,  242, 193,
             238, 210, 144, 12,  191, 179, 162, 241, 81,  51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,
             181, 199, 106, 157, 184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,
             222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156, 180}};
    };

}}}    // namespace easy::dsp::random
#endif // EASYDSP_PERLIN_NOISE_GENERATOR_HPP
