AS=arm-none-eabi-as
CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy

ARCH_FLAGS=-march=armv6

bare_metal.bin: bare_metal.elf
	$(OBJCOPY) -O binary $^ $@

bare_metal.elf: head.o bare_metal.o printf.o isr.o
	$(LD) $^ -T bare_metal.ld -o $@

head.o: head.S
	$(AS) $(ARCH_FLAGS) $^ -o $@

printf.o: printf.c
	$(CC) $(ARCH_FLAGS) -c $^

bare_metal.o: bare_metal.c
	$(CC) $(ARCH_FLAGS) -c $^

isr.o: isr.c
	$(CC) $(ARCH_FLAGS) -c $^

clean:
	rm -f *.o *.elf *.bin