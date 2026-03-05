#include "math.h"

namespace Math {


    union FloatBits {
        float    f;
        uint32_t u;
    };

    float fabs(float x) {
        FloatBits v;
        v.f = x;
        v.u &= 0x7FFFFFFF;
        return v.f;
    }


    int round(float x) {

        FloatBits v;
        v.f = x;
        int sign = (v.u >> 31) ? -1 : 1;
        v.u &= 0x7FFFFFFF;
        uint32_t input = v.u;


        int exponent = (int)((input >> 23) & 0xFF) - 126;


        uint32_t mantissa = ((1u << 23) | (input & 0x7FFFFF)) << 8;

        int result;
        if (exponent < 0) {

            result = 0;
        } else if (exponent == 0) {

            result = 1;
        } else if (exponent > 24) {

            result = (int)v.f;
        } else {

            result = (int)(mantissa >> (32 - exponent));

            result += (mantissa >> (31 - exponent)) & 1;
        }

        return sign * result;
    }


    int floor(float x) {
        int i = (int)x;
        FloatBits v;
        v.f = x;
        bool neg = (v.u >> 31) != 0;
        return (neg && (float)i != x) ? i - 1 : i;
    }

    int ceil(float x) {
        int i = (int)x;
        FloatBits v;
        v.f = x;
        bool neg = (v.u >> 31) != 0;
        return (!neg && (float)i != x) ? i + 1 : i;
    }


    uint32_t sqrt(uint32_t n) {
        if (n == 0) return 0;
        uint32_t x = n;
        uint32_t y = 1;
        while (x > y) {
            x = (x + y) / 2;
            y = n / x;
        }
        return x;
    }


    float pow(float base, int exp) {
        if (exp == 0) return 1.0f;

        float result = 1.0f;
        bool negative = (exp < 0);
        int  e = negative ? -exp : exp;

        while (e > 0) {
            if (e & 1) result *= base;
            base *= base;
            e >>= 1;
        }

        if (negative) {
            FloatBits one, res;
            one.f = 1.0f;
            res.f = result;
            result = 1.0f / result;
        }

        return result;
    }

}
