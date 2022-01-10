/*******************************************************************************
 * Size: 24 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef PIXELNICKNAME24
#define PIXELNICKNAME24 1
#endif

#if PIXELNICKNAME24

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0041 "A" */
    0x3c, 0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xff, 0xff,
    0xc3, 0xc3, 0xc3,

    /* U+0042 "B" */
    0xfd, 0xfb, 0x1e, 0x3f, 0xff, 0xb1, 0xe3, 0xc7,
    0xfb, 0xf0,

    /* U+0043 "C" */
    0x3c, 0x3c, 0x66, 0xc3, 0xc3, 0xc0, 0xc3, 0xc3,
    0x66, 0x7e, 0x3c,

    /* U+0044 "D" */
    0xfc, 0xfc, 0xc6, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc7, 0xfe, 0xfc,

    /* U+0045 "E" */
    0xff, 0xfc, 0x30, 0xc3, 0xef, 0xb0, 0xc3, 0xff,
    0xc0,

    /* U+0046 "F" */
    0xff, 0xfc, 0x30, 0xc3, 0xef, 0xb0, 0xc3, 0xc,
    0x0,

    /* U+0047 "G" */
    0x3c, 0x1e, 0x1d, 0x9e, 0xfc, 0x6, 0x3f, 0x7,
    0x83, 0xf7, 0xbf, 0xf, 0x0,

    /* U+0048 "H" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xff, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3,

    /* U+0049 "I" */
    0xff, 0xff, 0xfc,

    /* U+004A "J" */
    0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0xe3, 0xc6,
    0xb9, 0xf0,

    /* U+004B "K" */
    0xc3, 0xc3, 0xcc, 0xfc, 0xfc, 0xf0, 0xfc, 0xfc,
    0xfc, 0xc3, 0xc3,

    /* U+004C "L" */
    0xc3, 0xc, 0x30, 0xc3, 0xc, 0x30, 0xc3, 0xff,
    0xc0,

    /* U+004D "M" */
    0xc3, 0xc3, 0xc3, 0xf7, 0xff, 0xcb, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3,

    /* U+004E "N" */
    0xc3, 0xc3, 0xc3, 0xe3, 0xfb, 0xfb, 0xdf, 0xdf,
    0xdf, 0xc3, 0xc3,

    /* U+004F "O" */
    0x3c, 0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xe7, 0x7e, 0x3c,

    /* U+0050 "P" */
    0xfd, 0xff, 0x1e, 0x3c, 0x7f, 0xff, 0x60, 0xc1,
    0x83, 0x0,

    /* U+0051 "Q" */
    0x3c, 0x3c, 0x76, 0xc3, 0xc3, 0xc3, 0xcb, 0xc3,
    0xc7, 0x7e, 0x3d,

    /* U+0052 "R" */
    0xfd, 0xfb, 0x1e, 0x3d, 0xff, 0xbe, 0x7c, 0xdd,
    0xbf, 0x18,

    /* U+0053 "S" */
    0x3e, 0x7f, 0x70, 0xf0, 0xfe, 0x7f, 0x3, 0x3,
    0xc7, 0xff, 0x7e,

    /* U+0054 "T" */
    0xff, 0xfc, 0x60, 0xc1, 0x83, 0x6, 0xc, 0x18,
    0x30, 0x60,

    /* U+0055 "U" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xe7, 0x7e, 0x7e,

    /* U+0056 "V" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0x66, 0x3c, 0x3c,

    /* U+0057 "W" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xcb, 0xff, 0xf7,
    0xc3, 0xc3, 0xc3,

    /* U+0058 "X" */
    0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x3c, 0x3c, 0x66,
    0xe7, 0xc3, 0xc3,

    /* U+0059 "Y" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x7f, 0xdf, 0xc, 0x18,
    0x30, 0x60,

    /* U+005A "Z" */
    0xff, 0xfc, 0x18, 0x30, 0xe7, 0x9e, 0x38, 0xf1,
    0xff, 0xf8
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 77, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 12, .adv_w = 134, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 22, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 33, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 44, .adv_w = 115, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 53, .adv_w = 115, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 154, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 75, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 86, .adv_w = 58, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 89, .adv_w = 134, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 99, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 110, .adv_w = 115, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 119, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 130, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 141, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 152, .adv_w = 134, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 162, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 173, .adv_w = 134, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 183, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 194, .adv_w = 134, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 204, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 215, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 226, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 237, .adv_w = 154, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 248, .adv_w = 135, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 258, .adv_w = 134, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 1, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 2,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
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
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
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
const lv_font_t pixelnickname24 = {
#else
lv_font_t pixelnickname24 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 11,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if PIXELNICKNAME24*/

