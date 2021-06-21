#include "mat_common.h"
/* multiplies two 4x4 matrices, saving the result in `res`. `res` MUST neither be A nor B */
void pg_mat4x4mul_c(float A[16], float B[16], float res[16]) {
    //assert(res != A && res != B);
    MAT4(res, 0, 0) = MAT4(A, 0, 0)*MAT4(B, 0, 0) + MAT4(A, 0, 1)*MAT4(B, 1, 0) + MAT4(A, 0, 2)*MAT4(B, 2, 0) + MAT4(A, 0, 3)*MAT4(B, 3, 0);
    MAT4(res, 0, 1) = MAT4(A, 0, 0)*MAT4(B, 0, 1) + MAT4(A, 0, 1)*MAT4(B, 1, 1) + MAT4(A, 0, 2)*MAT4(B, 2, 1) + MAT4(A, 0, 3)*MAT4(B, 3, 1);
    MAT4(res, 0, 2) = MAT4(A, 0, 0)*MAT4(B, 0, 2) + MAT4(A, 0, 1)*MAT4(B, 1, 2) + MAT4(A, 0, 2)*MAT4(B, 2, 2) + MAT4(A, 0, 3)*MAT4(B, 3, 2);
    MAT4(res, 0, 3) = MAT4(A, 0, 0)*MAT4(B, 0, 3) + MAT4(A, 0, 1)*MAT4(B, 1, 3) + MAT4(A, 0, 2)*MAT4(B, 2, 3) + MAT4(A, 0, 3)*MAT4(B, 3, 3);

    MAT4(res, 1, 0) = MAT4(A, 1, 0)*MAT4(B, 0, 0) + MAT4(A, 1, 1)*MAT4(B, 1, 0) + MAT4(A, 1, 2)*MAT4(B, 2, 0) + MAT4(A, 1, 3)*MAT4(B, 3, 0);
    MAT4(res, 1, 1) = MAT4(A, 1, 0)*MAT4(B, 0, 1) + MAT4(A, 1, 1)*MAT4(B, 1, 1) + MAT4(A, 1, 2)*MAT4(B, 2, 1) + MAT4(A, 1, 3)*MAT4(B, 3, 1);
    MAT4(res, 1, 2) = MAT4(A, 1, 0)*MAT4(B, 0, 2) + MAT4(A, 1, 1)*MAT4(B, 1, 2) + MAT4(A, 1, 2)*MAT4(B, 2, 2) + MAT4(A, 1, 3)*MAT4(B, 3, 2);
    MAT4(res, 1, 3) = MAT4(A, 1, 0)*MAT4(B, 0, 3) + MAT4(A, 1, 1)*MAT4(B, 1, 3) + MAT4(A, 1, 2)*MAT4(B, 2, 3) + MAT4(A, 1, 3)*MAT4(B, 3, 3);

    MAT4(res, 2, 0) = MAT4(A, 2, 0)*MAT4(B, 0, 0) + MAT4(A, 2, 1)*MAT4(B, 1, 0) + MAT4(A, 2, 2)*MAT4(B, 2, 0) + MAT4(A, 2, 3)*MAT4(B, 3, 0);
    MAT4(res, 2, 1) = MAT4(A, 2, 0)*MAT4(B, 0, 1) + MAT4(A, 2, 1)*MAT4(B, 1, 1) + MAT4(A, 2, 2)*MAT4(B, 2, 1) + MAT4(A, 2, 3)*MAT4(B, 3, 1);
    MAT4(res, 2, 2) = MAT4(A, 2, 0)*MAT4(B, 0, 2) + MAT4(A, 2, 1)*MAT4(B, 1, 2) + MAT4(A, 2, 2)*MAT4(B, 2, 2) + MAT4(A, 2, 3)*MAT4(B, 3, 2);
    MAT4(res, 2, 3) = MAT4(A, 2, 0)*MAT4(B, 0, 3) + MAT4(A, 2, 1)*MAT4(B, 1, 3) + MAT4(A, 2, 2)*MAT4(B, 2, 3) + MAT4(A, 2, 3)*MAT4(B, 3, 3);

    MAT4(res, 3, 0) = MAT4(A, 3, 0)*MAT4(B, 0, 0) + MAT4(A, 3, 1)*MAT4(B, 1, 0) + MAT4(A, 3, 2)*MAT4(B, 2, 0) + MAT4(A, 3, 3)*MAT4(B, 3, 0);
    MAT4(res, 3, 1) = MAT4(A, 3, 0)*MAT4(B, 0, 1) + MAT4(A, 3, 1)*MAT4(B, 1, 1) + MAT4(A, 3, 2)*MAT4(B, 2, 1) + MAT4(A, 3, 3)*MAT4(B, 3, 1);
    MAT4(res, 3, 2) = MAT4(A, 3, 0)*MAT4(B, 0, 2) + MAT4(A, 3, 1)*MAT4(B, 1, 2) + MAT4(A, 3, 2)*MAT4(B, 2, 2) + MAT4(A, 3, 3)*MAT4(B, 3, 2);
    MAT4(res, 3, 3) = MAT4(A, 3, 0)*MAT4(B, 0, 3) + MAT4(A, 3, 1)*MAT4(B, 1, 3) + MAT4(A, 3, 2)*MAT4(B, 2, 3) + MAT4(A, 3, 3)*MAT4(B, 3, 3);
}
