C_OBJS = main.o
ASM_OBJS = asm.o matsdiv.o matsdiv_dup.o matmul.o
OBJS = $(C_OBJS) $(ASM_OBJS)
CC ?= cc
CFLAGS = -O2


playground: $(OBJS)
	$(CC) -g -o $@ $^

main.o:	main.c main.h
	$(CC) -g -c main.c $(CFLAGS)

%.o: %.S
	$(CC) -g -c -o $@ $^

clean:
	rm -vf $(C_OBJS) $(ASM_OBJS) playground

.PHONY: clean
