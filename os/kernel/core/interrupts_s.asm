extern interrupt_handler

%macro isr_err 1
isr_%+%1:
    push dword %1
    jmp common_interrupt_handler
%endmacro

%macro isr_no_err 1
isr_%+%1:

    push dword 0
    push dword %1
    jmp common_interrupt_handler
%endmacro

common_interrupt_handler:
    pushad

    lea eax, [esp + 36]
    push eax
    

    push dword [esp + 32 + 4]

    lea eax, [esp + 8]
    push eax

    call interrupt_handler

    add esp, 12

    popad

    add esp, 8
    
    iret


isr_no_err 0
isr_no_err 1
isr_no_err 2
isr_no_err 3
isr_no_err 4
isr_no_err 5
isr_no_err 6
isr_no_err 7
isr_err    8
isr_no_err 9
isr_err    10
isr_err    11
isr_err    12
isr_err    13
isr_err    14
isr_no_err 15
isr_no_err 16
isr_err    17
isr_no_err 18
isr_no_err 19
isr_no_err 20
isr_no_err 21
isr_no_err 22
isr_no_err 23
isr_no_err 24
isr_no_err 25
isr_no_err 26
isr_no_err 27
isr_no_err 28
isr_no_err 29
isr_err    30
isr_no_err 31


isr_no_err 32
isr_no_err 33
isr_no_err 34
isr_no_err 35
isr_no_err 36
isr_no_err 37
isr_no_err 38
isr_no_err 39
isr_no_err 40
isr_no_err 41
isr_no_err 42
isr_no_err 43
isr_no_err 44
isr_no_err 45
isr_no_err 46
isr_no_err 47


global load_idt
load_idt:
    mov eax, [esp+4]
    lidt [eax]
    ret


global isr_table
isr_table:
%assign i 0
%rep    48
    dd isr_%+i
%assign i i+1
%endrep