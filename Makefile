C_OBJS = main.o resize_half.o
ASM_OBJS = asm.o matsdiv.o matsdiv_dup.o matmul.o find.o
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
