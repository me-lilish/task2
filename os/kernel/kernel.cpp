

#include "core/ktypes.h"
#include "core/portio.h"
#include "core/interrupts.h"
#include "core/pic.h"
#include "core/math.h"

#include "drivers/monitor.h"
#include "drivers/timer.h"
#include "drivers/keyboard.h"

using namespace PortIO;
using namespace Monitor;



static void print_separator(char ch = '-', int len = 60) {
    for (int i = 0; i < len; i++) put_char(ch);
    newline();
}

static void print_label(const char* label, uint8_t color = make_color(YELLOW, BLACK)) {
    print_colored(label, color);
}

static void run_math_tests() {
    set_color(make_color(CYAN, BLACK));
    print("[ Math Library Tests ]\n");
    set_color(make_color(WHITE, BLACK));
    print_separator();

    struct {
        const char* expr;
        int         result;
        int         expected;
    } cases[] = {
        { "round(2.4)   ", Math::round(2.4f),   2  },
        { "round(2.5)   ", Math::round(2.5f),   3  },
        { "round(-1.6)  ", Math::round(-1.6f), -2  },
        { "round(0.3)   ", Math::round(0.3f),   0  },
        { "round(7.0)   ", Math::round(7.0f),   7  },
        { "floor(3.9)   ", Math::floor(3.9f),   3  },
        { "floor(-2.1)  ", Math::floor(-2.1f), -3  },
        { "ceil(2.1)    ", Math::ceil(2.1f),    3  },
        { "ceil(-1.9)   ", Math::ceil(-1.9f),  -1  },
        { "sqrt(16)     ", (int)Math::sqrt(16), 4  },
        { "sqrt(100)    ", (int)Math::sqrt(100),10 },
        { "sqrt(2)      ", (int)Math::sqrt(2),  1  },
        { "abs(-42)     ", Math::abs(-42),      42 },
        { "min(3,7)     ", Math::min(3, 7),     3  },
        { "max(3,7)     ", Math::max(3, 7),     7  },
        { "clamp(15,0,10)", Math::clamp(15,0,10), 10 },
        { "clamp(-5,0,10)", Math::clamp(-5,0,10), 0  },
        { "ipow(2,8)    ", Math::ipow(2, 8),   256 },
        { "sign(-99)    ", Math::sign(-99),     -1  },
    };

    int passed = 0, total = (int)(sizeof(cases) / sizeof(cases[0]));

    for (int i = 0; i < total; i++) {
        print("  ");
        print(cases[i].expr);
        print(" = ");
        print_int(cases[i].result);

        bool ok = (cases[i].result == cases[i].expected);
        if (ok) {
            print_colored("  [PASS]\n", make_color(LIGHT_GREEN, BLACK));
            passed++;
        } else {
            print_colored("  [FAIL] expected ", make_color(LIGHT_RED, BLACK));
            print_int(cases[i].expected);
            newline();
        }
    }

    print_separator();
    print("  Result: ");
    print_int(passed); print("/"); print_int(total);
    print(" tests passed");
    if (passed == total) {
        print_colored("  ALL PASS!\n", make_color(LIGHT_GREEN, BLACK));
    } else {
        print_colored("  SOME FAILED\n", make_color(LIGHT_RED, BLACK));
    }
    print_separator();
    newline();
}

static void run_timer_demo() {
    set_color(make_color(CYAN, BLACK));
    print("[ Timer Demo ]\n");
    set_color(make_color(WHITE, BLACK));
    print_separator();

    print("  Tick frequency: 1000 Hz (1 ms per tick)\n");
    print("  Sleeping 500 ms... ");
    uint32_t before = Timer::get_uptime_ms();
    Timer::sleep(500);
    uint32_t after  = Timer::get_uptime_ms();
    print("done (");
    print_uint(after - before);
    print_colored(" ms elapsed)\n", make_color(LIGHT_GREEN, BLACK));

    print("  Ticks since boot: ");
    print_uint(Timer::get_ticks());
    newline();
    print_separator();
    newline();
}

static void run_keyboard_shell() {
    set_color(make_color(CYAN, BLACK));
    print("[ Keyboard Shell ]\n");
    set_color(make_color(WHITE, BLACK));
    print_separator();
    print("  Type keys. Backspace works. Press Enter for a new line.\n");
    print("  (loop is infinite - reset QEMU to exit)\n");
    print_separator();
    newline();


    char line[80];
    int  pos = 0;


    print_colored("  > ", make_color(YELLOW, BLACK));

    while (1) {
        char c = Keyboard::get_char();

        if (c == '\n') {

            newline();
            pos = 0;
            print_colored("  > ", make_color(YELLOW, BLACK));
        } else if (c == '\b') {
            if (pos > 0) {
                pos--;
                put_char('\b');
            }
        } else if (pos < 79) {
            line[pos++] = c;
            put_char(c);
        }

    }
}


extern "C" int kmain() {


    InterruptManager::init();

    PIC::remap(32, 40);

    Monitor::set_color(make_color(WHITE, BLACK));
    Monitor::clear();

    Timer::init(1000);

    Keyboard::init();

    enable_interrupts();


    set_color(make_color(LIGHT_CYAN, BLACK));
    print("  +==========================================================+\n");
    print("  |                   Mini OS - Booted OK                    |\n");
    print("  +==========================================================+\n");
    set_color(make_color(WHITE, BLACK));
    newline();


    run_math_tests();


    run_timer_demo();


    run_keyboard_shell();

    return 0;
}
