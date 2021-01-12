#include "printf.h"
#include "isr.h"

void bare_metal_start(void)
{
    int base = 0;
    asm volatile (
        "mov %0, sp\n\t" : "=r" (base)
    );

    printf("\n\n%s:%x: Hello World! %s %s %d\n\n", __func__, base, __DATE__, __TIME__, __LINE__);
    printf("enter busy loop\n");

    timer_enable(1);

    volatile int i = 0;
    while(1){
        if((i++ & 0x00FFFFFF) == 0)
            printf("%d\n", i);
    }
}
