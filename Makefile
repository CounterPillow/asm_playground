C_OBJS = main.o
ASM_OBJS = asm.o matsdiv.o matsdiv_dup.o
USE_ASM ?= 0

CC ?= cc

ifeq ($(USE_ASM),1)
	OBJS = $(C_OBJS) $(ASM_OBJS)
	CFLAGS = -O3
else
	OBJS = $(C_OBJS)
	CFLAGS = -O3 -DNO_ASM
endif

playground: $(OBJS)
	$(CC) -g -o $@ $^

main.o:	main.c main.h
	$(CC) -g -c main.c $(CFLAGS)

%.o: %.S
	$(CC) -g -c -o $@ $^

clean:
	rm -vf $(C_OBJS) $(ASM_OBJS) playground

.PHONY: clean