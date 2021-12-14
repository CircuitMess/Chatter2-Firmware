#ifndef CHATTER_FIRMWARE_CHATTERTHEME_H
#define CHATTER_FIRMWARE_CHATTERTHEME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <core/lv_theme.h>
#include "core/lv_obj.h"

lv_theme_t* lv_theme_default_init(lv_disp_t* disp, lv_color_t color_primary, lv_color_t color_secondary, bool dark,
								  const lv_font_t* font);

lv_theme_t* lv_theme_default_get(void);

bool lv_theme_default_is_inited(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif //CHATTER_FIRMWARE_CHATTERTHEME_H

