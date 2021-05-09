#include "asm.S"

/* Previous version, which used fmul with a reciprocal divisor
 * Faster than the new version
 */
function mat4x4sdiv, export=1, align=4
    /* calculate 1 / divisor, for later
     * do not use s1 or whatever here, we'd stomp on v1
     */
    fmov s8, #1.0
    fdiv s0, s8, s0

    /* main calculation begins here.
     * We load four four-element 32-bit vectors from x0,
     * i.e. a whole ass 512 bits?
     */
    ld4 {v1.4S, v2.4S, v3.4S, v4.4S}, [x0]
    /* multiply each of those with our inverted divisor
     * s0 is just an alias for the first 32-bit scalar of v0
     * .4S -> it's 4 32-bit scalars, [0] -> take the first
     * we can't use s0 here because... thanks assembler?
     */
    fmul v1.4S, v1.4S, v0.4S[0]
    fmul v2.4S, v2.4S, v0.4S[0]
    fmul v3.4S, v3.4S, v0.4S[0]
    fmul v4.4S, v4.4S, v0.4S[0]
    /* write them back to memory */
    st4 {v1.4S, v2.4S, v3.4S, v4.4S}, [x0]
    ret
endfunc
