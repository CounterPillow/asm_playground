C_OBJS = main.o resize_half.o matmul.o matsdiv.o find.o
ASM_OBJS = asm/asm.o asm/matsdiv.o asm/matsdiv_dup.o asm/matmul.o asm/find.o
OBJS = $(C_OBJS) $(ASM_OBJS)
CC ?= cc
CCAS ?= gcc
CFLAGS = -O3


playground: $(OBJS)
	$(CC) -g3 -o $@ $^

%.o: %.c
	$(CC) -g3 -c -o $@ $(CFLAGS) $^

%.o: %.S
	$(CCAS) -g -c -o $@ $^

clean:
	rm -vf $(C_OBJS) $(ASM_OBJS) playground

.PHONY: clean
