
#include "printf.h"

void syscall_entry(int swi_num)
{
    int base = 0;
    asm volatile ("mov %0, sp\n\t"
         : "=r" (base)
    );
    printf("%s %x - swi %d\n", __func__, base, swi_num);
}
