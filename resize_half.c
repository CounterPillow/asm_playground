#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <arm_neon.h>
#include "resize_half.h"

void resize_half_c(uint8_t* dest_image, uint8_t* src_image, uint32_t src_width,
                   uint32_t src_height) {
    for(int y = 0; y < src_height; y += 2) {
        for(int x = 0; x < src_width; x += 2) {
            unsigned int r, g, b, a;
            uint8_t* topleft = src_image + src_width * y * 4 + x * 4;
            uint8_t* bottomleft = src_image + src_width * (y + 1) * 4 + x * 4;
            r = *topleft + *bottomleft + *(topleft + 4) + *(bottomleft + 4);
            g = *(topleft + 1) + *(bottomleft + 1) + *(topleft + 4 + 1) + *(bottomleft + 4 + 1);
            b = *(topleft + 2) + *(bottomleft + 2) + *(topleft + 4 + 2) + *(bottomleft + 4 + 2);
            a = *(topleft + 3) + *(bottomleft + 3) + *(topleft + 4 + 3) + *(bottomleft + 4 + 3);
            dest_image[0] = (uint8_t) (r >> 2);
            dest_image[1] = (uint8_t) (g >> 2);
            dest_image[2] = (uint8_t) (b >> 2);
            dest_image[3] = (uint8_t) (a >> 2);
            dest_image += 4;
        }
    }
}


/* this is the GPL version from overviewer which I don't even think is correct, because
 * it fucking reads two red components right next to each other but then writes out rgba ??? */
void resize_half_scuffed(uint8_t* dest_image, uint8_t* src_image, uint32_t src_width,
                   uint32_t src_height) {
     /* iteration variables */
     uint32_t x, y;
     /* temp color variables */
     uint32_t r, g, b, a;

     uint32_t dest_width = src_width / 2;
     uint32_t dest_height = src_height / 2;
     bool dest_has_alpha = true;;
     bool src_has_alpha = true;

     /* set to fully opaque if source has no alpha channel */
     if (!src_has_alpha)
         a = 0xFF << 2;

     for (y = 0; y < dest_height; y++) {

         uint8_t* out = (uint8_t*)(dest_image + y);
         uint8_t* in_row1 = (uint8_t*)(src_image + y * 2);
         uint8_t* in_row2 = (uint8_t*)(src_image + y * 2 + 1);

         for (x = 0; x < dest_width; x++) {

             // read first column
             r = *in_row1;
             r += *in_row2;
             in_row1++;
             in_row2++;
             g = *in_row1;
             g += *in_row2;
             in_row1++;
             in_row2++;
             b = *in_row1;
             b += *in_row2;
             in_row1++;
             in_row2++;

             if (src_has_alpha) {
                 a = *in_row1;
                 a += *in_row2;
                 in_row1++;
                 in_row2++;
             }

             // read second column
             r += *in_row1;
             r += *in_row2;
             in_row1++;
             in_row2++;
             g += *in_row1;
             g += *in_row2;
             in_row1++;
             in_row2++;
             b += *in_row1;
             b += *in_row2;
             in_row1++;
             in_row2++;

             if (src_has_alpha) {
                 a += *in_row1;
                 a += *in_row2;
                 in_row1++;
                 in_row2++;
             }

             // write blended color
             *out = (uint8_t)(r >> 2);
             out++;
             *out = (uint8_t)(g >> 2);
             out++;
             *out = (uint8_t)(b >> 2);
             out++;

             if (dest_has_alpha) {
                 *out = (uint8_t)(a >> 2);
                 out++;
             }
         }
     }
 }

void resize_half_intrin(uint8_t* dest_image, uint8_t* src_image, uint32_t src_width,
                        uint32_t src_height) {
    /* iteration variables */
    uint32_t x, y;

    for (y = 0; y < src_height; y += 2) {
        for (x = 0; x < src_width; x += 16) {

            uint8x16x4_t row1 = vld4q_u8(src_image + y * src_width * 4 + x * 4);
            uint8x16x4_t row2 = vld4q_u8(src_image + (y + 1) * src_width * 4 + x * 4);

            /*
             * | r0,0 | r1,0 | r2,0 | r3,0 | r4,0 | r5,0 | ...
             * | r0,1 | r1,1 | r2,1 | r3,1 | r4,1 | r5,1 | ...
             * =>
             * | (r0,0 + r0,1) / 2 | (r1,0 + r1,1) / 2 | (r2,0 + r2,1) / 2 | (r3,0 + r3,1) / 2 | ...
             */
            uint8x16_t p1 = vhaddq_u8(row1.val[0], row2.val[0]);    // r
            uint8x16_t p2 = vhaddq_u8(row1.val[1], row2.val[1]);    // g
            uint8x16_t p3 = vhaddq_u8(row1.val[2], row2.val[2]);    // b
            uint8x16_t p4 = vhaddq_u8(row1.val[3], row2.val[3]);    // a

            /* =>
             * | r0 | r2 | r4 | r6 | ... | g0 | g2 | ...
             * | r1 | r3 | r5 | r7 | ... | g1 | g3 | ...
             */
            uint8x16_t e1 = vuzp1q_u8(p1, p2);
            uint8x16_t o1 = vuzp2q_u8(p1, p2);
            /* =>
             * | (r0 + r1) / 2 | (r2 + r3) / 2 | (r4 + r5) / 2 | ...
             */
            uint8x16_t out1 = vhaddq_u8(e1, o1);
            uint64x2_t redgreen = vreinterpretq_u64_u8(out1);
            uint64_t red = vgetq_lane_u64(redgreen, 0);
            uint64_t green = vgetq_lane_u64(redgreen, 1);
            // blue and alpha
            uint8x16_t e2 = vuzp1q_u8(p3, p4);
            uint8x16_t o2 = vuzp2q_u8(p3, p4);
            uint8x16_t out2 = vhaddq_u8(e2, o2);
            uint64x2_t bluealpha = vreinterpretq_u64_u8(out2);
            uint64_t blue = vgetq_lane_u64(bluealpha, 0);
            uint64_t alpha = vgetq_lane_u64(bluealpha, 1);
            uint8x8x4_t storeme = {.val = {vcreate_u8(red), vcreate_u8(green), vcreate_u8(blue), vcreate_u8(alpha)}};
            vst4_u8(dest_image, storeme);
            dest_image = dest_image + 8 * 4;

        }
    }
}

void resize_half_intrin_new(uint8_t* dest_image, uint8_t* src_image, uint32_t src_width,
                        uint32_t src_height) {
    /* iteration variables */
    uint32_t x, y;

    for (y = 0; y < src_height; y += 2) {
        for (x = 0; x < src_width; x += 32) {
            /* Pairwise add and half the first row */
            uint8x16x4_t row1 = vld4q_u8(src_image + y * src_width * 4 + x * 4);
            uint16x8_t r1_h = vpaddlq_u8(row1.val[0]);
            uint16x8_t g1_h = vpaddlq_u8(row1.val[1]);
            uint16x8_t b1_h = vpaddlq_u8(row1.val[2]);
            uint16x8_t a1_h = vpaddlq_u8(row1.val[3]);
            uint8x8_t r1_h_half = vshrn_n_u16(r1_h, 1);
            uint8x8_t g1_h_half = vshrn_n_u16(g1_h, 1);
            uint8x8_t b1_h_half = vshrn_n_u16(b1_h, 1);
            uint8x8_t a1_h_half = vshrn_n_u16(a1_h, 1);

            /* Pairwise add and half the second row */
            uint8x16x4_t row2 = vld4q_u8(src_image + (y + 1) * src_width * 4 + x * 4);
            uint16x8_t r2_h = vpaddlq_u8(row2.val[0]);
            uint16x8_t g2_h = vpaddlq_u8(row2.val[1]);
            uint16x8_t b2_h = vpaddlq_u8(row2.val[2]);
            uint16x8_t a2_h = vpaddlq_u8(row2.val[3]);
            uint8x8_t r2_h_half = vshrn_n_u16(r2_h, 1);
            uint8x8_t g2_h_half = vshrn_n_u16(g2_h, 1);
            uint8x8_t b2_h_half = vshrn_n_u16(b2_h, 1);
            uint8x8_t a2_h_half = vshrn_n_u16(a2_h, 1);

            /* Add the rows together and half them */
            uint8x8_t r_quarter = vhadd_u8(r1_h_half, r2_h_half);
            uint8x8_t g_quarter = vhadd_u8(g1_h_half, g2_h_half);
            uint8x8_t b_quarter = vhadd_u8(b1_h_half, b2_h_half);
            uint8x8_t a_quarter = vhadd_u8(a1_h_half, a2_h_half);

            /* Same thing here but for a second set of 16x2 pixels */
            row1 = vld4q_u8(src_image + y * src_width * 4 + (x + 16) * 4);
            r1_h = vpaddlq_u8(row1.val[0]);
            g1_h = vpaddlq_u8(row1.val[1]);
            b1_h = vpaddlq_u8(row1.val[2]);
            a1_h = vpaddlq_u8(row1.val[3]);
            r1_h_half = vshrn_n_u16(r1_h, 1);
            g1_h_half = vshrn_n_u16(g1_h, 1);
            b1_h_half = vshrn_n_u16(b1_h, 1);
            a1_h_half = vshrn_n_u16(a1_h, 1);

            row2 = vld4q_u8(src_image + (y + 1) * src_width * 4 + (x + 16) * 4);
            r2_h = vpaddlq_u8(row2.val[0]);
            g2_h = vpaddlq_u8(row2.val[1]);
            b2_h = vpaddlq_u8(row2.val[2]);
            a2_h = vpaddlq_u8(row2.val[3]);
            r2_h_half = vshrn_n_u16(r2_h, 1);
            g2_h_half = vshrn_n_u16(g2_h, 1);
            b2_h_half = vshrn_n_u16(b2_h, 1);
            a2_h_half = vshrn_n_u16(a2_h, 1);

            uint8x8_t r_quarter_2 = vhadd_u8(r1_h_half, r2_h_half);
            uint8x8_t g_quarter_2 = vhadd_u8(g1_h_half, g2_h_half);
            uint8x8_t b_quarter_2 = vhadd_u8(b1_h_half, b2_h_half);
            uint8x8_t a_quarter_2 = vhadd_u8(a1_h_half, a2_h_half);

            /* Build the final vector for storing */
            uint8x16x4_t storeme = {
                .val = {
                        vcombine_u8(r_quarter, r_quarter_2),
                        vcombine_u8(g_quarter, g_quarter_2),
                        vcombine_u8(b_quarter, b_quarter_2),
                        vcombine_u8(a_quarter, a_quarter_2)
                }
            };
            vst4q_u8(dest_image, storeme);
            dest_image = dest_image + 16 * 4;
        }
    }
}
