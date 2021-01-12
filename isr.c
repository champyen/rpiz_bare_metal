
#include "reg.h"
#include "printf.h"

#define STIMER_BASE 0x20003000
#define STIMER_CS   0x00
#define STIMER_CLO  0x04
#define STIMER_CHI  0x08
#define STIMER_C0   0x0C
#define STIMER_C1   0x10
#define STIMER_C2   0x14
#define STIMER_C3   0x18

#define IRQ_BASE    0x2000B000
#define IRQ_PEND    0x200
#define IRQ_PEND1   0x204
#define IRQ_PEND2   0x208
#define IRQ_FIQ     0x20C
#define IRQ_EN1     0x210
#define IRQ_EN2     0x214
#define IRQ_EN      0x218
#define IRQ_DIS1    0x21C
#define IRQ_DIS2    0x220
#define IRQ_DIS     0x224

unsigned int num_ticks[4] = {0};

void timer_enable(int idx)
{
    printf("enable timer %d current LO:%x \n", idx, _REG(STIMER_BASE + STIMER_CLO));
    _REG(STIMER_BASE+STIMER_C0+4*idx) = _REG(STIMER_BASE + STIMER_CLO)+ 1000000;
    _REG(IRQ_BASE+IRQ_EN1) |= (1 << idx);
    printf("current LO:%x \n", _REG(STIMER_BASE + STIMER_CLO));
}

void timer_check(void)
{
    int stimer_cs = _REG(STIMER_BASE+STIMER_CS);
    for(int i = 0; i < 3; i++){
        if(stimer_cs & (1 << i)){
            printf("timer %d tick - %d\n", i, num_ticks[i]++);
            _REG(STIMER_BASE+STIMER_CS) |= (1 << i);
            _REG(STIMER_BASE+STIMER_C0+4*i) += 1000000;
        }
    }
}

void isr_entry(void)
{
    int base = 0;
    asm volatile ("mov %0, sp\n\t"
         : "=r" (base)
    );
    printf("%s %x\n", __func__, base);
    if(_REG(IRQ_BASE+IRQ_PEND1) & (0x0A))
        timer_check();
}
