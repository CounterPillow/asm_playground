#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "main.h"

// blows up memory if set too high
#define MAX_MATRICES 30000
// reduces matrix to 0 if set too high, making the check at the end pointless
#define REPETITIONS 20

#define EPSILON 0.000001

void pg_mat4x4sdiv_c(float mat[4][4], float scalar) {
    for(int row = 0; row < 4; row++) {
        for(int col = 0; col < 4; col++) {
            mat[row][col] = mat[row][col] / scalar;
        }
    }
}

void pg_printmat(float mat[4][4])  {
    for(int row = 0; row < 4; row++) {
        printf("%f %f %f %f\n", mat[row][0], mat[row][1], mat[row][2], mat[row][3]);
    }
}

void pg_randmat(float mat[4][4]) {
    for(int row = 0; row < 4; row++) {
        for(int col = 0; col < 4; col++) {
            mat[row][col] = (float) rand() / (float) RAND_MAX;
        }
    }
}

int main(int argc, char** argv) {

    __attribute__((aligned(64)))
    float m[3][MAX_MATRICES][4][4];

    printf("Pregenerating random matrices");
    srand(clock());
    for(int i = 0; i < MAX_MATRICES; i++) {
        pg_randmat(m[0][i]);
        memcpy(m[1][i], m[0][i], 64);
        memcpy(m[2][i], m[0][i], 64);
        if(i % 5000 == 0)
            printf(".");
    }
    printf("done!\n");

    clock_t start = clock();
    for(int i = 0; i < MAX_MATRICES * REPETITIONS; i++) {
        pg_mat4x4sdiv(m[0][i % MAX_MATRICES], 1.1);
    }
    clock_t elapsed_1 = clock() - start;
    printf("fmul test complete\n");

    start = clock();
    for(int i = 0; i < MAX_MATRICES * REPETITIONS; i++) {
        pg_mat4x4sdiv_dup(m[1][i % MAX_MATRICES], 1.1);
    }
    clock_t elapsed_2 = clock() - start;
    printf("fdiv test complete\n");

    start = clock();
    for(int i = 0; i < MAX_MATRICES * REPETITIONS; i++) {
        pg_mat4x4sdiv_c(m[2][i % MAX_MATRICES], 1.1);
    }
    clock_t elapsed_3 = clock() - start;
    printf("c test complete\n");

    printf("Elapsed time:\n");
    printf("fmul = %d, %.3fx faster than C\n", elapsed_1, (float) elapsed_3 / (float) elapsed_1);
    printf("fdiv = %d, %.3fx faster than C\n", elapsed_2, (float) elapsed_3 / (float) elapsed_2);
    printf("c = %d\n", elapsed_3);

    bool fucked[2] = {false, false};
    for(int i = 0; i < MAX_MATRICES; i++) {
        for(int j = 0; j < 2; j++) {
            for(int row = 0; row < 4; row++) {
                for(int col = 0; col < 4; col++) {
                    if(fabs(m[j][i][row][col] - m[2][i][row][col]) > EPSILON) {
                        fucked[j] = true;
                    }
                }
            }
        }
    }
    printf("fmul: %s, fdiv: %s\n", fucked[0] ? "FAILED" : "passed", fucked[1] ? "FAILED" : "passed");

    return 0;
}
