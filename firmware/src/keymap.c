#include "keymap.h"

const PROGMEM Keymap_t keymap = {
	.layers = {
		KEYMAP_LAYER(
			KC_USER0,  KC_F13, KC_F14, KC_F15, KC_F16, KC_F17, KC_F18, KC_F19, KC_F20, KC_F21,   KC_F22,   KC_F23,   KC_F24,    KC_MEDIA_PLAY_PAUSE, KC_MEDIA_PREV_TRACK, KC_MEDIA_STOP, KC_MEDIA_NEXT_TRACK, KC_MY_COMPUTER, KC_AUDIO_MUTE,
			KC_ESC,    KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,  KC_F9,    KC_F10,   KC_F11,   KC_F12,    KC_USER1,  KC_PRSC,  KC_SCRLK, KC_PAUSE, KC_USER2,            KC_USER3,
			KC_GRAVE,  KC_1,   KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,   KC_9,     KC_0,     KC_DASH,  KC_EQUAL,  KC_BSPACE, KC_INS,   KC_HOME,  KC_PGUP,  KC_NUMLK, KC_KP_DIV, KC_KP_MUL, KC_KP_SUB,
			KC_TAB,    KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,   KC_O,     KC_P,     KC_LBRK,  KC_RBRK,   KC_BSLASH, KC_DEL,   KC_END,   KC_PGDN,  KC_KP_7,  KC_KP_8,   KC_KP_9,
			KC_CAPSLK, KC_A,   KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,   KC_L,     KC_SEMI,  KC_QUOTE,            KC_ENTER,                                KC_KP_4,  KC_KP_5,   KC_KP_6,   KC_KP_ADD,
			KC_LSHIFT,         KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMMA, KC_DOT,   KC_SLASH,            KC_RSHIFT, KC_USER4, KC_UP,   KC_USER5,  KC_KP_1,  KC_KP_2,   KC_KP_3,
			KC_LCTRL,          KC_LGUI,        KC_LALT,        KC_SPACE,       KC_USER6,         KC_RALT,            KC_RCTRL,  KC_USER7,  KC_LEFT,  KC_DOWN, KC_RIGHT,  KC_KP_0,             KC_KP_DOT, KC_KP_ENTER
		)
	},
	.encoder = {
		.cw = KC_AUDIO_VOL_UP,
		.ccw = KC_AUDIO_VOL_DOWN
	}
};
