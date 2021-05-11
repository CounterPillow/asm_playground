#include "asm.S"

/* extern int pg_find(int haystack[], int h_len, int needle); */
function find, export=1, align=4
    mov x3, #0
    /* early continue to one-by-one, no dup */
    cmp w1, #4                  /* if < 4 */
    b.lt one_loop               /* BRANCH */
    dup v0.4S, w2
four:
    sub w4, w1, w3
    cmp w4, #4                  /* if < 4 */
    b.lt one_loop               /* BRANCH */
    mov x5, x3, lsl #2          /* x5 = x3 << 2 */
    ldr q1, [x0, x5]            /* load 4 32-bit numbers into q1/v1 */
    cmeq v1.4S, v1.4S, v0.4S    /* where do they match? */
    addv s2, v1.4S              /* add all elements across */
    fmov w4, s2                 /* but we need this in an int reg */
    cbnz w4, one                /* BRANCH if it's nonzero, do it one by one */
    add w3, w3, #4              /* advance counter by 4 bytes */
    b four
one:
    cmp w1, w3
    b.eq finish_failure         /* BRANCH */
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
