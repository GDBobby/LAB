#pragma once

#include "VectorTemplate.h"

#include <functional>
//straight ripped from glm

namespace lab{
    namespace detail{
        inline void hash_combine(std::size_t &seed, std::size_t hash)
        {
            hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= hash;
        }
    }
}

namespace std {
    template<std::floating_point F, uint8_t Dimensions>
    struct hash<lab::Vector<F, Dimensions>> {
        std::size_t operator()(lab::Vector<F, Dimensions> const& v) const {
            std::size_t seed = 0;
            std::hash<F> hasher;  // fix: use std::hash<F>, not undefined T

            lab::detail::hash_combine(seed, hasher(v.x));
            lab::detail::hash_combine(seed, hasher(v.y));
            if constexpr (Dimensions >= 3) {
                lab::detail::hash_combine(seed, hasher(v.z));
            }
            if constexpr (Dimensions == 4) {
                lab::detail::hash_combine(seed, hasher(v.w));
            }

            return seed;
        }
    };
}