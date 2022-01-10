/*******************************************************************************
 * Size: 24 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include <lvgl.h>
#endif

#ifndef PIXELBASIC24
#define PIXELBASIC24 1
#endif

#if PIXELBASIC24

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xfd,

    /* U+0022 "\"" */
    0x99, 0x90,

    /* U+0023 "#" */
    0x51, 0x4f, 0xd4, 0xfd, 0x45, 0x14,

    /* U+0025 "%" */
    0x41, 0xb2, 0x64, 0x8, 0x12, 0x26, 0x45, 0x82,

    /* U+0026 "&" */
    0x18, 0x64, 0x38, 0x4a, 0x8c, 0x84, 0x85, 0x7a,

    /* U+0027 "'" */
    0xe0,

    /* U+0028 "(" */
    0x6a, 0xaa, 0x90,

    /* U+0029 ")" */
    0x95, 0x55, 0x60,

    /* U+002A "*" */
    0x95, 0xef, 0xde, 0x94,

    /* U+002B "+" */
    0x10, 0x41, 0x3f, 0x10, 0x40,

    /* U+002C "," */
    0x58,

    /* U+002D "-" */
    0xf8,

    /* U+002E "." */
    0x80,

    /* U+002F "/" */
    0x8, 0x44, 0x21, 0x11, 0x8, 0x84, 0x0,

    /* U+0030 "0" */
    0x7a, 0x18, 0x61, 0x86, 0x18, 0x5e,

    /* U+0031 "1" */
    0x17, 0x91, 0x11, 0x11,

    /* U+0032 "2" */
    0x74, 0x42, 0x26, 0x42, 0x1f,

    /* U+0033 "3" */
    0x64, 0xc2, 0x61, 0x6, 0x2e,

    /* U+0034 "4" */
    0x8, 0xe4, 0x92, 0x8a, 0x2f, 0xc2,

    /* U+0035 "5" */
    0xfe, 0xf, 0x82, 0x4, 0x18, 0x5e,

    /* U+0036 "6" */
    0x7a, 0x18, 0x3e, 0x8a, 0x18, 0x5e,

    /* U+0037 "7" */
    0xf1, 0x12, 0x44, 0x44,

    /* U+0038 "8" */
    0x64, 0xe2, 0xe9, 0x46, 0x2e,

    /* U+0039 "9" */
    0x7a, 0x18, 0x5f, 0x4, 0x18, 0x5e,

    /* U+003A ":" */
    0x84,

    /* U+003B ";" */
    0x40, 0x16,

    /* U+003C "<" */
    0x2a, 0x22,

    /* U+003D "=" */
    0xf8, 0x3e,

    /* U+003E ">" */
    0x88, 0xa8,

    /* U+003F "?" */
    0x7a, 0x10, 0x42, 0x10, 0x0, 0x4,

    /* U+0040 "@" */
    0x7f, 0xd0, 0xa, 0x38, 0xc8, 0x99, 0x13, 0x22,
    0x64, 0xcc, 0x77, 0x40, 0x7, 0xf8,

    /* U+0041 "A" */
    0x11, 0xa8, 0x61, 0xfe, 0x18, 0x61,

    /* U+0042 "B" */
    0xfa, 0x18, 0x7e, 0x8a, 0x18, 0x7e,

    /* U+0043 "C" */
    0x7a, 0x18, 0x20, 0x82, 0x8, 0x5e,

    /* U+0044 "D" */
    0xf9, 0xa, 0xc, 0x18, 0x30, 0xa1, 0x7c,

    /* U+0045 "E" */
    0xfc, 0x21, 0xc8, 0x42, 0x1f,

    /* U+0046 "F" */
    0xfc, 0x21, 0xc8, 0x42, 0x10,

    /* U+0047 "G" */
    0x7a, 0x18, 0x2f, 0x86, 0x18, 0x5e,

    /* U+0048 "H" */
    0x86, 0x18, 0x7f, 0x86, 0x18, 0x61,

    /* U+0049 "I" */
    0xff,

    /* U+004A "J" */
    0x8, 0x42, 0x10, 0x86, 0x2e,

    /* U+004B "K" */
    0x86, 0x29, 0x38, 0x92, 0x48, 0xa1,

    /* U+004C "L" */
    0x84, 0x21, 0x8, 0x42, 0x1f,

    /* U+004D "M" */
    0x81, 0x81, 0xe3, 0x95, 0x89, 0x81, 0x81, 0x81,

    /* U+004E "N" */
    0x83, 0x7, 0x8c, 0x98, 0xf0, 0x60, 0xc1,

    /* U+004F "O" */
    0x18, 0xca, 0xc, 0x18, 0x30, 0x91, 0x1c,

    /* U+0050 "P" */
    0xfa, 0x18, 0x61, 0xfa, 0x8, 0x20,

    /* U+0051 "Q" */
    0x18, 0xca, 0xc, 0x18, 0xb0, 0x91, 0x1d,

    /* U+0052 "R" */
    0xfa, 0x18, 0x7e, 0x92, 0x48, 0xa1,

    /* U+0053 "S" */
    0x7a, 0x18, 0x1e, 0x8, 0x18, 0x5e,

    /* U+0054 "T" */
    0xfc, 0x41, 0x4, 0x10, 0x41, 0x4,

    /* U+0055 "U" */
    0x86, 0x18, 0x61, 0x86, 0x18, 0x5e,

    /* U+0056 "V" */
    0x86, 0x18, 0x61, 0x86, 0x14, 0x8c,

    /* U+0057 "W" */
    0x81, 0x81, 0x89, 0x89, 0x89, 0x99, 0x95, 0x62,

    /* U+0058 "X" */
    0x86, 0x14, 0x8c, 0x6a, 0x28, 0x61,

    /* U+0059 "Y" */
    0x86, 0x14, 0x92, 0x30, 0x41, 0x4,

    /* U+005A "Z" */
    0xfc, 0x10, 0x84, 0x62, 0x8, 0x3f,

    /* U+005B "[" */
    0xea, 0xaa, 0xb0,

    /* U+005C "\\" */
    0x84, 0x10, 0x84, 0x10, 0x42, 0x8, 0x40,

    /* U+005D "]" */
    0xd5, 0x55, 0x70,

    /* U+005F "_" */
    0xf8,

    /* U+0061 "a" */
    0xf0, 0x5f, 0x18, 0xfc,

    /* U+0062 "b" */
    0x84, 0x3d, 0x18, 0xc6, 0x5e,

    /* U+0063 "c" */
    0x7c, 0x21, 0x8, 0x3c,

    /* U+0064 "d" */
    0x8, 0x5f, 0x18, 0xc6, 0x2f,

    /* U+0065 "e" */
    0x64, 0xfd, 0x8, 0x3c,

    /* U+0066 "f" */
    0x72, 0x69, 0x24, 0x90,

    /* U+0067 "g" */
    0x64, 0xe3, 0x18, 0xbc, 0x7c,

    /* U+0068 "h" */
    0x81, 0x2, 0x67, 0x38, 0x30, 0x60, 0xc1,

    /* U+0069 "i" */
    0xbf,

    /* U+006A "j" */
    0x45, 0x55, 0x60,

    /* U+006B "k" */
    0x84, 0x23, 0x2e, 0x52, 0x51,

    /* U+006C "l" */
    0xd5, 0x55,

    /* U+006D "m" */
    0xf6, 0x89, 0x89, 0x89, 0x89, 0x89,

    /* U+006E "n" */
    0x99, 0xce, 0xc, 0x18, 0x30, 0x40,

    /* U+006F "o" */
    0x64, 0xe3, 0x19, 0x30,

    /* U+0070 "p" */
    0xf4, 0x63, 0x19, 0x7a, 0x10,

    /* U+0071 "q" */
    0x7c, 0x63, 0x18, 0xbc, 0x21,

    /* U+0072 "r" */
    0x9f, 0x21, 0x8, 0x40,

    /* U+0073 "s" */
    0x7c, 0x18, 0x20, 0xf8,

    /* U+0074 "t" */
    0x93, 0xc9, 0x23,

    /* U+0075 "u" */
    0x8c, 0x63, 0x18, 0xb8,

    /* U+0076 "v" */
    0x86, 0x18, 0x62, 0x48, 0xc0,

    /* U+0077 "w" */
    0x81, 0x89, 0x89, 0x99, 0x95, 0x62,

    /* U+0078 "x" */
    0x85, 0x23, 0xc, 0x4a, 0x10,

    /* U+0079 "y" */
    0x8c, 0x63, 0x18, 0xbc, 0x3e,

    /* U+007A "z" */
    0xf8, 0x44, 0x44, 0x7c,

    /* U+007B "{" */
    0x29, 0x28, 0x92, 0x44,

    /* U+007C "|" */
    0xff, 0xc0,

    /* U+007D "}" */
    0x89, 0x22, 0x92, 0x90
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 77, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 38, .box_w = 1, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2, .adv_w = 77, .box_w = 4, .box_h = 3, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 4, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 10, .adv_w = 154, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 18, .adv_w = 154, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 26, .adv_w = 38, .box_w = 1, .box_h = 3, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 27, .adv_w = 58, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 30, .adv_w = 58, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 33, .adv_w = 115, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 37, .adv_w = 115, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 42, .adv_w = 58, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 43, .adv_w = 96, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 44, .adv_w = 38, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 45, .adv_w = 96, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 52, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 58, .adv_w = 77, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 67, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 72, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 78, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 84, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 90, .adv_w = 96, .box_w = 4, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 94, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 99, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 105, .adv_w = 38, .box_w = 1, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 106, .adv_w = 58, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 108, .adv_w = 77, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 110, .adv_w = 96, .box_w = 5, .box_h = 3, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 112, .adv_w = 77, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 114, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 120, .adv_w = 192, .box_w = 11, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 134, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 140, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 146, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 152, .adv_w = 134, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 159, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 169, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 175, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 181, .adv_w = 38, .box_w = 1, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 182, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 187, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 198, .adv_w = 154, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 206, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 213, .adv_w = 134, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 220, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 226, .adv_w = 134, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 233, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 239, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 245, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 251, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 257, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 263, .adv_w = 154, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 271, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 277, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 283, .adv_w = 115, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 58, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 292, .adv_w = 96, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 299, .adv_w = 58, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 302, .adv_w = 96, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 303, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 307, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 312, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 316, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 321, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 325, .adv_w = 77, .box_w = 3, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 329, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 334, .adv_w = 115, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 341, .adv_w = 38, .box_w = 1, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 342, .adv_w = 58, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 345, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 350, .adv_w = 58, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 352, .adv_w = 154, .box_w = 8, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 358, .adv_w = 115, .box_w = 7, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 364, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 368, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 373, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 378, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 382, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 386, .adv_w = 77, .box_w = 3, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 389, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 393, .adv_w = 115, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 398, .adv_w = 154, .box_w = 8, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 404, .adv_w = 115, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 409, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 414, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 418, .adv_w = 77, .box_w = 3, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 422, .adv_w = 38, .box_w = 1, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 424, .adv_w = 77, .box_w = 3, .box_h = 10, .ofs_x = 1, .ofs_y = -2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 4, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 37, .range_length = 57, .glyph_id_start = 5,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 95, .range_length = 1, .glyph_id_start = 62,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 29, .glyph_id_start = 63,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    68, 63,
    68, 65,
    68, 66,
    68, 67
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    -19, -19, -19, -19
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 4,
    .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_pairs,
    .kern_scale = 16,
    .cmap_num = 4,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t pixelbasic24 = {
#else
lv_font_t pixelbasic24 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 10,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if PIXELBASIC24*/

