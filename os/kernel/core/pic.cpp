#include "pic.h"
#include "portio.h"

using namespace PortIO;

namespace PIC {


    void acknowledge(int irq) {

        if (irq >= 8) {
            outb(PIC2_COMMAND, PIC_EOI);
        }

        outb(PIC1_COMMAND, PIC_EOI);
    }


    void remap(int offset1, int offset2) {

        outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
        io_wait();
        outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
        io_wait();


        outb(PIC1_DATA, offset1); 
        io_wait();
        outb(PIC2_DATA, offset2); 
        io_wait();


        outb(PIC1_DATA, 4); 
        io_wait();

        outb(PIC2_DATA, 2); 
        io_wait();


        outb(PIC1_DATA, ICW4_8086);
        io_wait();
        outb(PIC2_DATA, ICW4_8086);
        io_wait();


        outb(PIC1_DATA, 0xFF);
        outb(PIC2_DATA, 0xFF);
    }


    void enable_irq(int irq) {
        uint16_t port;
        uint8_t value;

        if (irq < 8) {
            port = PIC1_DATA;
        } else {
            port = PIC2_DATA;
            irq -= 8;
        }


        value = inb(port) & ~(1 << irq); 
        outb(port, value);
    }


    void disable_irq(int irq) {
        uint16_t port;
        uint8_t value;

        if (irq < 8) {
            port = PIC1_DATA;
        } else {
            port = PIC2_DATA;
            irq -= 8;
        }


        value = inb(port) | (1 << irq); 
        outb(port, value);
    }
}