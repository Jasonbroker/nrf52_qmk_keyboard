RGB_MATRIX_EFFECT(CUSTOM_WHITE)
RGB_MATRIX_EFFECT(CUSTOM_GREEN)
RGB_MATRIX_EFFECT(CUSTOM_BLUE)
RGB_MATRIX_EFFECT(CUSTOM_YELLOW)
RGB_MATRIX_EFFECT(CUSTOM_RED)
RGB_MATRIX_EFFECT(CUSTOM_PURPLE)
RGB_MATRIX_EFFECT(CUSTOM_ORANGE)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool CUSTOM_WHITE(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
    rgb_matrix_set_color(i, 0xff, 0xff, 0xff);
  }
  return rgb_matrix_check_finished_leds(led_max);
}

static bool CUSTOM_GREEN(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
    rgb_matrix_set_color(i, 0x0, 0xff, 0x0);
  }
  return rgb_matrix_check_finished_leds(led_max);
}

static bool CUSTOM_BLUE(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
    rgb_matrix_set_color(i, 0x0, 0x0, 0xff);
  }
  return rgb_matrix_check_finished_leds(led_max);
}

static bool CUSTOM_YELLOW(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
    rgb_matrix_set_color(i, 0xff, 0xff, 0x0);
  }
  return rgb_matrix_check_finished_leds(led_max);
}

static bool CUSTOM_RED(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
    rgb_matrix_set_color(i, 0xff, 0x0, 0x0);
  }
  return rgb_matrix_check_finished_leds(led_max);
}

static bool CUSTOM_PURPLE(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
    rgb_matrix_set_color(i, 168, 0x0, 0xff);
  }
  return rgb_matrix_check_finished_leds(led_max);
}

static bool CUSTOM_ORANGE(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
    rgb_matrix_set_color(i, 200, 100, 80);
  }
  return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
