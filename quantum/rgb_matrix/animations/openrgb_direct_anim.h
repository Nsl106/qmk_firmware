#ifdef OPENRGB_ENABLE
RGB_MATRIX_EFFECT(OPENRGB_DIRECT)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
extern RGB           g_openrgb_direct_mode_colors[RGB_MATRIX_LED_COUNT];
extern volatile bool g_openrgb_host_connected;

// Boot/idle "loading" indicator: until the host pushes its first Direct frame,
// breathe a single dim ESC (LED 0) and keep every other key dark, instead of the
// full-buffer default (a hard-coded full-bright colour). In Windows (no host ever
// connects) this is the resting state; Fn+W still toggles RGB independently.
#        ifndef OPENRGB_BOOT_INDICATOR_LED
#            define OPENRGB_BOOT_INDICATOR_LED 0   // ESC
#        endif
#        ifndef OPENRGB_BOOT_INDICATOR_MAX
#            define OPENRGB_BOOT_INDICATOR_MAX 60  // peak white level of the breathe (0-255)
#        endif
#        ifndef OPENRGB_BOOT_INDICATOR_SHIFT
#            define OPENRGB_BOOT_INDICATOR_SHIFT 3 // breathe period ~= 256 * 2^shift ms (~2s)
#        endif

bool OPENRGB_DIRECT(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (!g_openrgb_host_connected) {
        const uint8_t v = scale8(sin8(g_rgb_timer >> OPENRGB_BOOT_INDICATOR_SHIFT), OPENRGB_BOOT_INDICATOR_MAX);
        for (uint8_t i = led_min; i < led_max; i++) {
            if (i == OPENRGB_BOOT_INDICATOR_LED) {
                rgb_matrix_set_color(i, v, v, v);
            } else {
                rgb_matrix_set_color(i, 0, 0, 0);
            }
        }
        return led_max < RGB_MATRIX_LED_COUNT;
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        #ifdef OPENRGB_DIRECT_MODE_USE_UNIVERSAL_BRIGHTNESS
        float brightness = (float)rgb_matrix_config.hsv.v / UINT8_MAX;
        rgb_matrix_set_color(
            i,
            brightness * g_openrgb_direct_mode_colors[i].r,
            brightness * g_openrgb_direct_mode_colors[i].g,
            brightness * g_openrgb_direct_mode_colors[i].b
        );
        #else
        rgb_matrix_set_color(
            i,
            g_openrgb_direct_mode_colors[i].r,
            g_openrgb_direct_mode_colors[i].g,
            g_openrgb_direct_mode_colors[i].b
        );
        #endif
    }
    return led_max < RGB_MATRIX_LED_COUNT;
}
#    endif
#endif
