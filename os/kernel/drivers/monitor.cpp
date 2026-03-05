#include "monitor.h"
#include "../core/portio.h"

using namespace PortIO;

namespace Monitor {


    constexpr uint16_t VGA_WIDTH  = 80;
    constexpr uint16_t VGA_HEIGHT = 25;
    constexpr uint32_t VGA_BASE   = 0xB8000;


    constexpr uint16_t VGA_CTRL_PORT = 0x3D4;
    constexpr uint16_t VGA_DATA_PORT = 0x3D5;
    constexpr uint8_t  VGA_CURSOR_HI = 14;
    constexpr uint8_t  VGA_CURSOR_LO = 15;


    static volatile uint16_t* const vga_buf = (volatile uint16_t*)VGA_BASE;
    static uint16_t cursor_x    = 0;
    static uint16_t cursor_y    = 0;
    static uint8_t  cur_color   = make_color(WHITE, BLACK);




    static inline uint16_t vga_cell(char c, uint8_t color) {
        return (uint16_t)((uint8_t)c) | ((uint16_t)color << 8);
    }


    static void update_hw_cursor() {
        uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
        outb(VGA_CTRL_PORT, VGA_CURSOR_HI);
        outb(VGA_DATA_PORT, (uint8_t)(pos >> 8));
        outb(VGA_CTRL_PORT, VGA_CURSOR_LO);
        outb(VGA_DATA_PORT, (uint8_t)(pos & 0xFF));
    }


    static void scroll_up() {

        for (uint16_t row = 0; row < VGA_HEIGHT - 1; row++) {
            for (uint16_t col = 0; col < VGA_WIDTH; col++) {
                vga_buf[row * VGA_WIDTH + col] =
                    vga_buf[(row + 1) * VGA_WIDTH + col];
            }
        }

        uint16_t blank = vga_cell(' ', cur_color);
        for (uint16_t col = 0; col < VGA_WIDTH; col++) {
            vga_buf[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = blank;
        }
        if (cursor_y > 0) cursor_y--;
    }


    void set_color(uint8_t color) {
        cur_color = color;
    }

    uint8_t get_color() {
        return cur_color;
    }

    void clear() {
        uint16_t blank = vga_cell(' ', cur_color);
        for (uint16_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
            vga_buf[i] = blank;
        }
        cursor_x = 0;
        cursor_y = 0;
        update_hw_cursor();
    }

    void set_cursor(uint8_t col, uint8_t row) {
        cursor_x = col;
        cursor_y = row;
        update_hw_cursor();
    }

    void put_char_at(char c, uint8_t col, uint8_t row, uint8_t color) {
        vga_buf[(uint16_t)row * VGA_WIDTH + col] = vga_cell(c, color);
    }

    void put_char(char c) {
        switch (c) {
            case '\n':
                cursor_x = 0;
                cursor_y++;
                break;
            case '\r':
                cursor_x = 0;
                break;
            case '\b':
                if (cursor_x > 0) {
                    cursor_x--;
                    vga_buf[cursor_y * VGA_WIDTH + cursor_x] = vga_cell(' ', cur_color);
                }
                break;
            case '\t':
                cursor_x = (cursor_x + 8) & ~(uint16_t)7;
                break;
            default:
                vga_buf[cursor_y * VGA_WIDTH + cursor_x] = vga_cell(c, cur_color);
                cursor_x++;
                break;
        }

        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }

        if (cursor_y >= VGA_HEIGHT) {
            scroll_up();
        }

        update_hw_cursor();
    }

    void print(const char* str) {
        while (*str) {
            put_char(*str++);
        }
    }

    void newline() {
        put_char('\n');
    }

    void print_uint(uint32_t n) {
        if (n == 0) {
            put_char('0');
            return;
        }
        char buf[12];
        int  i = 0;
        while (n > 0) {
            buf[i++] = '0' + (char)(n % 10);
            n /= 10;
        }
        for (int j = i - 1; j >= 0; j--) {
            put_char(buf[j]);
        }
    }

    void print_int(int n) {
        if (n < 0) {
            put_char('-');
            // Avoid UB on INT_MIN by casting to unsigned
            print_uint((uint32_t)(-(n + 1)) + 1u);
        } else {
            print_uint((uint32_t)n);
        }
    }

    void print_hex(uint32_t n) {
        const char* digits = "0123456789ABCDEF";
        print("0x");
        bool leading = true;
        for (int shift = 28; shift >= 0; shift -= 4) {
            uint8_t nibble = (uint8_t)((n >> shift) & 0xF);
            if (nibble || !leading || shift == 0) {
                leading = false;
                put_char(digits[nibble]);
            }
        }
    }

    void print_colored(const char* str, uint8_t color) {
        uint8_t saved = cur_color;
        cur_color = color;
        print(str);
        cur_color = saved;
    }

}
