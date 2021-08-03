/* Copyright 2020 Jay Greco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "remote_kb.h"
#include "bitc_led.h"
extern MidiDevice midi_device;

#define _BASE     0
#define _VIA1     1
#define _VIA2     2
#define _MIDI     3


enum custom_keycodes {
  COOL = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* BASE/MIDI
    * |------+------+------+------|
   * |       | D2  | Ch +  |  Ch - |
   * ,----------------------------
   * | KNOB  | ON  | Oct - |Oct + |
   * |------+------+------+------|
   * | D3   |  E3  |  F3  |  G3  |
   * |------+------+------+------|
   * | G2   |  A2  |  B2  |   C2 |
   * |------+------+------+------|
   * |  C2 |  D2  | E2   |  F2   |
   * `----------------------------
   */
  
  [_BASE] = LAYOUT(
        TO(_VIA1), TO(_VIA1), TO(_MIDI), \
  RESET, KC_8,     KC_9, KC_PSLS, \
  KC_4, KC_5,     KC_6, KC_PAST, \
  KC_1, KC_2,     KC_3, KC_PMNS, \
  COOL, KC_DOT, KC_ENT, KC_PPLS  \
  ),
  [_VIA1] = LAYOUT(
           TO(_BASE), TO(_VIA2), TO(_MIDI), \
  RGB_TOG, KC_TRNS, KC_TRNS, KC_TRNS, \
  KC_LSHIFT, KC_TRNS, KC_TRNS, KC_TRNS, \
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
   RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI  \
  ),
  [_VIA2] = LAYOUT(
           TO(_BASE), KC_TRNS, KC_TRNS, \
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS  \
  ),
  [_MIDI] = LAYOUT(
        TO(_VIA1), MI_OCTD, MI_OCTU, \
  TO(_BASE), MI_A_2, MI_B_2, MI_C_3, \
  MI_D_2, MI_E_2, MI_F_2, MI_G_2, \
  MI_G_1, MI_A_1, MI_B_1, MI_C_2, \
  MI_C_1, MI_D_1, MI_E_1, MI_F_1  \
  ),

};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
   switch (keycode) {
    case COOL:
        if (record->event.pressed) {
            SEND_STRING("test_beat");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    }
  process_record_remote_kb(keycode, record);
  return true;
}

void matrix_init_user(void) {
  matrix_init_remote_kb();
  set_bitc_LED(LED_OFF);
}

void matrix_scan_user(void) {
  matrix_scan_remote_kb();
}

// bool encoder_update_user(uint8_t index, bool clockwise) {
//   if (clockwise) {
//     tap_code(KC_VOLU);
//   } else {
//     tap_code(KC_VOLD);
//   } 
//   return true;
// }

bool encoder_update_user(uint8_t index, bool clockwise) {
  switch (get_highest_layer(layer_state)) {
        case _BASE:
          if (clockwise) {
        tap_code(KC_VOLU);
          }else {
        tap_code(KC_VOLD);
          }
          return true;
          break;
          // Backlight
        case _VIA1:
          if (clockwise) {
          // tap_code(RGB_HUI);
          }else {
         //   tap_code(RGB_SAI);
          }
          return true;
          break;
       
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            return true;
    }
 
}
 

void led_set_kb(uint8_t usb_led) {
  if (usb_led & (1<<USB_LED_NUM_LOCK))
    set_bitc_LED(LED_DIM);
  else
    set_bitc_LED(LED_OFF);
}

#ifdef OLED_DRIVER_ENABLE
void oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("STAGE: "), false);

    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("S-IC \nGodspeed"), false);
            break;
        case _VIA1:
            oled_write_P(PSTR("S-II \n Backlight"), false);
            break;
        case _VIA2:
            oled_write_P(PSTR("S-IVB \nEarth Departure"), false);
            break;
        case _MIDI:
            oled_write_P(PSTR("EAGLE HAS LANDED MIDI"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            oled_write_ln_P(PSTR("Undefined"), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP ") : PSTR("    "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
}
#endif