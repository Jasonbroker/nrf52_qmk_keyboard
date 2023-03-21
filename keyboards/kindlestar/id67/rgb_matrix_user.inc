RGB_MATRIX_EFFECT(CUSTOM_WHITE)
RGB_MATRIX_EFFECT(CUSTOM_GREEN)
RGB_MATRIX_EFFECT(CUSTOM_BLUE)
RGB_MATRIX_EFFECT(CUSTOM_YELLOW)
RGB_MATRIX_EFFECT(CUSTOM_RED)
RGB_MATRIX_EFFECT(CUSTOM_PURPLE)
RGB_MATRIX_EFFECT(CUSTOM_ORANGE)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#define MYCOLOR(NAME, R, G, B) \
static bool CUSTOM_##NAME(effect_params_t* params) { \
  RGB_MATRIX_USE_LIMITS(led_min, led_max); \
  float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX; \
  for (uint8_t i = led_min; i < led_max; i++) { \
    rgb_matrix_set_color(i, f * R, f * G , f * B); \
  } \
  return rgb_matrix_check_finished_leds(led_max); \
}


MYCOLOR(WHITE, 0xff, 0xff, 0xff)

MYCOLOR(GREEN, 0, 0xff, 0)
MYCOLOR(BLUE, 0x0, 0, 0xff)
MYCOLOR(YELLOW, 0xff, 0xff, 0)
MYCOLOR(RED, 0xff, 0x0, 0x0)
MYCOLOR(PURPLE, 168, 0xff, 0xff)
MYCOLOR(ORANGE, 0xff, 0x8c, 0x0)

// static bool CUSTOM_WHITE(effect_params_t* params) {
//     RGB_MATRIX_USE_LIMITS(led_min, led_max);
//     float f   = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
//     rgb_matrix_set_color_all(f * 0xff, f * 0xff, f * 0xff);
//     return rgb_matrix_check_finished_leds(led_max);
// }

// static bool CUSTOM_GREEN(effect_params_t* params) {
//     RGB_MATRIX_USE_LIMITS(led_min, led_max);
//   float f   = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
//   rgb_matrix_set_color_all(0, f * 0xff, 0x0);
//   return rgb_matrix_check_finished_leds(led_max);
// }

// static bool CUSTOM_BLUE(effect_params_t* params) {
//         RGB_MATRIX_USE_LIMITS(led_min, led_max);
//   float f   = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
//   rgb_matrix_set_color_all(0, 0x0, f * 0xff);
//   return rgb_matrix_check_finished_leds(led_max);
// }

// static bool CUSTOM_YELLOW(effect_params_t* params) {
//         RGB_MATRIX_USE_LIMITS(led_min, led_max);
//   float f   = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
//   rgb_matrix_set_color_all(f * 0xff, f * 0xff, 0x0);
//   return rgb_matrix_check_finished_leds(led_max);
// }

// static bool CUSTOM_RED(effect_params_t* params) {
//         RGB_MATRIX_USE_LIMITS(led_min, led_max);
//     float f   = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
//   rgb_matrix_set_color_all(f * 0xff, 0x0, 0x0);
//   return rgb_matrix_check_finished_leds(led_max);
// }

// static bool CUSTOM_PURPLE(effect_params_t* params) {
//         RGB_MATRIX_USE_LIMITS(led_min, led_max);
//   float f   = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
//   rgb_matrix_set_color_all(f * 168, 0x0, f * 0xff);
//   return rgb_matrix_check_finished_leds(led_max);
// }

// static bool CUSTOM_ORANGE(effect_params_t* params) {
//     RGB_MATRIX_USE_LIMITS(led_min, led_max);
//   float f = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
//   rgb_matrix_set_color_all(f * 0xff, f * 0x8c, 0);
//   return rgb_matrix_check_finished_leds(led_max);
// }

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
