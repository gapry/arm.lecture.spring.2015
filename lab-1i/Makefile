CC	= arm-linux-gnueabihf-gcc
CFLAGS	= -O0 -Wall -g -lrt
LDFLAGS = -fno-stack-protector

objects	= fibseq.o fib.o

default: fibseq

.PHONY: default clean qemu

fibseq: $(objects)
	$(CC) $(LDFLAGS) -o $@ $^

fib.o: fib.s
fibseq.o: fibseq.c

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.s
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(objects) fibseq

DBG_SER:
	qemu-arm -L /usr/arm-linux-gnueabihf -g 1234 fibseq

DBG_Cli:
	arm-none-eabi-gdb -x gdbscript.txt fibseq
