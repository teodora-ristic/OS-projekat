.extern _ZN5Riscv10handleIntrEv
.extern _ZN5Riscv11handleTimerEv
.extern _ZN5Riscv13handleConsoleEv

.align 4
.global _ZN5Riscv14supervisorTrapEv
.type _ZN5Riscv14supervisorTrapEv, @function
_ZN5Riscv14supervisorTrapEv:
     j _ZN5Riscv8intrTrapEv
     j _ZN5Riscv9timerTrapEv
     addi x0,x0,0
     addi x0,x0,0
     addi x0,x0,0
     addi x0,x0,0
     addi x0,x0,0
     addi x0,x0,0
     addi x0,x0,0
     j _ZN5Riscv11consoleTrapEv


_ZN5Riscv8intrTrapEv:

    # push all registers to stack
    addi sp, sp, -256
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
    .endr

    call _ZN5Riscv10handleIntrEv


    //sd a0,80(sp)

     # pop all registers from stack
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256

    sret

_ZN5Riscv9timerTrapEv:

# push all registers to stack
    addi sp, sp, -256
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
    .endr

    call _ZN5Riscv11handleTimerEv


    //sd a0,80(sp)

     # pop all registers from stack
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256

    sret

_ZN5Riscv11consoleTrapEv:

    # push all registers to stack
    addi sp, sp, -256
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
    .endr

    call _ZN5Riscv13handleConsoleEv


    //sd a0,80(sp)

     # pop all registers from stack
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr
    addi sp, sp, 256

    sret



