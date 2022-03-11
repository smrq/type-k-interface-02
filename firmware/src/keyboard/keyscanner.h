#pragma once

#include "../defs.h"

typedef struct {
	u8 current;
	u8 counter0;
	u8 counter1;
	u8 previous;
} Keyscanner_ColumnState_t;

void keyscanner_update();
u8 keyscanner_get_state(u8 column);
u8 keyscanner_get_previous_state(u8 column);
