#pragma once

#include "VectorTemplate.h"

#include <functional>
#include <cstring>
#include <cmath> //just for isnan
//straight ripped from glm

namespace lab{
    namespace detail{
        inline void hash_combine(std::size_t &seed, std::size_t hash)
        {
            hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= hash;
        }

        //https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp
        inline uint32_t fmix32 ( uint32_t h ) {
            h ^= h >> 16;
            h *= 0x85ebca6b;
            h ^= h >> 13;
            h *= 0xc2b2ae35;
            h ^= h >> 16;

            return h;
        }


        inline uint32_t rotl32 ( uint32_t x, int8_t r ) {
            return (x << r) | (x >> (32 - r));
        }

        inline uint32_t getblock32 ( const uint32_t * p, int i ) {
            return p[i];
        }

        void MurmurHash3_x86_32 ( const void * key, int len, uint32_t seed, void * out ) {
            const uint8_t * data = (const uint8_t*)key;
            const int nblocks = len / 4;

            uint32_t h1 = seed;

            const uint32_t c1 = 0xcc9e2d51;
            const uint32_t c2 = 0x1b873593;

            //----------
            // body

            const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

            for(int i = -nblocks; i; i++) {
                uint32_t k1 = getblock32(blocks,i);

                k1 *= c1;
                k1 = rotl32(k1,15);
                k1 *= c2;
                
                h1 ^= k1;
                h1 = rotl32(h1,13); 
                h1 = h1*5+0xe6546b64;
            }

            //----------
            // tail

            const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

            uint32_t k1 = 0;

            switch(len & 3) {
                case 3: k1 ^= tail[2] << 16;
                case 2: k1 ^= tail[1] << 8;
                case 1: k1 ^= tail[0];
                        k1 *= c1; k1 = rotl32(k1,15); k1 *= c2; h1 ^= k1;
            };

            //----------
            // finalization

            h1 ^= len;

            h1 = fmix32(h1);

            *(uint32_t*)out = h1;
        } 


        uint32_t HashFloat(float f, uint32_t seed = 0) {
            if (f == 0.0f) {
                f = 0.0f; //this removes the sign from -0.0f
            }

            if (std::isnan(f)) {
                f = std::numeric_limits<float>::quiet_NaN();
            }

            uint32_t out;
            MurmurHash3_x86_32(&f, sizeof(float), seed, &out);
            
            return out;
        }
    }
}

namespace std {
    template<std::floating_point F, uint8_t Dimensions>
    struct hash<lab::Vector<F, Dimensions>> {
        std::size_t operator()(lab::Vector<F, Dimensions> const& v) const {
            std::size_t seed = 0;

            lab::detail::hash_combine(seed, lab::detail::HashFloat(v.x, seed));
            lab::detail::hash_combine(seed, lab::detail::HashFloat(v.y, seed));
            if constexpr (Dimensions >= 3) {
                lab::detail::hash_combine(seed, lab::detail::HashFloat(v.z, seed));
            }
            if constexpr (Dimensions == 4) {
                lab::detail::hash_combine(seed, lab::detail::HashFloat(v.w, seed));
            }

            return seed;
        }
    };
}