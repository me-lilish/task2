#include "interrupts.h"
#include "portio.h"
#include "pic.h"


__attribute__((aligned(0x10)))
static idt_entry_t idt[IDT_MAX_DESCRIPTORS];

static idtr_t idtr;


static uint8_t vectors[IDT_MAX_DESCRIPTORS];


static uint32_t pic_isr[IDT_PIC_DESCRIPTORS];


extern "C" void* isr_table[];


static uint16_t get_code_segment() {
    uint16_t cs;
    __asm__ volatile("mov %%cs, %0" : "=r"(cs));
    return cs;
}


static void set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* d = &idt[vector];
    uint32_t addr  = reinterpret_cast<uint32_t>(isr);

    d->isr_low   = addr & 0xFFFF;
    d->kernel_cs = get_code_segment();
    d->reserved  = 0;
    d->attributes = flags;
    d->isr_high  = (addr >> 16) & 0xFFFF;
}


extern "C" void interrupt_handler(cpu_state* cpu, uint32_t interrupt, stack_state* stack) {
    (void)cpu;   // Suppress unused-parameter warnings
    (void)stack;

    if (interrupt < IDT_INTEL_DESCRIPTORS) {

        PortIO::disable_interrupts();
        __asm__ volatile("hlt");

        while (1) {}

    } else if (interrupt < IDT_INTEL_DESCRIPTORS + IDT_PIC_DESCRIPTORS) {

        int pic_index = (int)interrupt - IDT_INTEL_DESCRIPTORS;

        if (vectors[interrupt]) {

            auto fn = reinterpret_cast<void(*)()>(pic_isr[pic_index]);
            fn();
        } else {

            PIC::acknowledge(pic_index);
        }
    }

}


namespace InterruptManager {

    void init() {

        idtr.base  = reinterpret_cast<uint32_t>(&idt[0]);
        idtr.limit = (uint16_t)(sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS) - 1;


        for (uint8_t v = 0; v < IDT_INTEL_DESCRIPTORS; v++) {
            set_descriptor(v, isr_table[v], 0x8E);
            vectors[v] = 1;
        }

        for (uint8_t v = IDT_INTEL_DESCRIPTORS;
             v < IDT_INTEL_DESCRIPTORS + IDT_PIC_DESCRIPTORS; v++) {
            set_descriptor(v, isr_table[v], 0x8E);
            vectors[v] = 0;
        }


        load_idt(&idtr);
    }

    void add_interrupt(interruption* inr) {
        int pic_index = inr->code - IDT_INTEL_DESCRIPTORS;
        pic_isr[pic_index] = inr->callback;
        set_descriptor(inr->code, isr_table[inr->code], 0x8E);
        vectors[inr->code] = 1;
    }

}
