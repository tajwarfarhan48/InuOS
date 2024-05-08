[BITS 32]

global divide_by_zero_handler
global timer_handler
global keyboard_press_handler
global no_int_handler

extern divide_by_zero_service_routine
extern timer_service_routine
extern keyboard_press_service_routine
extern no_int_service_routine

divide_by_zero_handler:
    cli
    pushad

    call divide_by_zero_service_routine

    popad
    sti

    jmp $

timer_handler:
    cli
    pushad 

    call timer_service_routine

    popad
    sti
    iret

keyboard_press_handler:
    cli
    pushad

    call keyboard_press_service_routine

    popad
    sti
    iret

no_int_handler:
    cli
    pushad

    call no_int_service_routine

    popad
    sti
    iret
