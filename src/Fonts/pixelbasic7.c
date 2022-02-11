/*******************************************************************************
 * Size: 14 px
 * Bpp: 1
 * Opts:
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include <lvgl.h>
#endif

#ifndef PIXELBASIC_7
#define PIXELBASIC_7 1
#endif

#if PIXELBASIC_7

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
		/* U+0020 " " */
		0x0,

		/* U+0021 "!" */
		0xfa,

		/* U+0022 "\"" */
		0xb6, 0x80,

		/* U+0023 "#" */
		0x52, 0xbe, 0xaf, 0xa9, 0x40,

		/* U+0025 "%" */
		0x43, 0x49, 0x20, 0x82, 0x49, 0x61, 0x0,

		/* U+0026 "&" */
		0x30, 0x90, 0xc2, 0xa8, 0x91, 0x5d, 0x0,

		/* U+0027 "'" */
		0xe0,

		/* U+0028 "(" */
		0x6a, 0xaa, 0x40,

		/* U+0029 ")" */
		0x95, 0x55, 0x80,

		/* U+002A "*" */
		0xab, 0xbe, 0xea, 0x80,

		/* U+002B "+" */
		0x21, 0x3e, 0x42, 0x0,

		/* U+002C "," */
		0x58,

		/* U+002D "-" */
		0xf0,

		/* U+002E "." */
		0x80,

		/* U+002F "/" */
		0x11, 0x22, 0x24, 0x48, 0x80,

		/* U+0030 "0" */
		0x74, 0x63, 0x18, 0xc5, 0xc0,

		/* U+0031 "1" */
		0x2e, 0x92, 0x48,

		/* U+0032 "2" */
		0x69, 0x12, 0x48, 0xf0,

		/* U+0033 "3" */
		0x69, 0x12, 0x19, 0x60,

		/* U+0034 "4" */
		0x11, 0x94, 0xa9, 0x7c, 0x40,

		/* U+0035 "5" */
		0xfc, 0x3c, 0x10, 0xc5, 0xc0,

		/* U+0036 "6" */
		0x74, 0x61, 0xe8, 0xc5, 0xc0,

		/* U+0037 "7" */
		0xf1, 0x12, 0x44, 0x40,

		/* U+0038 "8" */
		0x69, 0x96, 0x99, 0x60,

		/* U+0039 "9" */
		0x74, 0x62, 0xf0, 0xc5, 0xc0,

		/* U+003A ":" */
		0x88,

		/* U+003B ";" */
		0x40, 0x58,

		/* U+003C "<" */
		0x2a, 0x22,

		/* U+003D "=" */
		0xf0, 0xf0,

		/* U+003E ">" */
		0x88, 0xa8,

		/* U+003F "?" */
		0x74, 0x42, 0x22, 0x0, 0x80,

		/* U+0040 "@" */
		0x3e, 0x20, 0xa7, 0x34, 0x5a, 0x2d, 0x16, 0x76,
		0x80, 0x3f, 0x0,

		/* U+0041 "A" */
		0x22, 0xa3, 0x1f, 0xc6, 0x20,

		/* U+0042 "B" */
		0xf4, 0x63, 0xe8, 0xc7, 0xc0,

		/* U+0043 "C" */
		0x74, 0x61, 0x8, 0x45, 0xc0,

		/* U+0044 "D" */
		0xf2, 0x28, 0x61, 0x86, 0x2f, 0x0,

		/* U+0045 "E" */
		0xf8, 0x8e, 0x88, 0xf0,

		/* U+0046 "F" */
		0xf8, 0x8e, 0x88, 0x80,

		/* U+0047 "G" */
		0x74, 0x61, 0x78, 0xc5, 0xc0,

		/* U+0048 "H" */
		0x8c, 0x63, 0xf8, 0xc6, 0x20,

		/* U+0049 "I" */
		0xfe,

		/* U+004A "J" */
		0x11, 0x11, 0x19, 0x60,

		/* U+004B "K" */
		0x8c, 0xa9, 0x8a, 0x4a, 0x20,

		/* U+004C "L" */
		0x88, 0x88, 0x88, 0xf0,

		/* U+004D "M" */
		0x83, 0x7, 0x1d, 0x59, 0x30, 0x60, 0x80,

		/* U+004E "N" */
		0x8c, 0x73, 0x59, 0xc6, 0x20,

		/* U+004F "O" */
		0x31, 0x28, 0x61, 0x85, 0x23, 0x0,

		/* U+0050 "P" */
		0xf4, 0x63, 0x1f, 0x42, 0x0,

		/* U+0051 "Q" */
		0x31, 0x28, 0x61, 0x95, 0x23, 0x40,

		/* U+0052 "R" */
		0xf4, 0x63, 0xea, 0x4a, 0x20,

		/* U+0053 "S" */
		0x74, 0x60, 0xe0, 0xc5, 0xc0,

		/* U+0054 "T" */
		0xf9, 0x8, 0x42, 0x10, 0x80,

		/* U+0055 "U" */
		0x8c, 0x63, 0x18, 0xc5, 0xc0,

		/* U+0056 "V" */
		0x8c, 0x63, 0x18, 0xa8, 0x80,

		/* U+0057 "W" */
		0x83, 0x6, 0x4c, 0x99, 0x35, 0x51, 0x0,

		/* U+0058 "X" */
		0x8c, 0x54, 0x45, 0x46, 0x20,

		/* U+0059 "Y" */
		0x8c, 0x54, 0xa2, 0x10, 0x80,

		/* U+005A "Z" */
		0xf8, 0x44, 0x44, 0x43, 0xe0,

		/* U+005B "[" */
		0xea, 0xaa, 0xc0,

		/* U+005C "\\" */
		0x88, 0x44, 0x42, 0x21, 0x10,

		/* U+005D "]" */
		0xd5, 0x55, 0xc0,

		/* U+005F "_" */
		0xf0,

		/* U+0061 "a" */
		0xe1, 0x79, 0xf0,

		/* U+0062 "b" */
		0x88, 0xe9, 0x99, 0xe0,

		/* U+0063 "c" */
		0x78, 0x88, 0x70,

		/* U+0064 "d" */
		0x11, 0x79, 0x99, 0x70,

		/* U+0065 "e" */
		0x69, 0xe8, 0x70,

		/* U+0066 "f" */
		0x72, 0x69, 0x24, 0x80,

		/* U+0067 "g" */
		0x69, 0x99, 0x71, 0xe0,

		/* U+0068 "h" */
		0x84, 0x2d, 0x98, 0xc6, 0x20,

		/* U+0069 "i" */
		0xbe,

		/* U+006A "j" */
		0x45, 0x55, 0x80,

		/* U+006B "k" */
		0x88, 0x9a, 0xca, 0x90,

		/* U+006C "l" */
		0xd5, 0x54,

		/* U+006D "m" */
		0xed, 0x26, 0x4c, 0x99, 0x20,

		/* U+006E "n" */
		0xb6, 0x63, 0x18, 0x80,

		/* U+006F "o" */
		0x69, 0x99, 0x60,

		/* U+0070 "p" */
		0xe9, 0x99, 0xe8, 0x80,

		/* U+0071 "q" */
		0x79, 0x99, 0x71, 0x10,

		/* U+0072 "r" */
		0xbc, 0x88, 0x80,

		/* U+0073 "s" */
		0x78, 0x61, 0xe0,

		/* U+0074 "t" */
		0x93, 0xc9, 0x18,

		/* U+0075 "u" */
		0x99, 0x99, 0x60,

		/* U+0076 "v" */
		0x8c, 0x62, 0xa2, 0x0,

		/* U+0077 "w" */
		0x83, 0x26, 0x4d, 0x54, 0x40,

		/* U+0078 "x" */
		0x8a, 0x88, 0xa8, 0x80,

		/* U+0079 "y" */
		0x99, 0x99, 0x71, 0xe0,

		/* U+007A "z" */
		0xf1, 0x24, 0xf0,

		/* U+007B "{" */
		0x29, 0x28, 0x92, 0x20,

		/* U+007C "|" */
		0xff, 0x80,

		/* U+007D "}" */
		0x89, 0x22, 0x92, 0x80
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
		{.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
		{.bitmap_index = 0, .adv_w = 67, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
		{.bitmap_index = 1, .adv_w = 34, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 2, .adv_w = 67, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 4},
		{.bitmap_index = 4, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 9, .adv_w = 134, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 16, .adv_w = 134, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 23, .adv_w = 34, .box_w = 1, .box_h = 3, .ofs_x = 1, .ofs_y = 4},
		{.bitmap_index = 24, .adv_w = 50, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 27, .adv_w = 50, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 30, .adv_w = 101, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 2},
		{.bitmap_index = 34, .adv_w = 101, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
		{.bitmap_index = 38, .adv_w = 50, .box_w = 2, .box_h = 3, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 39, .adv_w = 84, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
		{.bitmap_index = 40, .adv_w = 34, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 41, .adv_w = 84, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 46, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 51, .adv_w = 67, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 54, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 58, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 62, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 67, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 72, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 77, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 81, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 85, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 90, .adv_w = 34, .box_w = 1, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 91, .adv_w = 50, .box_w = 2, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 93, .adv_w = 67, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
		{.bitmap_index = 95, .adv_w = 84, .box_w = 4, .box_h = 3, .ofs_x = 1, .ofs_y = 2},
		{.bitmap_index = 97, .adv_w = 67, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = 1},
		{.bitmap_index = 99, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 104, .adv_w = 168, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 115, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 120, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 125, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 130, .adv_w = 118, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 136, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 140, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 144, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 149, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 154, .adv_w = 34, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 155, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 159, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 164, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 168, .adv_w = 134, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 175, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 180, .adv_w = 118, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 186, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 191, .adv_w = 118, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 197, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 202, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 207, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 212, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 217, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 222, .adv_w = 134, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 229, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 234, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 239, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 244, .adv_w = 50, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 247, .adv_w = 84, .box_w = 4, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 252, .adv_w = 50, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 255, .adv_w = 84, .box_w = 4, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 256, .adv_w = 84, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 259, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 263, .adv_w = 84, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 266, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 270, .adv_w = 84, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 273, .adv_w = 67, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 277, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 281, .adv_w = 101, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 286, .adv_w = 34, .box_w = 1, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 287, .adv_w = 50, .box_w = 2, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 290, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 294, .adv_w = 50, .box_w = 2, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 296, .adv_w = 134, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 301, .adv_w = 101, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 305, .adv_w = 84, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 308, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 312, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 316, .adv_w = 84, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 319, .adv_w = 84, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 322, .adv_w = 67, .box_w = 3, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 325, .adv_w = 84, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 328, .adv_w = 101, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 332, .adv_w = 134, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 337, .adv_w = 101, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 341, .adv_w = 84, .box_w = 4, .box_h = 7, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 345, .adv_w = 84, .box_w = 4, .box_h = 5, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 348, .adv_w = 67, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 352, .adv_w = 34, .box_w = 1, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
		{.bitmap_index = 354, .adv_w = 67, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = -2}
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
				-17, -17, -17, -17
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
const lv_font_t pixelbasic7 = {
#else
		lv_font_t pixelbasic7 = {
#endif
		.get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
		.get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
		.line_height = 9,          /*The maximum line height required by the font*/
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



#endif /*#if PIXELBASIC_7*/

