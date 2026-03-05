#pragma once
#include "ktypes.h"


constexpr uint16_t IDT_MAX_DESCRIPTORS = 256;
constexpr uint16_t IDT_INTEL_DESCRIPTORS = 32;
constexpr uint16_t IDT_PIC_DESCRIPTORS = 16;


struct __attribute__((packed)) idt_entry_t {
    uint16_t    isr_low;
    uint16_t    kernel_cs;
    uint8_t     reserved;
    uint8_t     attributes;
    uint16_t    isr_high;
};


struct __attribute__((packed)) idtr_t {
    uint16_t    limit;
    uint32_t    base;
};


struct __attribute__((packed)) cpu_state {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
};


struct __attribute__((packed)) stack_state {
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
};


struct __attribute__((packed)) interruption {
    uint8_t code;
    uint32_t callback;
};


namespace InterruptManager {
    void init();
    void add_interrupt(interruption* intr);
}


extern "C" {

    void interrupt_handler(cpu_state* cpu, uint32_t interrupt, stack_state* stack);
    
    void load_idt(idtr_t* _idtr);
}