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
#define _BASEMOD  4


enum custom_keycodes {
  COOL = SAFE_RANGE,
  LAYERCHG = SAFE_RANGE,
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
        KC_KP_SLASH, KC_PAST, KC_PMNS, \
  /* KC_MUTE, KC_MPRV, KC_MPLY, KC_MNXT,\ */
  TO(_VIA1), KC_7, KC_8, KC_9, \
  KC_PPLS, KC_4, KC_5, KC_6,  \
  MO(_VIA1), KC_1, KC_2,  KC_3,  \
  KC_ENT, KC_DEL, KC_KP_DOT, KC_0     \
  ),  
  
     [_VIA1] = LAYOUT(
           TO(_BASE), TO(_VIA2), TO(_MIDI), \
  TO(_VIA2), KC_TRNS, KC_UP, KC_TRNS, \
  KC_TRNS, KC_LEFT, KC_DOWN, KC_RIGHT, \
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
  KC_TRNS, KC_TRNS, KC_DEL, KC_TRNS  \
  ),
    [_VIA2] = LAYOUT(
           RGB_HUI, RGB_SAI, RGB_VAI, \
  TO(_BASE), RGB_HUD, RGB_SAD, RGB_VAD, \
 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, \
   RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI  \
  ),

  [_MIDI] = LAYOUT(
        TO(_VIA1), MI_OCTD, MI_OCTU, \
  TO(_BASEMOD), MI_A_2, MI_B_2, MI_C_3, \
  MI_D_2, MI_E_2, MI_F_2, MI_G_2, \
  MI_G_1, MI_A_1, MI_B_1, MI_C_2, \
  MI_C_1, MI_D_1, MI_E_1, MI_F_1  \
  ),

  [_BASEMOD] = LAYOUT(
        TO(_VIA1), MI_OCTD, MI_OCTU, \
  TO(_BASE), MI_A_2, MI_B_2, MI_C_3, \
  MI_D_2, MI_E_2, MI_F_2, MI_G_2, \
  MI_G_1, MI_A_1, MI_B_1, MI_C_2, \
  //Reset Tiling
  HYPR(KC_R), MI_D_1, MI_E_1, KC_TRNS  \
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  process_record_remote_kb(keycode, record);
  return true;
}
//Listens for keycodes, EXPERIMENTAL
// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   // EXPERIMENTAL
//  // const uint16_t layerList[] = {_BASE, _VIA1, _VIA2, _MIDI};
//   //int layerIndex = 0;
  

//    switch (keycode) {
//     case LAYERCHG:
//         if (record->event.pressed) {
//          // layerIndex++;
//            SEND_STRING("test_beat");
//           // layer_move(_BASEMOD);
         
//         // layerIndex++;
//         //    if (layerIndex == 4){
//         //     layerIndex = 0;
//         //   }
        
//         /// layer_move(layerList[layerIndex % 4]);

//         } 
//         break;
//    }
 
//   process_record_remote_kb(keycode, record);

//   return true;
// }

void matrix_init_user(void) {
  matrix_init_remote_kb();
  set_bitc_LED(LED_OFF);
}

void matrix_scan_user(void) {
  matrix_scan_remote_kb();
}

// ENCODER
bool encoder_update_user(uint8_t index, bool clockwise) {
  switch (get_highest_layer(layer_state)) {
        case _BASE:
          if (clockwise) {
            // tap_code(KC_VOLU);
          } else {
            // tap_code(KC_VOLD);
          }
          return true;
          break;
          // ENCODER LED
        // case _VIA2:
        //   if (clockwise) {
        //     tap_code(RGB_HUI);                             
        //   } else {
        //     tap_code(RGB_MODE_REVERSE);
        //   }
        //   return true;
        //   break;
          // Backlight
        case _BASEMOD:
          if (clockwise) {
            tap_code(KC_RIGHT);
          }else {
            tap_code(KC_LEFT);
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

// layer_state_t layer_state_set_user(layer_state_t state) {
//     switch (get_highest_layer(layer_state)) {
//     case _BASE:
//         rgblight_setrgb (0x00,  0x00, 0xFF);
        
//         break;
//     case _VIA1:
//         rgblight_setrgb (0xFF,  0x00, 0x00);
//         break;
//     case _VIA2:
//         rgblight_setrgb (0x00,  0xFF, 0x00);
//         break;
//     case _MIDI:
//         rgblight_setrgb (0x7A,  0x00, 0xFF);
//         break;
//     default: //  for any other layers, or the default layer
//         rgblight_setrgb (0x00,  0xFF, 0xFF);
//         break;
//     }
//   return state;
// }

#ifdef OLED_DRIVER_ENABLE
void oled_task_user(void) {
    // Host Keyboard Layer Status

    oled_write_P(PSTR("LAYER: "), false);
  
    switch (get_highest_layer(layer_state)) {
        case _BASE:
             oled_write_P(PSTR("Base"), false);
        //oled_write_P(testChar, false);
            //  rgblight_setrgb (0xFF, 0xFF, 0xB2); for changing light per layer
            break;
       
        case _VIA1:
            oled_write_P(PSTR("MOD"), false);
            break;
        case _VIA2:
            oled_write_P(PSTR("Backlight"), false);
            break;
        case _MIDI:
            oled_write_P(PSTR("EAGLE HAS LANDED MIDI"), false);
            rgblight_setrgb (0x7F, 0xFF, 0xFA);
            break;
         case _BASEMOD:
            oled_write_P(PSTR("Basemod"), false);
            rgblight_setrgb (0xF9, 0x6D, 0x6D);
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