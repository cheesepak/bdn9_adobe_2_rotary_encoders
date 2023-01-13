 /* Copyright 2023 SirApple (based on Copyright 2019 Danny Nguyen <danny@keeb.io>)
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

 /* 
    SirApple's bdn9 keymap for use with Adobe Software on Windows 10.
    Currently supports Photoshop and Illustrator.
    Version 0.1.1 
    
    Note: Rotary encoders' clockwise/counterclockwise may be wrong using 
    this keymap on your bdn9. Might have done something wrong when building 
    mine, rip. If you decide to use this, you may need to fork and 
    adjust accordingly.
 */

 #include QMK_KEYBOARD_H

enum encoder_names {
  _LEFT,
  _RIGHT,
};

enum macropad_layers {
    _MUSIC, //Music controls and generic macros (F17-F22)
    _PS_BRUSHES, //Photoshop Brush controls
    _AI_TEXT, //Illustrator Text/Type controls
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	/*
	| Knob 1: Prev/Next |            | Knob 2: Vol Dn/Up  |
        | Press: Mute       | To Layer 1 | Press: Play/Pause  |
        | F17		    | F18   	 | F19 		      |
        | F20               | F21 	 | F22                |
	*/
	[_MUSIC] = LAYOUT(
		KC_MUTE, TO(1), KC_MPLY, 
		KC_F17, KC_F18, KC_F19, 
		KC_F20, KC_F21, KC_F22
	),

	/*	
        | Knob 1: Brush Hardness Adj	 |             | Knob 2: Brush Size Adj    |
        | CTRL + Alt w+ G (Clipping Mask)| To Layer 2  | CTRL + ; (Toggle Guides)  |
        | L (Lasso)			 | V (Move)    | Alt (temp Eyedropper)     |
        | W (Magic Wand)		 | E (Eraser)  | B (Brush)                 |
    */
	[_PS_BRUSHES] = LAYOUT(
		LCA(KC_G), TO(2), LCTL(KC_SCLN), 
		KC_L, KC_V, KC_LALT, 
		KC_W, KC_E, KC_B
	),

	/*
    	| Knob 1: Leading Adj		|			| Knob 2: Type Size Adj		|
        | CTRL + G (Group)		| To Layer 0    	| CTRL + ; (Toggle Guides)	|
        | T (Text) 			| V (Selection)		| Shift (Select Multiple)	|
        | Undo				| I (Eyedropper) 	| Space (Hand)			|
        */
	[_AI_TEXT] = LAYOUT(
		LCTL(KC_G), TO(0), LCTL(KC_SCLN), 
		KC_T, KC_V, KC_RSFT, 
		LCTL(KC_Z), KC_I, KC_SPC
	)
};


bool encoder_update_user(uint8_t index, bool clockwise) {
    if (layer_state_is(_MUSIC)) { 
        if (index == _LEFT) {
            if (clockwise) { // Prev/Next
                tap_code(KC_MPRV);
            } else {
                tap_code(KC_MNXT);
            }
        }
        else if (index == _RIGHT) { // Vol Dn/Up 
            if (clockwise) { 
                tap_code(KC_VOLD); 
            } else {
                tap_code(KC_VOLU);
            }
        }
    } 
   
    else if (layer_state_is(_PS_BRUSHES)) {
        if (index == _LEFT) { // Brush Hardness: SHIFT + [ or ]
            if (clockwise) {
                tap_code16(LSFT(KC_LBRC));
            } else {
                tap_code16(LSFT(KC_RBRC));
            }
        }
        else if (index == _RIGHT) { // Brush Size: [ or ]
            if (clockwise) {
                tap_code(KC_LBRC);
            } else {
                tap_code(KC_RBRC);
            }

        }
    }  
    else if (layer_state_is(_AI_TEXT)) {
        if (index == _LEFT) { // Leading Adj: ALT + Up or Down
            if (clockwise) {
                tap_code16(LALT(KC_UP));
            } else {
                tap_code16(LALT(KC_DOWN));
            }
        }
        else if (index == _RIGHT) { //Type Size Adj: 
            if (clockwise) {
                tap_code16(RCS(KC_COMM));
            } else {
                tap_code16(RCS(KC_DOT));
            }
        }
    } 
    return false;
};

 /* 
    Backlight LED brightness adjustment for layers.
    This currently does not take any alternate BL status into account. 
 */

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  bool pressed = record->event.pressed;
  
  switch(keycode) {
    case TO(0):
    if (pressed) {
      breathing_enable();
    }
    break;

    case TO(1):
    if (pressed) {
      breathing_disable();
      backlight_level(1);
    }
    break;

    case TO(2):
    if (pressed) {
      breathing_disable();
      backlight_level(3);
    }
    break;
  }
  return true;
};
