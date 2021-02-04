.section .text.entry

_start:
    ldr     pc, reset_target        /* 0x00 mode: svc */
    ldr     pc, undefined_target    /* 0x04 mode: ? */
    ldr     pc, swi_target          /* 0x08 mode: svc */
    ldr     pc, prefetch_target     /* 0x0c mode: abort */
    ldr     pc, abort_target        /* 0x10 mode: abort */
    ldr     pc, unused_target       /* 0x14 unused */
    ldr     pc, irq_target          /* 0x18 mode: irq */
    ldr     pc, fiq_target          /* 0x1c mode: fiq */

reset_target:           .word   reset_entry
undefined_target:       .word   undefined_entry
swi_target:             .word   swi_entry
prefetch_target:        .word   prefetch_entry
abort_target:           .word   abort_entry
unused_target:          .word   unused_entry
irq_target:             .word   irq_entry
fiq_target:             .word   fiq_entry


reset_entry:
    /* set VBAR to 0x8000 */
    mov r0, #0x8000
    mcr p15, 0, r0, c12, c0, 0

    /* (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS) */
    mov r0,#0xD1
    msr cpsr_c,r0
    ldr sp, stack_fiq_top

    /* (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS) */
    mov r0,#0xD2
    msr cpsr_c,r0
    ldr sp, stack_irq_top

    /* (PSR_ABT_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS) */
    mov r0,#0xD7
    msr cpsr_c,r0
    ldr sp, stack_abt_top

    /* (PSR_UND_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS) */
    mov r0,#0xDb
    msr cpsr_c,r0
    ldr sp, stack_und_top

    /* (PSR_SYS_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS) */
    mov r0,#0xDf
    msr cpsr_c,r0
    ldr sp, stack_sys_top

    /* (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS) */
    mov r0,#0xD3
    msr cpsr_c,r0
    ldr sp, stack_svc_top

    cpsie i
    bl  bare_metal_start

undefined_entry:
    b reset_entry

swi_entry:
    stmfd  sp!, {r0-r12, lr}

    ldr     r0, [lr,#-4]
    bic     r0, r0, #0xff000000
    add     lr, pc, #4
    bl      syscall_entry

    ldmfd   sp!, {r0-r12, lr}
    movs    pc, lr

prefetch_entry:
    b reset_entry

abort_entry:
    b reset_entry

unused_entry:
    b reset_entry

fiq_entry:
    b reset_entry

irq_entry:
    stmfd  sp!, {r0-r12, lr}

    add     lr, pc, #4
    bl      isr_entry

    ldmfd   sp!, {r0-r12, lr}

    subs    pc, lr, #4


stack_svc_top:      .word   0x100000
stack_fiq_top:      .word   0x110000
stack_irq_top:      .word   0x120000
stack_abt_top:      .word   0x130000
stack_und_top:      .word   0x140000
stack_sys_top:      .word   0x150000
