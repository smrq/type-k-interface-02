#pragma once

#include "../defs.h"

typedef struct {
	u8 current;
	u8 counter0;
	u8 counter1;
} Keyscanner_ColumnState_t;

void keyscanner_update();
u8 keyscanner_get_state(u8 column);
