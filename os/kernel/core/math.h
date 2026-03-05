#pragma once
#include "ktypes.h"


namespace Math {


    int round(float x);


    int floor(float x);


    int ceil(float x);


    float fabs(float x);


    uint32_t sqrt(uint32_t n);
    float pow(float base, int exp);



    constexpr int abs(int x) {
        return (x < 0) ? -x : x;
    }


    constexpr int min(int a, int b) {
        return (a < b) ? a : b;
    }


    constexpr int max(int a, int b) {
        return (a > b) ? a : b;
    }


    constexpr int clamp(int val, int lo, int hi) {
        return (val < lo) ? lo : (val > hi) ? hi : val;
    }


    constexpr int sign(int x) {
        return (x > 0) - (x < 0);
    }


    constexpr int ipow(int base, int exp) {
        int result = 1;
        while (exp-- > 0) result *= base;
        return result;
    }


    constexpr bool is_pow2(uint32_t n) {
        return (n != 0) && ((n & (n - 1)) == 0);
    }


    constexpr uint32_t align_up(uint32_t val, uint32_t align) {
        return (val + align - 1) & ~(align - 1);
    }


    constexpr uint32_t align_down(uint32_t val, uint32_t align) {
        return val & ~(align - 1);
    }

}
