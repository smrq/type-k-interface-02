#include "keyboard.h"

void keyboard_update() {
	keyscanner_update();
	encoder_update();
}
