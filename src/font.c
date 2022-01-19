/*******************************************************************************
 * Size: 32 px
 * Bpp: 1
 * Opts:
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include <lvgl.h>
#endif

#ifndef PIXELBASIC16
#define PIXELBASIC16 1
#endif

#if PIXELBASIC16

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
		/* U+0020 " " */
		0x0,

		/* U+0021 "!" */
		0xff, 0xff, 0xc,

		/* U+0022 "\"" */
		0xde, 0xf7, 0xbd, 0x80,

		/* U+0023 "#" */
		0x6c, 0x6c, 0x6c, 0xff, 0x6c, 0x6c, 0xff, 0x6c,
		0x6c, 0x6c, 0x6c,

		/* U+0025 "%" */
		0x20, 0x2c, 0xb, 0x63, 0x10, 0x80, 0x20, 0xc,
		0x6, 0x31, 0xa, 0x21, 0x28, 0x2b, 0x3, 0x0,

		/* U+0026 "&" */
		0x1c, 0x4, 0x80, 0x8c, 0xe, 0x2, 0xc0, 0x59,
		0xb0, 0xc6, 0x18, 0xc3, 0x28, 0xa8, 0xf3, 0x0,

		/* U+0027 "'" */
		0xff, 0xc0,

		/* U+0028 "(" */
		0x3b, 0x6d, 0xb6, 0xdb, 0x64, 0x40,

		/* U+0029 ")" */
		0xc4, 0x92, 0x49, 0x24, 0x95, 0x80,

		/* U+002A "*" */
		0xdb, 0x3c, 0x3c, 0xff, 0x3c, 0x3c, 0xdb,

		/* U+002B "+" */
		0x18, 0x18, 0x18, 0xff, 0x18, 0x18, 0x18, 0x18,

		/* U+002C "," */
		0x24, 0xac,

		/* U+002D "-" */
		0xfc,

		/* U+002E "." */
		0xc0,

		/* U+002F "/" */
		0x6, 0xc, 0x18, 0xc1, 0x83, 0x6, 0xc, 0x20,
		0x40, 0x86, 0xc, 0x18, 0x0,

		/* U+0030 "0" */
		0x3c, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
		0xc3, 0x44, 0x3c,

		/* U+0031 "1" */
		0x18, 0xcf, 0xb1, 0x8c, 0x63, 0x18, 0xc6,

		/* U+0032 "2" */
		0x39, 0x8c, 0x18, 0x30, 0x83, 0x8, 0x60, 0xc1,
		0x83, 0xf8,

		/* U+0033 "3" */
		0x39, 0x8c, 0x18, 0x30, 0x83, 0x1, 0x83, 0xc6,
		0x90, 0xe0,

		/* U+0034 "4" */
		0x6, 0x6, 0x1e, 0x26, 0x26, 0x26, 0xc6, 0xc6,
		0xff, 0x6, 0x6,

		/* U+0035 "5" */
		0xff, 0xc0, 0xc0, 0xfc, 0x3, 0x3, 0x3, 0x3,
		0xc3, 0x44, 0x3c,

		/* U+0036 "6" */
		0x3c, 0x44, 0xc3, 0xc0, 0xc0, 0xfc, 0xc3, 0xc3,
		0xc3, 0x44, 0x3c,

		/* U+0037 "7" */
		0xfe, 0xc, 0x18, 0x30, 0x61, 0xc, 0x18, 0x30,
		0x60, 0xc0,

		/* U+0038 "8" */
		0x39, 0x8f, 0x1e, 0x34, 0x87, 0x31, 0xe3, 0xc6,
		0x90, 0xe0,

		/* U+0039 "9" */
		0x3c, 0xc3, 0xc3, 0xc3, 0x43, 0x3f, 0x3, 0x3,
		0xc3, 0x44, 0x3c,

		/* U+003A ":" */
		0xc0, 0x3,

		/* U+003B ";" */
		0x20, 0x0, 0x9, 0x2b, 0x0,

		/* U+003C "<" */
		0xc, 0x42, 0x30, 0x40, 0x81, 0x3,

		/* U+003D "=" */
		0xfc, 0x0, 0x3f,

		/* U+003E ">" */
		0xc1, 0x2, 0x7, 0x10, 0x84, 0x30,

		/* U+003F "?" */
		0x3c, 0xc3, 0x3, 0x3, 0x4, 0x4, 0x18, 0x0,
		0x0, 0x0, 0x18,

		/* U+0040 "@" */
		0x1f, 0xe0, 0x40, 0x31, 0x0, 0x26, 0x3e, 0x3d,
		0x82, 0x7b, 0x4, 0xf6, 0x9, 0xec, 0x13, 0xd8,
		0x27, 0x90, 0xcf, 0x1f, 0x79, 0x0, 0x2, 0x0,
		0x3, 0xff, 0x0,

		/* U+0041 "A" */
		0x18, 0x28, 0x26, 0xc1, 0xc1, 0xc1, 0xff, 0xc1,
		0xc1, 0xc1, 0xc1,

		/* U+0042 "B" */
		0xfc, 0xc3, 0xc3, 0xc3, 0xc4, 0xfc, 0xc3, 0xc3,
		0xc3, 0xc4, 0xfc,

		/* U+0043 "C" */
		0x3c, 0x44, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
		0xc3, 0x44, 0x3c,

		/* U+0044 "D" */
		0xfc, 0x31, 0xc, 0x33, 0x3, 0xc0, 0xf0, 0x3c,
		0xf, 0x3, 0xc3, 0x31, 0xf, 0xc0,

		/* U+0045 "E" */
		0xff, 0x83, 0x6, 0xc, 0x1f, 0x30, 0x60, 0xc1,
		0x83, 0xf8,

		/* U+0046 "F" */
		0xff, 0x83, 0x6, 0xc, 0x1f, 0x30, 0x60, 0xc1,
		0x83, 0x0,

		/* U+0047 "G" */
		0x3c, 0xc3, 0xc0, 0xc0, 0xc0, 0xdf, 0xc3, 0xc3,
		0xc3, 0x44, 0x3c,

		/* U+0048 "H" */
		0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3,
		0xc3, 0xc3, 0xc3,

		/* U+0049 "I" */
		0xff, 0xff, 0xfc,

		/* U+004A "J" */
		0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0xc6,
		0x90, 0xe0,

		/* U+004B "K" */
		0xc1, 0xc2, 0xc6, 0xd8, 0xe0, 0xe0, 0xd8, 0xc8,
		0xc6, 0xc2, 0xc1,

		/* U+004C "L" */
		0xc1, 0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1,
		0x83, 0xf8,

		/* U+004D "M" */
		0xc0, 0x78, 0xf, 0x1, 0xf0, 0x7e, 0x17, 0xb2,
		0xf1, 0x9e, 0x3, 0xc0, 0x78, 0xf, 0x1, 0x80,

		/* U+004E "N" */
		0xc3, 0xc3, 0xc3, 0xe3, 0xe3, 0xdb, 0xc7, 0xc3,
		0xc3, 0xc3, 0xc3,

		/* U+004F "O" */
		0x1c, 0x9, 0x2, 0x33, 0x3, 0xc0, 0xf0, 0x3c,
		0xf, 0x3, 0x23, 0x9, 0x1, 0xc0,

		/* U+0050 "P" */
		0xfc, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xfc, 0xc0,
		0xc0, 0xc0, 0xc0,

		/* U+0051 "Q" */
		0x1c, 0x9, 0x2, 0x33, 0x3, 0xc0, 0xf0, 0x3c,
		0xf, 0x33, 0x23, 0x9, 0x41, 0xcc,

		/* U+0052 "R" */
		0xfe, 0xc1, 0xc1, 0xc1, 0xc2, 0xfe, 0xd8, 0xd8,
		0xc6, 0xc2, 0xc1,

		/* U+0053 "S" */
		0x3c, 0xc3, 0xc0, 0xc0, 0x40, 0x3c, 0x3, 0x3,
		0xc3, 0x44, 0x3c,

		/* U+0054 "T" */
		0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
		0x18, 0x18, 0x18,

		/* U+0055 "U" */
		0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
		0xc3, 0x42, 0x3e,

		/* U+0056 "V" */
		0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0xc1,
		0x26, 0x28, 0x18,

		/* U+0057 "W" */
		0xc0, 0x38, 0x7, 0x0, 0xe3, 0x1c, 0x63, 0x8c,
		0x71, 0x8e, 0x31, 0xd9, 0x2c, 0x28, 0x83, 0x0,

		/* U+0058 "X" */
		0xc1, 0xc1, 0xc1, 0x26, 0x28, 0x18, 0x26, 0x42,
		0xc1, 0xc1, 0xc1,

		/* U+0059 "Y" */
		0xc1, 0xc1, 0xc1, 0x26, 0x26, 0x26, 0x18, 0x18,
		0x18, 0x18, 0x18,

		/* U+005A "Z" */
		0xff, 0x1, 0x1, 0x6, 0x8, 0x18, 0x20, 0xc0,
		0xc0, 0xc0, 0xff,

		/* U+005B "[" */
		0xfb, 0x6d, 0xb6, 0xdb, 0x6d, 0xc0,

		/* U+005C "\\" */
		0xc1, 0x83, 0x1, 0x2, 0x4, 0x8, 0x10, 0x18,
		0x30, 0x60, 0x30, 0x60, 0xc0,

		/* U+005D "]" */
		0xf3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3f,

		/* U+005F "_" */
		0xfc,

		/* U+0061 "a" */
		0xf8, 0xc, 0x19, 0xfc, 0x78, 0xf1, 0xff,

		/* U+0062 "b" */
		0xc1, 0x83, 0x7, 0xcc, 0x98, 0xf1, 0xe3, 0xc7,
		0x8f, 0xe0,

		/* U+0063 "c" */
		0x3d, 0xc, 0x30, 0xc3, 0xc, 0xf,

		/* U+0064 "d" */
		0x6, 0xc, 0x19, 0xf4, 0x78, 0xf1, 0xe3, 0xc7,
		0x8c, 0xf8,

		/* U+0065 "e" */
		0x38, 0x93, 0x1f, 0xcc, 0x18, 0x30, 0x1f,

		/* U+0066 "f" */
		0x3e, 0x31, 0x8c, 0x7b, 0x18, 0xc6, 0x31, 0x8c,
		0x60,

		/* U+0067 "g" */
		0x39, 0x8f, 0x1e, 0x3c, 0x78, 0xd1, 0x9f, 0x6,
		0x13, 0xe0,

		/* U+0068 "h" */
		0xc0, 0xc0, 0xc0, 0xdc, 0xe4, 0xe3, 0xc3, 0xc3,
		0xc3, 0xc3, 0xc3,

		/* U+0069 "i" */
		0xc3, 0xff, 0xfc,

		/* U+006A "j" */
		0x20, 0x12, 0x49, 0x24, 0x95, 0x80,

		/* U+006B "k" */
		0xc1, 0x83, 0x6, 0x3c, 0x9b, 0x38, 0x70, 0xd9,
		0x93, 0x18,

		/* U+006C "l" */
		0xf3, 0x33, 0x33, 0x33, 0x33, 0x30,

		/* U+006D "m" */
		0xf9, 0xd9, 0x4b, 0x18, 0xe3, 0x1c, 0x63, 0x8c,
		0x71, 0x8e, 0x31,

		/* U+006E "n" */
		0xdc, 0xe4, 0xe3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,

		/* U+006F "o" */
		0x38, 0x93, 0x1e, 0x3c, 0x78, 0xf1, 0x9c,

		/* U+0070 "p" */
		0xf9, 0x8f, 0x1e, 0x3c, 0x78, 0xf2, 0x7c, 0xc1,
		0x83, 0x0,

		/* U+0071 "q" */
		0x3f, 0x8f, 0x1e, 0x3c, 0x78, 0xd1, 0x9f, 0x6,
		0xc, 0x18,

		/* U+0072 "r" */
		0xdf, 0x8e, 0x30, 0xc3, 0xc, 0x30,

		/* U+0073 "s" */
		0x3e, 0x83, 0x1, 0xc0, 0x80, 0xc2, 0x7c,

		/* U+0074 "t" */
		0xc6, 0x31, 0xfc, 0x63, 0x18, 0xc2, 0xe,

		/* U+0075 "u" */
		0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xd2, 0x1c,

		/* U+0076 "v" */
		0xc1, 0xc1, 0xc1, 0xc1, 0xc1, 0x26, 0x28, 0x18,

		/* U+0077 "w" */
		0xc0, 0x38, 0x7, 0x18, 0xe3, 0x1c, 0x63, 0xb2,
		0x58, 0x51, 0x6,

		/* U+0078 "x" */
		0xc1, 0x42, 0x26, 0x18, 0x28, 0x26, 0x42, 0xc1,

		/* U+0079 "y" */
		0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xd1, 0x9f, 0x6,
		0x13, 0xe0,

		/* U+007A "z" */
		0xfe, 0xc, 0x18, 0xc6, 0xc, 0x18, 0x7f,

		/* U+007B "{" */
		0xc, 0xc2, 0x8, 0x20, 0x8c, 0x10, 0x20, 0x82,
		0x8, 0x20, 0x70,

		/* U+007C "|" */
		0xff, 0xff, 0xff, 0xf0,

		/* U+007D "}" */
		0xc0, 0x82, 0x8, 0x20, 0x81, 0xc4, 0x20, 0x82,
		0x8, 0x23, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
		{.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
		{.bitmap_index = 0, .adv_w = 102, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
		{.bitmap_index = 1, .adv_w = 51, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 4, .adv_w = 102, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 6},
		{.bitmap_index = 8, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 19, .adv_w = 205, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 35, .adv_w = 205, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 51, .adv_w = 51, .box_w = 2, .box_h = 5, .ofs_x = 1, .ofs_y = 6},
		{.bitmap_index = 53, .adv_w = 77, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 59, .adv_w = 77, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 65, .adv_w = 154, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 4},
		{.bitmap_index = 72, .adv_w = 154, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 2},
		{.bitmap_index = 80, .adv_w = 77, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 82, .adv_w = 128, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = 5},
		{.bitmap_index = 83, .adv_w = 51, .box_w = 2, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 84, .adv_w = 128, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 97, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 108, .adv_w = 102, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 115, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 125, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 135, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 146, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 157, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 168, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
		{.bitmap_index = 178, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 188, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 199, .adv_w = 51, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 201, .adv_w = 77, .box_w = 3, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 206, .adv_w = 102, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 2},
		{.bitmap_index = 212, .adv_w = 128, .box_w = 6, .box_h = 4, .ofs_x = 1, .ofs_y = 4},
		{.bitmap_index = 215, .adv_w = 102, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 2},
		{.bitmap_index = 221, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 232, .adv_w = 256, .box_w = 15, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 259, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 270, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 281, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 292, .adv_w = 179, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 306, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 316, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 326, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 337, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 348, .adv_w = 51, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 351, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 361, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 372, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 382, .adv_w = 205, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 398, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 409, .adv_w = 179, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 423, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 434, .adv_w = 179, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 448, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 459, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 470, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 481, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 492, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 503, .adv_w = 205, .box_w = 11, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 519, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 530, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 541, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 552, .adv_w = 77, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 558, .adv_w = 128, .box_w = 7, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 571, .adv_w = 77, .box_w = 4, .box_h = 14, .ofs_x = 0, .ofs_y = -3},
		{.bitmap_index = 578, .adv_w = 128, .box_w = 6, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 579, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 586, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 596, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 602, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 612, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 619, .adv_w = 102, .box_w = 5, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 628, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 638, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 649, .adv_w = 51, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 652, .adv_w = 77, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 658, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 668, .adv_w = 77, .box_w = 4, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
		{.bitmap_index = 674, .adv_w = 205, .box_w = 11, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 685, .adv_w = 154, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 693, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 700, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 710, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 720, .adv_w = 128, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 726, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 733, .adv_w = 102, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 740, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 747, .adv_w = 154, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 755, .adv_w = 205, .box_w = 11, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 766, .adv_w = 154, .box_w = 8, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 774, .adv_w = 128, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 784, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
		{.bitmap_index = 791, .adv_w = 102, .box_w = 6, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 802, .adv_w = 51, .box_w = 2, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
		{.bitmap_index = 806, .adv_w = 102, .box_w = 6, .box_h = 14, .ofs_x = 1, .ofs_y = -3}
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
				-26, -26, -26, -26
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
const lv_font_t pixelbasic16 = {
#else
		lv_font_t pixelbasic16 = {
#endif
		.get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
		.get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
		.line_height = 14,          /*The maximum line height required by the font*/
		.base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
		.subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
		.underline_position = -3,
		.underline_thickness = 2,
#endif
		.dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if PIXELBASIC16*/

