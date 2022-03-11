#include "led_animation.h"

enum AnimationResult_t animation_led_race(u16 frame) {
	LED_clear();
	LED_set(frame, 0x0a0a0a);
	LED_update();
	return (frame == LED_COUNT-1) ?
		AnimationResult_Finished :
		AnimationResult_Continue;	
}
