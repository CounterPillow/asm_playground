#ifndef PG_MAIN_H
#define PG_MAIN_H
/* divides a 4x4 matrix of floats by the scalar scalar
 * in place, i.e. overwrites the input
 */
extern void pg_mat4x4sdiv(float mat[4][4], float scalar);
void pg_mat4x4sdiv_c(float mat[4][4], float scalar);
extern void pg_mat4x4sdiv_dup(float mat[4][4], float scalar);
#endif
