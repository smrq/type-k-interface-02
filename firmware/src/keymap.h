#pragma once

#include "defs.h"
#include "usb/usb_reports.h"
#include "usb/usb_spec.h"

typedef struct {
	u8 cw;
	u8 ccw;
} Keymap_Encoder_t;

typedef struct {
	u8 layers[LAYER_COUNT][ROW_COUNT][COLUMN_COUNT];
	Keymap_Encoder_t encoder;
} Keymap_t;

extern const PROGMEM Keymap_t keymap;

void keymap_user_macro(u8 index, bool held, USB_NkroKeyboardReport_t *keyboardReport, u16 *systemCode, u16 *consumerCode);

#define KEYMAP_LAYER( \
	KA00, KA01, KA02, KA03, KA04, KA05, KA06, KA07, KA08, KA09, KA10, KA11, KA12,   KA13, KA14, KA15, KA16,      KA18,       KA20,    \
	KB00, KB01, KB02, KB03, KB04, KB05, KB06, KB07, KB08, KB09, KB10, KB11, KB12,   KB13, KB14, KB15, KB16,      KB18,       KB20,    \
	KC00, KC01, KC02, KC03, KC04, KC05, KC06, KC07, KC08, KC09, KC10, KC11, KC12, KC13,   KC14, KC15, KC16,   KC17, KC18, KC19, KC20, \
	KD00, KD01, KD02, KD03, KD04, KD05, KD06, KD07, KD08, KD09, KD10, KD11, KD12, KD13,   KD14, KD15, KD16,   KD17, KD18, KD19,       \
	KE00, KE01, KE02, KE03, KE04, KE05, KE06, KE07, KE08, KE09, KE10, KE11,       KE13,                       KE17, KE18, KE19, KE20, \
	KF00,       KF02, KF03, KF04, KF05, KF06, KF07, KF08, KF09, KF10, KF11,       KF13,   KF14, KF15, KF16,   KF17, KF18, KF19,       \
	KG00,   KG01,   KG02,               KG06,             KG10,   KG11,   KG12,   KG13,   KG14, KG15, KG16,   KG17,       KG19, KG20  \
) { \
	{ KA00, KA02, KA03, KA04, KA06, KA07, KA08, KA10, KA11, KA12, KA13, KA14, KA15, KA16, KA18, KA20 }, \
	{ KB00, KA01, KB03, KB04, KA05, KB07, KB08, KA09, KB11, KB12, KB13, KB14, KB15, KB16, KB18, KB20 }, \
	{ KC00, KB01, KB02, KC04, KB05, KB06, KC08, KB09, KB10, KC11, KC12, KC13, KC15, KC16, KC18, KC19 }, \
	{ KD00, KC01, KC02, KC03, KC05, KC06, KC07, KC09, KC10, KD11, KD12, KC14, KD15, KC17, KD19, KC20 }, \
	{ KE00, KD01, KD02, KD03, KD05, KD06, KD07, KD08, KD10, KE11, KD13, KD14, KD16, KD17, KD18, KE20 }, \
	{ KF00, KE01, KE02, KD04, KE05, KE06, KE07, KD09, KE10, KF11, KE13, KF15, KF16, KE17, KE18, KE19 }, \
	{ KG00, KF02, KE03, KE04, KF05, KF06, KE08, KE09, KF10, KG11, KF13, KF14, KG16, KF17, KF18, KF19 }, \
	{ KG01, KG02, KF03, KF04, KG06, KF07, KF08, KF09, KG10, KG12, KG13, KG14, KG15, KG17, KG19, KG20 }  \
}

// Keymap constants

#define KC_NULL        0x00
#define KC_TRANSPARENT 0x01

// Keyboard scancodes

#define KEYMAP_IS_SCANCODE(n)   (((n) >= 0x04 && (n) <= 0xA4) || ((n) >= 0xE0 && (n) <= 0xE7))
#define KC_A                    HID_KEYBOARD_A
#define KC_B                    HID_KEYBOARD_B
#define KC_C                    HID_KEYBOARD_C
#define KC_D                    HID_KEYBOARD_D
#define KC_E                    HID_KEYBOARD_E
#define KC_F                    HID_KEYBOARD_F
#define KC_G                    HID_KEYBOARD_G
#define KC_H                    HID_KEYBOARD_H
#define KC_I                    HID_KEYBOARD_I
#define KC_J                    HID_KEYBOARD_J
#define KC_K                    HID_KEYBOARD_K
#define KC_L                    HID_KEYBOARD_L
#define KC_M                    HID_KEYBOARD_M
#define KC_N                    HID_KEYBOARD_N
#define KC_O                    HID_KEYBOARD_O
#define KC_P                    HID_KEYBOARD_P
#define KC_Q                    HID_KEYBOARD_Q
#define KC_R                    HID_KEYBOARD_R
#define KC_S                    HID_KEYBOARD_S
#define KC_T                    HID_KEYBOARD_T
#define KC_U                    HID_KEYBOARD_U
#define KC_V                    HID_KEYBOARD_V
#define KC_W                    HID_KEYBOARD_W
#define KC_X                    HID_KEYBOARD_X
#define KC_Y                    HID_KEYBOARD_Y
#define KC_Z                    HID_KEYBOARD_Z
#define KC_1                    HID_KEYBOARD_1_BANG
#define KC_2                    HID_KEYBOARD_2_AT
#define KC_3                    HID_KEYBOARD_3_HASH
#define KC_4                    HID_KEYBOARD_4_DOLLAR
#define KC_5                    HID_KEYBOARD_5_PERCENT
#define KC_6                    HID_KEYBOARD_6_CARET
#define KC_7                    HID_KEYBOARD_7_AMPERSAND
#define KC_8                    HID_KEYBOARD_8_ASTERISK
#define KC_9                    HID_KEYBOARD_9_LPAREN
#define KC_0                    HID_KEYBOARD_0_RPAREN
#define KC_ENTER                HID_KEYBOARD_ENTER
#define KC_ESC                  HID_KEYBOARD_ESCAPE
#define KC_BSPACE               HID_KEYBOARD_BACKSPACE
#define KC_TAB                  HID_KEYBOARD_TAB
#define KC_SPACE                HID_KEYBOARD_SPACE
#define KC_DASH                 HID_KEYBOARD_DASH_UNDERSCORE
#define KC_EQUAL                HID_KEYBOARD_EQUAL_PLUS
#define KC_LBRK                 HID_KEYBOARD_LBRACKET_LBRACE
#define KC_RBRK                 HID_KEYBOARD_RBRACKET_RBRACE
#define KC_BSLASH               HID_KEYBOARD_BACKSLASH_PIPE
#define KC_NONUS_HASH           HID_KEYBOARD_NON_US_HASH_TILDE
#define KC_SEMI                 HID_KEYBOARD_SEMICOLON_COLON
#define KC_QUOTE                HID_KEYBOARD_APOSTROPHE_QUOTE
#define KC_GRAVE                HID_KEYBOARD_BACKTICK_TILDE
#define KC_COMMA                HID_KEYBOARD_COMMA_LANGLE
#define KC_DOT                  HID_KEYBOARD_DOT_RANGLE
#define KC_SLASH                HID_KEYBOARD_SLASH_QUESTION
#define KC_CAPSLK               HID_KEYBOARD_CAPS_LOCK
#define KC_F1                   HID_KEYBOARD_F1
#define KC_F2                   HID_KEYBOARD_F2
#define KC_F3                   HID_KEYBOARD_F3
#define KC_F4                   HID_KEYBOARD_F4
#define KC_F5                   HID_KEYBOARD_F5
#define KC_F6                   HID_KEYBOARD_F6
#define KC_F7                   HID_KEYBOARD_F7
#define KC_F8                   HID_KEYBOARD_F8
#define KC_F9                   HID_KEYBOARD_F9
#define KC_F10                  HID_KEYBOARD_F10
#define KC_F11                  HID_KEYBOARD_F11
#define KC_F12                  HID_KEYBOARD_F12
#define KC_PRSC                 HID_KEYBOARD_PRINT_SCREEN
#define KC_SCRLK                HID_KEYBOARD_SCROLL_LOCK
#define KC_PAUSE                HID_KEYBOARD_PAUSE
#define KC_INS                  HID_KEYBOARD_INSERT
#define KC_HOME                 HID_KEYBOARD_HOME
#define KC_PGUP                 HID_KEYBOARD_PAGE_UP
#define KC_DEL                  HID_KEYBOARD_DELETE
#define KC_END                  HID_KEYBOARD_END
#define KC_PGDN                 HID_KEYBOARD_PAGE_DOWN
#define KC_RIGHT                HID_KEYBOARD_RIGHT_ARROW
#define KC_LEFT                 HID_KEYBOARD_LEFT_ARROW
#define KC_DOWN                 HID_KEYBOARD_DOWN_ARROW
#define KC_UP                   HID_KEYBOARD_UP_ARROW
#define KC_NUMLK                HID_KEYPAD_NUM_LOCK
#define KC_KP_DIV               HID_KEYPAD_SLASH
#define KC_KP_MUL               HID_KEYPAD_ASTERISK
#define KC_KP_SUB               HID_KEYPAD_DASH
#define KC_KP_ADD               HID_KEYPAD_PLUS
#define KC_KP_ENTER             HID_KEYPAD_ENTER
#define KC_KP_1                 HID_KEYPAD_1_END
#define KC_KP_2                 HID_KEYPAD_2_DOWN_ARROW
#define KC_KP_3                 HID_KEYPAD_3_PAGE_DOWN
#define KC_KP_4                 HID_KEYPAD_4_LEFT_ARROW
#define KC_KP_5                 HID_KEYPAD_5
#define KC_KP_6                 HID_KEYPAD_6_RIGHT_ARROW
#define KC_KP_7                 HID_KEYPAD_7_HOME
#define KC_KP_8                 HID_KEYPAD_8_UP_ARROW
#define KC_KP_9                 HID_KEYPAD_9_PAGE_UP
#define KC_KP_0                 HID_KEYPAD_0_INSERT
#define KC_KP_DOT               HID_KEYPAD_DOT_DELETE
#define KC_NON_US_BSLASH        HID_KEYBOARD_NON_US_BACKSLASH_PIPE
#define KC_APP                  HID_KEYBOARD_APPLICATION
#define KC_POWER                HID_KEYBOARD_POWER
#define KC_KP_EQUAL             HID_KEYPAD_EQUAL
#define KC_F13                  HID_KEYBOARD_F13
#define KC_F14                  HID_KEYBOARD_F14
#define KC_F15                  HID_KEYBOARD_F15
#define KC_F16                  HID_KEYBOARD_F16
#define KC_F17                  HID_KEYBOARD_F17
#define KC_F18                  HID_KEYBOARD_F18
#define KC_F19                  HID_KEYBOARD_F19
#define KC_F20                  HID_KEYBOARD_F20
#define KC_F21                  HID_KEYBOARD_F21
#define KC_F22                  HID_KEYBOARD_F22
#define KC_F23                  HID_KEYBOARD_F23
#define KC_F24                  HID_KEYBOARD_F24
#define KC_KP_COMMA             HID_KEYPAD_COMMA
#define KC_INTL1                HID_KEYBOARD_INTERNATIONAL1
#define KC_INTL2                HID_KEYBOARD_INTERNATIONAL2
#define KC_INTL3                HID_KEYBOARD_INTERNATIONAL3
#define KC_INTL4                HID_KEYBOARD_INTERNATIONAL4
#define KC_INTL5                HID_KEYBOARD_INTERNATIONAL5
#define KC_INTL6                HID_KEYBOARD_INTERNATIONAL6
#define KC_LANG1                HID_KEYBOARD_LANG1
#define KC_LANG2                HID_KEYBOARD_LANG2
#define KC_LANG3                HID_KEYBOARD_LANG3
#define KC_LANG4                HID_KEYBOARD_LANG4
#define KC_LANG5                HID_KEYBOARD_LANG5
#define KC_LCTRL                HID_KEYBOARD_LCONTROL
#define KC_LSHIFT               HID_KEYBOARD_LSHIFT
#define KC_LALT                 HID_KEYBOARD_LALT
#define KC_LGUI                 HID_KEYBOARD_LGUI
#define KC_RCTRL                HID_KEYBOARD_RCONTROL
#define KC_RSHIFT               HID_KEYBOARD_RSHIFT
#define KC_RALT                 HID_KEYBOARD_RALT
#define KC_RGUI                 HID_KEYBOARD_RGUI
#define KC_RESERVED_E9          0xE9
#define KC_RESERVED_EA_SLEEP    0xEA
#define KC_RESERVED_EB          0xEB

// System control page

#define KEYMAP_IS_SYSTEM(n)   ((n) >= 0xA5 && (n) <= 0xAF)
#define KC_SYSTEM_POWER_DOWN  0xA5
#define KC_SYSTEM_SLEEP       0xA6
#define KC_SYSTEM_WAKE        0xA7

inline u16 keymap_code_to_system(u8 code) {
	switch (code) {
		case KC_SYSTEM_POWER_DOWN: return HID_DESKTOP_SYSTEM_POWER_DOWN;
		case KC_SYSTEM_SLEEP     : return HID_DESKTOP_SYSTEM_SLEEP;
		case KC_SYSTEM_WAKE      : return HID_DESKTOP_SYSTEM_WAKE_UP;
		default: return 0;
	}
}

// Consumer page

#define KEYMAP_IS_CONSUMER(n) ((n) >= 0xB0 && (n) <= 0xBF)
#define KC_BRIGHTNESS_INC     0xB0
#define KC_BRIGHTNESS_DEC     0xB1
#define KC_MEDIA_FAST_FORWARD 0xB2
#define KC_MEDIA_REWIND       0xB3
#define KC_MEDIA_NEXT_TRACK   0xB4
#define KC_MEDIA_PREV_TRACK   0xB5
#define KC_MEDIA_STOP         0xB6
#define KC_MEDIA_PLAY_PAUSE   0xB7
#define KC_AUDIO_MUTE         0xB8
#define KC_AUDIO_VOL_UP       0xB9
#define KC_AUDIO_VOL_DOWN     0xBA
#define KC_MEDIA_SELECT       0xBB
#define KC_MAIL               0xBC
#define KC_CALCULATOR         0xBD
#define KC_MY_COMPUTER        0xBE

#define KC_PLAY KC_MEDIA_PLAY_PAUSE
#define KC_PREV KC_MEDIA_PREV_TRACK
#define KC_NEXT KC_MEDIA_NEXT_TRACK

inline u16 keymap_code_to_consumer(u8 code) {
	switch (code) {
		case KC_BRIGHTNESS_INC    : return HID_CONSUMER_BRIGHTNESS_INCREMENT;
		case KC_BRIGHTNESS_DEC    : return HID_CONSUMER_BRIGHTNESS_DECREMENT;
		case KC_MEDIA_FAST_FORWARD: return HID_CONSUMER_FAST_FORWARD;
		case KC_MEDIA_REWIND      : return HID_CONSUMER_REWIND;
		case KC_MEDIA_NEXT_TRACK  : return HID_CONSUMER_SCAN_NEXT_TRACK;
		case KC_MEDIA_PREV_TRACK  : return HID_CONSUMER_SCAN_PREVIOUS_TRACK;
		case KC_MEDIA_STOP        : return HID_CONSUMER_STOP;
		case KC_MEDIA_PLAY_PAUSE  : return HID_CONSUMER_PLAY_PAUSE;
		case KC_AUDIO_MUTE        : return HID_CONSUMER_MUTE;
		case KC_AUDIO_VOL_UP      : return HID_CONSUMER_VOLUME_INCREMENT;
		case KC_AUDIO_VOL_DOWN    : return HID_CONSUMER_VOLUME_DECREMENT;
		case KC_MEDIA_SELECT      : return HID_CONSUMER_APPLAUNCH_CC_CONFIG;
		case KC_MAIL              : return HID_CONSUMER_APPLAUNCH_EMAIL;
		case KC_CALCULATOR        : return HID_CONSUMER_APPLAUNCH_CALCULATOR;
		case KC_MY_COMPUTER       : return HID_CONSUMER_APPLAUNCH_LOCAL_BROWSER;
		default: return 0;
	}
}

// User macros

#define KEYMAP_IS_USER(n)     ((n) >= 0xC0 && (n) <= 0xDF)
#define KC_USER(n)            (0xC0 | n)

inline u8 keymap_code_to_user(u8 code) {
	return code & 0x1F;
}

// Firmware functions

#define KEYMAP_IS_FIRMWARE(n) ((n) >= 0xE8 && (n) <= 0xEF)
#define KC_FW_RESET           0xE8

// Layer shifts

#define KEYMAP_IS_LAYER(n)    ((n) >= 0xF0)
#define KC_LAYER(n)           (0xF0 | n)

inline u8 keymap_code_to_layer(u8 code) {
	return code & 0x0F;
}
