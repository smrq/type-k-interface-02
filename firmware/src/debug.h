#pragma once

void console_write(const char *text);
#define DEBUG(...) do { char buf[64]; snprintf(buf, sizeof(buf), __VA_ARGS__); console_write(buf); } while (0)
