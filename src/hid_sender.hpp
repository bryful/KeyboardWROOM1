#pragma once
#include <stdint.h>
#include "keypad_common.hpp"

void hidSendCurrentKeyReport(int keyCode, KeyConfig *keyConfigs, bool *keyPressed, uint8_t *prevKeys);
void hidSendModifierKey(uint8_t modifiers, bool press);
