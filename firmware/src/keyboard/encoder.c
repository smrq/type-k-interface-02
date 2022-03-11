#include "encoder.h"

local u8 _previousState = 0;
local u8 _currentState = 0;

void encoder_update() {
	_previousState = _currentState;
	_currentState = ((PINC & _BV(6)) >> 6) | ((PIND & _BV(2)) >> 1);
}

enum Encoder_State_t encoder_get_state() {
	switch (_previousState) {
		case 0b00:
			return
				_currentState == 0b01 ? Encoder_State_CW :
				_currentState == 0b10 ? Encoder_State_CCW :
				Encoder_State_None;
		case 0b01:
			return
				_currentState == 0b11 ? Encoder_State_CW :
				_currentState == 0b00 ? Encoder_State_CCW :
				Encoder_State_None;
		case 0b11:
			return
				_currentState == 0b10 ? Encoder_State_CW :
				_currentState == 0b01 ? Encoder_State_CCW :
				Encoder_State_None;
		case 0b10:
			return
				_currentState == 0b00 ? Encoder_State_CW :
				_currentState == 0b11 ? Encoder_State_CCW :
				Encoder_State_None;
		default: // This should never happen.
			return Encoder_State_None;
	}
}
