PUBLIC initusb_asm
EXTERN puts:PROC

.data

    hello1 db "Hello from ASM.",0

.code

initusb_asm PROC
    push rbp
    mov rbp, rsp
    sub rsp, 32                 ; Shadow Space
    and spl, -16                ; Align stack at 16

    lea rcx, hello1
    call puts

    leave                       ; Restore stack (rsp) & frame pointer (rbp)
    ret
initusb_asm ENDP

END