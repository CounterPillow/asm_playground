#include "asm.S"

/* extern void pg_mat4x4mul(float A[4][4], float B[4][4], float res[4][4]) */
function mat4x4mul, export=1, align=4
    /* zero our result scratch registers */
    movi v5.4S, 0
    movi v6.4S, 0
    movi v7.4S, 0
    movi v24.4S, 0  /* d8 aka v8's lower half needs to be preserved, d24
                     * is scratch space though. */

    /* The reason why the loads here are weird is because
     * My ARM SBC is in-order, so making sure it has something to
     * do inbetween loads means the pipeline presumably stalls less.
     * i.e., and yes I tested this, it's faster.
     */

    ld1 {v0.4S}, [x0], #16      /* load A[0] */

    ld1 {v4.4S}, [x1], #16      /* load B[0] */
    fmla v5.4S, v4.4S, v0.4S[0]
    ld1 {v1.4S}, [x0], #16      /* load A[1] */
    fmla v6.4S, v4.4S, v1.4S[0]
    ld1 {v2.4S}, [x0], #16      /* load A[2] */
    fmla v7.4S, v4.4S, v2.4S[0]
    ld1 {v3.4S}, [x0]           /* load A[3] */
    fmla v24.4S, v4.4S, v3.4S[0]
    ld1 {v4.4S}, [x1], #16      /* load B[2] */
    fmla v5.4S, v4.4S, v0.4S[1]
    fmla v6.4S, v4.4S, v1.4S[1]
    fmla v7.4S, v4.4S, v2.4S[1]
    fmla v24.4S, v4.4S, v3.4S[1]
    ld1 {v4.4S}, [x1], #16      /* load B[3] */
    fmla v5.4S, v4.4S, v0.4S[2]
    fmla v6.4S, v4.4S, v1.4S[2]
    fmla v7.4S, v4.4S, v2.4S[2]
    fmla v24.4S, v4.4S, v3.4S[2]
    ld1 {v4.4S}, [x1]           /* load B[4] */
    fmla v5.4S, v4.4S, v0.4S[3]
    fmla v6.4S, v4.4S, v1.4S[3]
    fmla v7.4S, v4.4S, v2.4S[3]
    fmla v24.4S, v4.4S, v3.4S[3]

    /* store the result in the location pointed at by the third parameter */
    st1 {v5.4S}, [x2], #16
    st1 {v6.4S}, [x2], #16
    st1 {v7.4S}, [x2], #16
    st1 {v24.4S}, [x2]
    ret
endfunc
