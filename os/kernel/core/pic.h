#pragma once
#include "ktypes.h"


namespace PIC {

    constexpr uint16_t PIC1_COMMAND = 0x20;
    constexpr uint16_t PIC1_DATA    = 0x21;
    constexpr uint16_t PIC2_COMMAND = 0xA0;
    constexpr uint16_t PIC2_DATA    = 0xA1;

    // --- Commands ---
    constexpr uint8_t PIC_EOI       = 0x20;


    constexpr uint8_t ICW1_ICW4       = 0x01;
    constexpr uint8_t ICW1_SINGLE     = 0x02;
    constexpr uint8_t ICW1_INTERVAL4  = 0x04;
    constexpr uint8_t ICW1_LEVEL      = 0x08;
    constexpr uint8_t ICW1_INIT       = 0x10;

    constexpr uint8_t ICW4_8086       = 0x01;
    constexpr uint8_t ICW4_AUTO       = 0x02;
    constexpr uint8_t ICW4_BUF_SLAVE  = 0x08;
    constexpr uint8_t ICW4_BUF_MASTER = 0x0C;
    constexpr uint8_t ICW4_SFNM       = 0x10;

    // Offsets
    constexpr uint8_t PIC1_OFFSET     = 0x20;
    constexpr uint8_t PIC2_OFFSET     = 0x28;


    void remap(int offset1, int offset2);


    void acknowledge(int irq);


    void enable_irq(int irq);


    void disable_irq(int irq);
}