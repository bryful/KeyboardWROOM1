#pragma once
#include <stdint.h>
#include "keypad_common.hpp"

// ピン配置の外部変数宣言
extern const int ROWS;
extern const int COLS;
extern const int rowPins[4];
extern const int colPins[4];

void keypadMatrixInit();
int scanMatrixRaw();
int scanMatrixDebounced();
