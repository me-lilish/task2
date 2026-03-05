#include "ktypes.h"
#include "portio.h"

namespace PortIO {


    void outb(uint16_t port, uint8_t data) {
        __asm__ volatile (
            "outb %0, %1"
            :
            : "a"(data), "Nd"(port)
        );
    }


    void outw(uint16_t port, uint16_t data) {
        __asm__ volatile (
            "outw %0, %1"
            :
            : "a"(data), "Nd"(port)
        );
    }


    uint8_t inb(uint16_t port) {
        uint8_t result;
        __asm__ volatile (
            "inb %1, %0"
            : "=a"(result)
            : "Nd"(port)
        );
        return result;
    }


    uint16_t inw(uint16_t port) {
        uint16_t result;
        __asm__ volatile (
            "inw %1, %0"
            : "=a"(result)
            : "Nd"(port)
        );
        return result;
    }


    void enable_interrupts(void) {
        __asm__ volatile ("sti");
    }


    void disable_interrupts(void){
        __asm__ volatile ("cli");
    }


    void io_wait(void) {
        outb(0x80, 0);
    }

}