#include <stdio.h>
#include "main.h"


#if !defined(__aarch64__) || defined(NO_ASM)
void pg_mat4x4sdiv(float mat[4][4], float scalar) {
    for(int row = 0; row < 4; row++) {
        for(int col = 0; col < 4; col++) {
            mat[row][col] = mat[row][col] / scalar;
        }
    }
}
#endif

void pg_printmat(float mat[4][4])  {
    for(int row = 0; row < 4; row++) {
        printf("%f %f %f %f\n", mat[row][0], mat[row][1], mat[row][2], mat[row][3]);
    }
}

int main(int argc, char** argv) {
    float bar = 4.0;
    float foo = 1.0 / bar;
    float m[4][4] = {{1.1, 1.2, 1.3, 1.4},
                     {2.1, 2.2, 2.3, 2.4},
                     {3.1, 3.2, 3.3, 3.4},
                     {4.1, 4.2, 4.3, 4.4}};
    pg_mat4x4sdiv(m, 2.0);
    pg_printmat(m);
    return 0;
}
