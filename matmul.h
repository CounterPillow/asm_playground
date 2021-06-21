#ifndef PG_MATMUL_H
#define PG_MATMUL_H

void pg_mat4x4mul_c(float A[16], float B[16], float res[16]);
extern void pg_mat4x4mul(float A[16], float B[16], float res[16]);

#endif
