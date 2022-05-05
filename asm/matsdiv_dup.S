#include "asm.S"

/* New version, duplicates s0 to all elements of v0.S4 and does vector division
 * Is simpler, but runs slower, and is almost exactly what gcc -O3 outputs
 */
function mat4x4sdiv_dup, export=1, align=4
    /* duplicate v0.4S[0] aka s0 to all elements of s, i.e. {s0, 0, 0, 0} -> {s0, s0, s0, s0} */
    dup v0.4S, v0.S[0]

    /* main calculation begins here.
     * We load four four-element 32-bit vectors from x0,
     * i.e. a whole ass 512 bits?
     */
    ld4 {v1.4S, v2.4S, v3.4S, v4.4S}, [x0]
    /* Divide each of those vectors by our s0 filled vector, meaning each element gets divided
     * by s0.
     */
    fdiv v1.4S, v1.4S, v0.4S
    fdiv v2.4S, v2.4S, v0.4S
    fdiv v3.4S, v3.4S, v0.4S
    fdiv v0.4S, v4.4S, v0.4S    /* we overwrite v0 here, improves performance */
    /* write them back to memory
     * if we didn't overwrite v0, we could've used
     *   st4 {v1.4S, v2.4S, v3.4S, v4.4S}, [x0]
     * but the instruction encoding doesn't allow v0 to be at the end here.
     * So instead, we use two _consecutive_ stp instructions, which get optimised
     * to a seemingly four wide store by the µarch.
     */
    stp q1, q2, [x0]
    stp q3, q0, [x0, #32]
    ret
endfunc
