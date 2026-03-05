[bits 32]
[extern kmain]
global start
start:
    call kmain
    jmp $