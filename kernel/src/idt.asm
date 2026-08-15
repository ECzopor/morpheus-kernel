;so the code knows this bit is from somewhere else
extern exception_handler

;another two macros for avinf gp registers
%macro pushagrd 0
push rax
push rbx
push rcx
push rdx
push rbp
push rsi
push rdi
push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15
%endmacro

%macro popagrd 0
pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop rdi
pop rsi
pop rbp
pop rdx
pop rcx
pop rbx
pop rax
%endmacro

;two macros, so I don't have to reapet myslef while creating IRS stubs
;why do we need to err and no err again? -> because some errors push error code to stack (BUT NOT ALL!), so we want to get it

%macro isr_err_stub 1
isr_stub_%+%1:
    pushagrd
    call exception_handler
    popagrd
    add rsp, 8
    iretq ;Short for Interrupt Return, this automaticlly restores the current state of registers etc (error code, RIP, CS, RFLAGS, RSP, and SS)
%endmacro

%macro isr_no_err_stub 1
isr_stub_%+%1:
    push qword 0 ;so each macro will the error code on the call stack
    pushagrd
    call exception_handler
    popagrd
    add rsp, 8
    iretq
%endmacro

;instantiating the 32 CPU exception entry points (vectors 0-31)
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

;this is a table with all the ISRs, so C can access it
global isr_stub_table
isr_stub_table:
%assign i 0 ;i=0
%rep    32 ;reapeat 32 times
    dq isr_stub_%+i ;assign the val in the isr to the table (64 bits)
%assign i i+1 ;i++
%endrep

section .note.GNU-stack noexec
