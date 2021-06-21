#ifndef PG_MAIN_H
#define PG_MAIN_H

#include <stdint.h>

void pg_printmat(float mat[16]);
void pg_randmat(float mat[16]);
void pg_randarray(int array[], int n);
void pg_randarray8(uint8_t array[], size_t n);
void pg_printarray8(uint8_t array[], size_t n);

#endif
