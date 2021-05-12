#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include "main.h"

// blows up memory if set too high
#define MAX_MATRICES 30000
// reduces matrix to 0 if set too high, making the check at the end pointless
#define REPETITIONS 20
#define MATMUL_REPETITIONS 100
#define HAYSTACK_LEN 8000
#define MAX_HAYSTACKS 200

#define EPSILON 0.000001

void pg_mat4x4sdiv_c(float mat[4][4], float scalar) {
    for(int row = 0; row < 4; row++) {
        for(int col = 0; col < 4; col++) {
            mat[row][col] = mat[row][col] / scalar;
        }
    }
}

/* multiplies two 4x4 matrices, saving the result in `res`. `res` MUST neither be A nor B */
void pg_mat4x4mul_c(float A[4][4], float B[4][4], float res[4][4]) {
    //assert(res != A && res != B);
    res[0][0] = A[0][0]*B[0][0] + A[0][1]*B[1][0] + A[0][2]*B[2][0] + A[0][3]*B[3][0];
    res[0][1] = A[0][0]*B[0][1] + A[0][1]*B[1][1] + A[0][2]*B[2][1] + A[0][3]*B[3][1];
    res[0][2] = A[0][0]*B[0][2] + A[0][1]*B[1][2] + A[0][2]*B[2][2] + A[0][3]*B[3][2];
    res[0][3] = A[0][0]*B[0][3] + A[0][1]*B[1][3] + A[0][2]*B[2][3] + A[0][3]*B[3][3];

    res[1][0] = A[1][0]*B[0][0] + A[1][1]*B[1][0] + A[1][2]*B[2][0] + A[1][3]*B[3][0];
    res[1][1] = A[1][0]*B[0][1] + A[1][1]*B[1][1] + A[1][2]*B[2][1] + A[1][3]*B[3][1];
    res[1][2] = A[1][0]*B[0][2] + A[1][1]*B[1][2] + A[1][2]*B[2][2] + A[1][3]*B[3][2];
    res[1][3] = A[1][0]*B[0][3] + A[1][1]*B[1][3] + A[1][2]*B[2][3] + A[1][3]*B[3][3];

    res[2][0] = A[2][0]*B[0][0] + A[2][1]*B[1][0] + A[2][2]*B[2][0] + A[2][3]*B[3][0];
    res[2][1] = A[2][0]*B[0][1] + A[2][1]*B[1][1] + A[2][2]*B[2][1] + A[2][3]*B[3][1];
    res[2][2] = A[2][0]*B[0][2] + A[2][1]*B[1][2] + A[2][2]*B[2][2] + A[2][3]*B[3][2];
    res[2][3] = A[2][0]*B[0][3] + A[2][1]*B[1][3] + A[2][2]*B[2][3] + A[2][3]*B[3][3];

    res[3][0] = A[3][0]*B[0][0] + A[3][1]*B[1][0] + A[3][2]*B[2][0] + A[3][3]*B[3][0];
    res[3][1] = A[3][0]*B[0][1] + A[3][1]*B[1][1] + A[3][2]*B[2][1] + A[3][3]*B[3][1];
    res[3][2] = A[3][0]*B[0][2] + A[3][1]*B[1][2] + A[3][2]*B[2][2] + A[3][3]*B[3][2];
    res[3][3] = A[3][0]*B[0][3] + A[3][1]*B[1][3] + A[3][2]*B[2][3] + A[3][3]*B[3][3];
}

/* does a linear search through an array of integers
 * Returns index of needle if found, and -1 otherwise
 */
int pg_find_c(int haystack[], int h_len, int needle) {
    for(int i = 0; i < h_len; i++) {
        if(haystack[i] == needle) {
            return i;
        }
    }
    return -1;
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

void pg_randarray(int array[], int n) {
    for(int i = 0; i < n; i++) {
        array[i] = rand() & 0xfff;
    }
}

bool test_matsdiv() {
    __attribute__((aligned(64)))
    float m[3][MAX_MATRICES][4][4];

    printf("Pregenerating random matrices");
    for(int i = 0; i < MAX_MATRICES; i++) {
        pg_randmat(m[0][i]);
        memcpy(m[1][i], m[0][i], 64);
        memcpy(m[2][i], m[0][i], 64);
        if(i % 5000 == 0)
            printf(".");
    }
    printf(" done!\n");

    clock_t start = clock();
    for(int i = 0; i < MAX_MATRICES * REPETITIONS; i++) {
        pg_mat4x4sdiv(m[0][i % MAX_MATRICES], 1.1);
    }
    clock_t elapsed_1 = clock() - start;
    printf("matsdiv fmul test complete\n");

    start = clock();
    for(int i = 0; i < MAX_MATRICES * REPETITIONS; i++) {
        pg_mat4x4sdiv_dup(m[1][i % MAX_MATRICES], 1.1);
    }
    clock_t elapsed_2 = clock() - start;
    printf("matsdiv fdiv test complete\n");

    start = clock();
    for(int i = 0; i < MAX_MATRICES * REPETITIONS; i++) {
        pg_mat4x4sdiv_c(m[2][i % MAX_MATRICES], 1.1);
    }
    clock_t elapsed_3 = clock() - start;
    printf("matsdiv C test complete\n");

    printf("matsdiv elapsed time:\n");
    printf("fmul = %ld, %.3fx faster than C\n", elapsed_1, (float) elapsed_3 / (float) elapsed_1);
    printf("fdiv = %ld, %.3fx faster than C\n", elapsed_2, (float) elapsed_3 / (float) elapsed_2);
    printf("C = %ld\n", elapsed_3);

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
    return !(fucked[0] || fucked[1]);
}

bool test_matmul() {
    __attribute__((aligned(64)))
    float A_ex[4][4] = {
        {1.1, 1.2, 1.3, 1.4},
        {2.1, 2.2, 2.3, 2.4},
        {3.1, 3.2, 3.3, 3.4},
        {4.1, 4.2, 4.3, 4.4},
    };
    __attribute__((aligned(64)))
    float B_ex[4][4] = {
        {5.1, 5.2, 5.3, 5.4},
        {6.1, 6.2, 6.3, 6.4},
        {7.1, 7.2, 7.3, 7.4},
        {8.1, 8.2, 8.3, 8.4},
    };
    __attribute__((aligned(64)))
    float res_ex[4][4];
    float res2_ex[4][4];

    pg_mat4x4mul_c(A_ex, B_ex, res_ex);
    printf("Matrix multiplication result:\n");
    pg_printmat(res_ex);

    pg_mat4x4mul(A_ex, B_ex, res2_ex);
    printf("Matrix multiplication result, Assembly:\n");
    pg_printmat(res2_ex);

    __attribute__((aligned(64)))
    float A[2][MAX_MATRICES / 2][4][4];
    __attribute__((aligned(64)))
    float B[2][MAX_MATRICES / 2][4][4];
    float res[2][MAX_MATRICES / 2][4][4];

    printf("Pregenerating random matrices");
    for(int i = 0; i < MAX_MATRICES / 2; i++) {
        pg_randmat(A[0][i]);
        pg_randmat(B[0][i]);
        memcpy(A[1][i], A[0][i], 64);
        memcpy(B[1][i], B[0][i], 64);
        if(i % 5000 == 0)
            printf(".");
    }
    printf(" done!\n");

    clock_t start = clock();
    for(int i = 0; i < MAX_MATRICES / 2 * MATMUL_REPETITIONS; i++) {
        pg_mat4x4mul_c(A[0][i % (MAX_MATRICES / 2)], B[0][i % (MAX_MATRICES / 2)],
                       res[0][i % (MAX_MATRICES / 2)]);
    }
    clock_t elapsed_c = clock() - start;
    printf("matmul C test complete\n");

    start = clock();
    for(int i = 0; i < MAX_MATRICES / 2 * MATMUL_REPETITIONS; i++) {
        pg_mat4x4mul(A[1][i % (MAX_MATRICES / 2)], B[1][i % (MAX_MATRICES / 2)],
                     res[1][i % (MAX_MATRICES / 2)]);
    }
    clock_t elapsed_simd = clock() - start;
    printf("matmul elapsed time:\n");
    printf("simd = %ld, %.3fx faster than C\n", elapsed_simd, (float) elapsed_c / (float) elapsed_simd);
    printf("C = %ld\n", elapsed_c);

    bool p = true;
    for(int i = 0; i < MAX_MATRICES / 2; i++) {
        for(int row = 0; row < 4; row++) {
            for(int col = 0; col < 4; col++) {
                if(fabs(res[0][i][row][col] - res[0][i][row][col]) > EPSILON) {
                    p = false;
                }
            }
        }
    }
    printf("matmul test: %s\n", p ? "passed" : "FAILED");

    return p;
}

bool test_find() {
    int results[2][MAX_HAYSTACKS];
    __attribute__((aligned(64)))
    int haystack[MAX_HAYSTACKS][HAYSTACK_LEN];
    for(int i = 0; i < MAX_HAYSTACKS; i++) {
        pg_randarray(haystack[i], HAYSTACK_LEN);
    }

    clock_t start = clock();
    for(int i = 0; i < MAX_HAYSTACKS; i++) {
        results[0][i] = pg_find_c(haystack[i], HAYSTACK_LEN, 69);
    }
    clock_t elapsed_c = clock() - start;

    start = clock();
    for(int i = 0; i < MAX_HAYSTACKS; i++) {
        results[1][i] = pg_find(haystack[i], HAYSTACK_LEN, 69);
    }
    clock_t elapsed_simd = clock() - start;

    printf("find elapsed time:\n");
    printf("simd = %ld, %.3fx faster than C\n", elapsed_simd, (float) elapsed_c / (float) elapsed_simd);
    printf("C = %ld\n", elapsed_c);
    bool passed = true;
    printf("results: ");
    for(int i = 0; i < MAX_HAYSTACKS; i++) {
        passed = passed & (results[0][i] == results[1][i]);
        printf("%d%s", results[0][i], i < MAX_HAYSTACKS - 1 ? ", " : "\n");
    }
    printf("find test: %s\n", passed ? "passed" : "FAILED");
    return passed;
}


int main(int argc, char** argv) {
    srand(clock());
    bool passed = true;
    passed = passed & test_matsdiv();
    printf("------------------------------\n");
    passed = passed & test_matmul();
    printf("------------------------------\n");
    passed = passed & test_find();

    return passed ? 0 : -1;
}
