#include "asm.S"

/* extern int pg_find(int haystack[], int h_len, int needle); */
function find, export=1, align=4
    mov x3, #0
    /* early continue to one-by-one, no dup */
    cmp w1, #8                  /* if < 8 */
    b.lt one_loop               /* BRANCH */
    dup v0.4S, w2
eight:
    sub w4, w1, w3
    cmp w4, #8                  /* if < 8 */
    b.lt one_loop               /* BRANCH */
    lsl x5, x3, #2              /* x5 = x3 << 2 */
    ldr q1, [x0, x5]            /* load 4 32-bit numbers into q1/v1 */
    add x5, x5, #16
    ldr q2, [x0, x5]
    cmeq v1.4S, v1.4S, v0.4S    /* where do they match? */
    cmeq v2.4S, v2.4S, v0.4S    /* where do they match? */
    orr v1.16B, v1.16B, v2.16B
    mov x5, v1.d[0]             /* move 64 bit parts into int regs */
    mov x4, v1.d[1]
    orr x4, x4, x5              /* or them together */
    cbnz x4, one
    add w3, w3, #8              /* advance counter by 8 elements */
    b eight
one:
    cmp w1, w3
    b.lt finish_failure         /* BRANCH */
one_loop:
    ldr w4, [x0, x3, lsl #2]
    cmp w4, w2                  /* haystack[w3] == needle? */
    b.eq finish_success         /* BRANCH */
    add w3, w3, #1
    b one
finish_failure:
    mov x0, #-1
    ret
finish_success:
    mov x0, x3
    ret                         /* return value in x0 */
endfunc
