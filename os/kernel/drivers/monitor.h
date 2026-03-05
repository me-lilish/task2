#pragma once
#include "../core/ktypes.h"



namespace Monitor {


    enum Color : uint8_t {
        BLACK         = 0x0,
        BLUE          = 0x1,
        GREEN         = 0x2,
        CYAN          = 0x3,
        RED           = 0x4,
        MAGENTA       = 0x5,
        BROWN         = 0x6,
        LIGHT_GREY    = 0x7,
        DARK_GREY     = 0x8,
        LIGHT_BLUE    = 0x9,
        LIGHT_GREEN   = 0xA,
        LIGHT_CYAN    = 0xB,
        LIGHT_RED     = 0xC,
        LIGHT_MAGENTA = 0xD,
        YELLOW        = 0xE,
        WHITE         = 0xF,
    };


    constexpr uint8_t make_color(Color fg, Color bg) {
        return (uint8_t)fg | ((uint8_t)bg << 4);
    }


    void clear();


    void set_color(uint8_t color);


    uint8_t get_color();


    void set_cursor(uint8_t col, uint8_t row);


    void put_char(char c);


    void print(const char* str);


    void print_int(int n);


    void print_uint(uint32_t n);


    void print_hex(uint32_t n);


    void newline();


    void put_char_at(char c, uint8_t col, uint8_t row, uint8_t color);


    void print_colored(const char* str, uint8_t color);

}
