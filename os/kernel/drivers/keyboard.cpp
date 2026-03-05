#include "keyboard.h"
#include "../core/portio.h"
#include "../core/pic.h"
#include "../core/interrupts.h"

using namespace PortIO;

namespace Keyboard {

    // --- Hardware Constants ---
    constexpr uint16_t DATA_PORT    = 0x60;
    constexpr uint8_t  IRQ_NUMBER   = 1;
    constexpr uint8_t  IDT_VECTOR   = PIC::PIC1_COMMAND + IRQ_NUMBER;


    static volatile char last_ascii_char = 0;
    static volatile bool new_data_available = false;


    static interruption intr_def;


    static const char scan_code_table[128] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
        '2', '3', '0', '.'

    };


    void handler() {

        uint8_t scan_code = inb(DATA_PORT);

        if (!(scan_code & 0x80)) {
            

            if (scan_code < sizeof(scan_code_table)) {
                char ascii = scan_code_table[scan_code];
                
                if (ascii != 0) {

                    last_ascii_char = ascii;
                    new_data_available = true;
                }
            }
        }


        PIC::acknowledge(IRQ_NUMBER);
    }



    void init() {

        PIC::enable_irq(IRQ_NUMBER);
        

        intr_def.code = IDT_VECTOR;
        

        intr_def.callback = reinterpret_cast<uint32_t>(&handler);


        InterruptManager::add_interrupt(&intr_def);
    }


    char get_char() {

        new_data_available = false;
        while(new_data_available == false) {
            __asm__ volatile("hlt");
        }

        return last_ascii_char;
    }

}