#ifndef PG_RESIZE_HALF_H
#define PG_RESIZE_HALF_H
#include <stdint.h>
#include <stdbool.h>

void resize_half_c(uint8_t* dest_image, uint8_t* src_image, uint32_t src_width,
                   uint32_t src_height);

void resize_half_scuffed(uint8_t* dest_image, uint8_t* src_image, uint32_t src_width,
                        uint32_t src_height);

void resize_half_intrin(uint8_t* dest_image, uint8_t* src_image, uint32_t src_width,
                        uint32_t src_height);
void resize_half_intrin_new(uint8_t* dest_image, uint8_t* src_image, uint32_t src_width,
                            uint32_t src_height);

#endif
