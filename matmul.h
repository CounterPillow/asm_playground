#ifndef PG_MATMUL_H
#define PG_MATMUL_H

void pg_mat4x4mul_c(float A[4][4], float B[4][4], float res[4][4]);
extern void pg_mat4x4mul(float A[4][4], float B[4][4], float res[4][4]);

#endif
