#include "timer.h"
#include "../core/portio.h"
#include "../core/pic.h"
#include "../core/interrupts.h"

using namespace PortIO;

namespace Timer {


    constexpr uint16_t PIT_CHANNEL0   = 0x40;
    constexpr uint16_t PIT_COMMAND    = 0x43;


    constexpr uint32_t PIT_BASE_FREQ  = 1193180;

    // IRQ / IDT mapping
    constexpr uint8_t  IRQ_NUMBER     = 0;
    constexpr uint8_t  IDT_VECTOR     = 32;


    constexpr uint8_t  PIT_CMD_RATE   = 0x36;


    static volatile uint32_t tick_count  = 0;
    static          uint32_t freq_hz     = 0;

    static interruption intr_def;


    static void handler() {
        tick_count++;
        PIC::acknowledge(IRQ_NUMBER);
    }



    void init(uint32_t frequency) {
        freq_hz    = frequency;
        tick_count = 0;

        uint32_t divisor = PIT_BASE_FREQ / frequency;

        outb(PIT_COMMAND, PIT_CMD_RATE);
        outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
        outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));

        // --- Register interrupt handler ---
        intr_def.code     = IDT_VECTOR;
        intr_def.callback = reinterpret_cast<uint32_t>(&handler);
        InterruptManager::add_interrupt(&intr_def);


        PIC::enable_irq(IRQ_NUMBER);
    }

    uint32_t get_ticks() {
        return tick_count;
    }

    uint32_t get_uptime_ms() {
        if (freq_hz == 0) return 0;
        return (tick_count * 1000u) / freq_hz;
    }

    void sleep(uint32_t ms) {
        if (freq_hz == 0) return;

        uint32_t ticks_needed = (ms * freq_hz) / 1000u;
        uint32_t start        = tick_count;

        while ((tick_count - start) < ticks_needed) {
            __asm__ volatile("hlt");
        }
    }

}
