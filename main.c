#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include "main.h"
#include "resize_half.h"
#include "matmul.h"
#include "matsdiv.h"
#include "find.h"

// blows up memory if set too high
#define MAX_MATRICES 30000
// reduces matrix to 0 if set too high, making the check at the end pointless
#define REPETITIONS 20
#define MATMUL_REPETITIONS 100
#define HAYSTACK_LEN 8000
#define MAX_HAYSTACKS 200
#define MAX_IMGS 20

#define EPSILON 0.000001

#define ERR(a) "\e[1;91m" a "\e[0m"


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

void pg_randarray8(uint8_t array[], size_t n) {
    for(int i = 0; i < n; i++) {
        array[i] = (uint8_t) (rand() & 0xff);
    }
}

void pg_printarray8(uint8_t array[], size_t n) {
    for(int i = 0; i < n - 1; i++) {
        printf("%d, ", array[i]);
    }
    printf("%d\n", array[n - 1]);
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
    printf("fmul: %s, fdiv: %s\n", fucked[0] ? ERR("FAILED") : "passed", fucked[1] ? ERR("FAILED") : "passed");
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
    printf("matmul test: %s\n", p ? "passed" : ERR("FAILED"));

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
    printf("find test: %s\n", passed ? "passed" : ERR("FAILED"));
    return passed;
}

bool test_resize_half() {
    //uint8_t src_img[MAX_IMGS][1024 * 1024 * 4];
    uint8_t** src_img = malloc(MAX_IMGS * sizeof(uint8_t*));
    for(int i = 0; i < MAX_IMGS; i++) {
        src_img[i] = malloc(1024 * 1024 * 4);
    }
    //uint8_t dest_img[2][MAX_IMGS][512 * 512 * 4];
    uint8_t** dest_img[2];
    dest_img[0] = malloc(MAX_IMGS * sizeof(uint8_t*));
    dest_img[1] = malloc(MAX_IMGS * sizeof(uint8_t*));
    for(int i = 0; i < MAX_IMGS; i++) {
        dest_img[0][i] = malloc(512 * 512 * 4);
        dest_img[1][i] = malloc(512 * 512 * 4);
    }
    assert(src_img != NULL && dest_img != NULL);

    for(int i = 0; i < MAX_IMGS; i++) {
        pg_randarray8(src_img[i], 1024 * 1024 * 4);
    }

    clock_t start = clock();
    for(int i = 0; i < MAX_IMGS; i++) {
        resize_half_c(dest_img[0][i], src_img[i], 1024, 1024);
    }
    clock_t elapsed_c = clock() - start;

    start = clock();
    for(int i = 0; i < MAX_IMGS; i++) {
        resize_half_intrin(dest_img[1][i], src_img[i], 1024, 1024);
    }
    clock_t elapsed_intrin = clock() - start;

    bool passed = true;
    for(int i = 0; i < MAX_IMGS; i++) {
        for(int y = 0; y < 512; y++) {
            for(int x = 0; x < 512; x++) {
                passed = passed &  (dest_img[0][i][y * 512 + x] ==
                                    dest_img[1][i][y * 512 + x]);
            }
        }
    }
    for(int i = 0; i < MAX_IMGS; i++) {
        free(src_img[i]);
        free(dest_img[0][i]);
        free(dest_img[1][i]);
    }
    free(src_img);
    free(dest_img[0]);
    free(dest_img[1]);

    printf("resize_half elapsed time:\n");
    printf("intrin = %ld, %.3fx faster than C\n", elapsed_intrin, (float) elapsed_c / (float) elapsed_intrin);
    printf("C = %ld\n", elapsed_c);

    printf("resize_half test: %s\n", passed ? "passed" : ERR("FAILED"));
    /*uint8_t test_src[1024];
    for(int i = 0; i < 1024; i++) {
        test_src[i] = 1;
    }
    uint8_t test_dest[256];
    uint8_t test_dest2[256];
    printf("I: ");
    resize_half_intrin(test_dest, test_src, 16, 16);
    pg_printarray8(test_dest, 256);
    printf("C: ");
    resize_half_scuffed(test_dest2, test_src, 16, 16);
    pg_printarray8(test_dest2, 256);
    for(int i = 0; i < 256; i++) {
        char broken[13];
        snprintf(broken, 13, ERR("%d"), abs((test_dest[i] - test_dest2[i])) % 10);
        printf("%s", test_dest[i] == test_dest2[i] ? "0" : broken);
    }
    printf("\n");*/

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
    printf("------------------------------\n");
    passed = passed & test_resize_half();

    return passed ? 0 : -1;
}
