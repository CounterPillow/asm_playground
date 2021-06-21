#include "mat_common.h"

void pg_mat4x4sdiv_c(float mat[16], float scalar) {
    for(int row = 0; row < 4; row++) {
        for(int col = 0; col < 4; col++) {
            MAT4(mat, row, col) = MAT4(mat, row, col) / scalar;
        }
    }
}
