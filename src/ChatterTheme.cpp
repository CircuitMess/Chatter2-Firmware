#include <core/lv_disp.h>
#include <misc/lv_gc.h>
#include <extra/widgets/msgbox/lv_msgbox.h>
#include "ChatterTheme.h"

#define MODE_DARK 0

#define LIGHT_COLOR_TEXT       lv_palette_darken(LV_PALETTE_GREY, 4)
#define DARK_COLOR_TEXT        lv_palette_lighten(LV_PALETTE_GREY, 5)

#define OUTLINE_WIDTH           lv_disp_dpx(theme.disp, 3)

typedef struct {
	lv_style_t scr;
	lv_style_t scrollbar;
	lv_style_t scrollbar_scrolled;
	lv_style_t card;
	lv_style_t btn;

	/*Utility*/
	lv_style_t bg_color_primary;
	lv_style_t bg_color_primary_muted;
	lv_style_t bg_color_secondary;
	lv_style_t bg_color_secondary_muted;
	lv_style_t bg_color_grey;
	lv_style_t bg_color_white;
	lv_style_t pressed;
	lv_style_t disabled;
	lv_style_t pad_zero;
	lv_style_t pad_tiny;
	lv_style_t pad_small;
	lv_style_t pad_normal;
	lv_style_t pad_gap;
	lv_style_t line_space_large;
	lv_style_t text_align_center;
	lv_style_t outline_primary;
	lv_style_t outline_secondary;
	lv_style_t circle;
	lv_style_t no_radius;
	lv_style_t clip_corner;
	lv_style_t grow;
	lv_style_t transition_delayed;
	lv_style_t transition_normal;
	lv_style_t anim;
	lv_style_t anim_fast;
	/*Parts*/
	lv_style_t knob;
	lv_style_t indic;

	lv_style_t arc_indic;
	lv_style_t arc_indic_primary;

	lv_style_t chart_series, chart_indic, chart_ticks, chart_bg;

	lv_style_t dropdown_list;

	lv_style_t cb_marker, cb_marker_checked;

	lv_style_t switch_knob;

	lv_style_t line;

	lv_style_t table_cell;

	lv_style_t meter_marker, meter_indic;

	lv_style_t ta_cursor, ta_placeholder;

	lv_style_t calendar_btnm_bg, calendar_btnm_day, calendar_header;

	lv_style_t colorwheel_main;

	lv_style_t msgbox_bg, msgbox_btn_bg, msgbox_backdrop_bg;

	lv_style_t keyboard_btn_bg;

	lv_style_t list_bg, list_btn, list_item_grow, list_label;

	lv_style_t tab_bg_focus, tab_btn;

	lv_style_t led;
} my_theme_styles_t;

typedef struct {
	lv_theme_t base;
	uint8_t light: 1;
} my_theme_t;

typedef enum {
	DISP_SMALL = 3,
	DISP_MEDIUM = 2,
	DISP_LARGE = 1,
} disp_size_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void theme_apply(lv_theme_t* th, lv_obj_t* obj);

static void style_init_reset(lv_style_t* style);

/**********************
 *  STATIC VARIABLES
 **********************/
static my_theme_styles_t* styles;
static lv_theme_t theme;
static disp_size_t disp_size;
static bool inited;
static lv_color_t color_scr;
static lv_color_t color_text;
static lv_color_t color_card;
static lv_color_t color_grey;


/**********************
 *      MACROS
 **********************/

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void style_init(void){
	color_text = theme.flags & MODE_DARK ? DARK_COLOR_TEXT : LIGHT_COLOR_TEXT;

	style_init_reset(&styles->scrollbar);
	lv_style_set_bg_color(&styles->scrollbar, (theme.flags & MODE_DARK) ? lv_palette_darken(LV_PALETTE_GREY, 2) : lv_palette_main(LV_PALETTE_GREY));

	lv_style_set_radius(&styles->scrollbar, LV_RADIUS_CIRCLE);
	lv_style_set_pad_right(&styles->scrollbar, lv_disp_dpx(theme.disp, 7));
	lv_style_set_pad_top(&styles->scrollbar, lv_disp_dpx(theme.disp, 7));
	lv_style_set_size(&styles->scrollbar, lv_disp_dpx(theme.disp, 5));
	lv_style_set_bg_opa(&styles->scrollbar, LV_OPA_40);

	style_init_reset(&styles->card);
	lv_style_set_bg_opa(&styles->card, LV_OPA_COVER);
	lv_style_set_bg_color(&styles->card, color_card);
	lv_style_set_border_color(&styles->card, color_grey);
	lv_style_set_text_color(&styles->card, color_text);
	lv_style_set_line_width(&styles->card, lv_disp_dpx(theme.disp, 1));

	style_init_reset(&styles->ta_cursor);
	lv_style_set_border_color(&styles->ta_cursor, color_text);
	lv_style_set_border_width(&styles->ta_cursor, lv_disp_dpx(theme.disp, 1));
	lv_style_set_pad_left(&styles->ta_cursor, -lv_disp_dpx(theme.disp, 1));
	lv_style_set_border_side(&styles->ta_cursor, LV_BORDER_SIDE_LEFT);
	lv_style_set_anim_time(&styles->ta_cursor, 500);

	style_init_reset(&styles->bg_color_primary_muted);
	lv_style_set_bg_color(&styles->bg_color_primary_muted, theme.color_primary);
	lv_style_set_text_color(&styles->bg_color_primary_muted, theme.color_primary);
	lv_style_set_bg_opa(&styles->bg_color_primary_muted, LV_OPA_20);

	style_init_reset(&styles->circle);
	lv_style_set_radius(&styles->circle, LV_RADIUS_CIRCLE);

	style_init_reset(&styles->knob);
	lv_style_set_bg_color(&styles->knob, theme.color_primary);
	lv_style_set_bg_opa(&styles->knob, LV_OPA_COVER);
	lv_style_set_pad_all(&styles->knob, lv_disp_dpx(theme.disp, 6));
	lv_style_set_radius(&styles->knob, LV_RADIUS_CIRCLE);

	style_init_reset(&styles->ta_placeholder);
	lv_style_set_text_color(&styles->ta_placeholder, (theme.flags & MODE_DARK) ? lv_palette_darken(LV_PALETTE_GREY,
																								   2) : lv_palette_lighten(LV_PALETTE_GREY, 1));
	style_init_reset(&styles->outline_primary);
	lv_style_set_outline_color(&styles->outline_primary, theme.color_primary);
	lv_style_set_outline_width(&styles->outline_primary, OUTLINE_WIDTH);
	lv_style_set_outline_pad(&styles->outline_primary, OUTLINE_WIDTH);
	lv_style_set_outline_opa(&styles->outline_primary, LV_OPA_50);

	style_init_reset(&styles->outline_secondary);
	lv_style_set_outline_color(&styles->outline_secondary, theme.color_secondary);
	lv_style_set_outline_width(&styles->outline_secondary, OUTLINE_WIDTH);
	lv_style_set_outline_opa(&styles->outline_secondary, LV_OPA_50);

	style_init_reset(&styles->bg_color_primary);
	lv_style_set_bg_color(&styles->bg_color_primary, theme.color_primary);
	lv_style_set_text_color(&styles->bg_color_primary, lv_color_white());
	lv_style_set_bg_opa(&styles->bg_color_primary, LV_OPA_COVER);

}

/**********************
*   GLOBAL FUNCTIONS
**********************/

void chatterThemeInit(lv_disp_t* disp){

	/*This trick is required only to avoid the garbage collection of
	 *styles' data if LVGL is used in a binding (e.g. Micropython)
	 *In a general case styles could be in simple `static lv_style_t my_style...` variables*/
	if(!inited){
		LV_GC_ROOT(_lv_theme_default_styles) = lv_mem_alloc(sizeof(my_theme_styles_t));
		styles = (my_theme_styles_t*) LV_GC_ROOT(_lv_theme_default_styles);
	}

	if(LV_HOR_RES <= 320) disp_size = DISP_SMALL;
	else if(LV_HOR_RES < 720) disp_size = DISP_MEDIUM;
	else disp_size = DISP_LARGE;

	theme.disp = disp;
	theme.apply_cb = theme_apply;

	style_init();

	inited = true;

	if(disp == NULL || lv_disp_get_theme(disp) == &theme) lv_obj_report_style_change(NULL);

	disp->theme = &theme;
}

lv_theme_t* chatterThemeGet(void){
	if(!inited) return NULL;

	return (lv_theme_t*) &theme;
}

bool chatterThemeInited(void){
	return inited;
}

static void theme_apply(lv_theme_t* th, lv_obj_t* obj){
	LV_UNUSED(th);

	if(lv_obj_check_type(obj, &lv_textarea_class)){
		lv_obj_add_style(obj, &styles->ta_cursor, LV_PART_CURSOR | LV_STATE_FOCUSED);
		lv_obj_add_style(obj, &styles->ta_placeholder, LV_PART_TEXTAREA_PLACEHOLDER);
	}

	if(lv_obj_check_type(obj, &lv_obj_class)){
		lv_obj_add_style(obj, &styles->card, 0);
		lv_obj_add_style(obj, &styles->scrollbar, LV_PART_SCROLLBAR);
	}
	else if(lv_obj_check_type(obj, &lv_slider_class)){
		lv_obj_add_style(obj, &styles->bg_color_primary_muted, 0);
		lv_obj_add_style(obj, &styles->circle, 0);
		lv_obj_add_style(obj, &styles->outline_primary, LV_STATE_FOCUS_KEY);
		lv_obj_add_style(obj, &styles->outline_secondary, LV_STATE_EDITED);
		lv_obj_add_style(obj, &styles->bg_color_primary, LV_PART_INDICATOR);
		lv_obj_add_style(obj, &styles->circle, LV_PART_INDICATOR);
		lv_obj_add_style(obj, &styles->knob, LV_PART_KNOB);

	}

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void style_init_reset(lv_style_t* style){
	if(inited) lv_style_reset(style);
	else lv_style_init(style);
}