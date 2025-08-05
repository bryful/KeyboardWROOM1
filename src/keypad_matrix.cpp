#include "keypad_matrix.hpp"
#include <Arduino.h>

static const unsigned long DEBOUNCE_MS = 10;
static unsigned long lastChangeTime[KEY_MATRIX_SIZE] = {0};
static bool stableState[KEY_MATRIX_SIZE] = {false};
static bool prevStableState[KEY_MATRIX_SIZE] = {false};

void keypadMatrixInit()
{
    for (int r = 0; r < ROWS; r++)
    {
        pinMode(rowPins[r], OUTPUT);
        digitalWrite(rowPins[r], HIGH);
    }
    for (int c = 0; c < COLS; c++)
    {
        pinMode(colPins[c], INPUT_PULLUP);
    }
}

int scanMatrixRaw()
{
    int keyState = 0;
    for (int r = 0; r < ROWS; r++)
    {
        for (int i = 0; i < ROWS; i++)
            digitalWrite(rowPins[i], HIGH);
        digitalWrite(rowPins[r], LOW);
        for (int c = 0; c < COLS; c++)
        {
            if (digitalRead(colPins[c]) == LOW)
            {
                int keyIndex = r * COLS + c;
                keyState |= (1 << keyIndex);
            }
        }
    }
    return keyState;
}

int scanMatrixDebounced()
{
    int rawState = scanMatrixRaw();
    unsigned long now = millis();
    int debouncedState = 0;
    for (int i = 0; i < KEY_MATRIX_SIZE; i++)
    {
        bool pressed = (rawState & (1 << i));
        if (pressed != stableState[i])
        {
            if (pressed != prevStableState[i])
            {
                lastChangeTime[i] = now;
            }
            if ((now - lastChangeTime[i]) > DEBOUNCE_MS)
            {
                stableState[i] = pressed;
            }
        }
        if (stableState[i])
        {
            debouncedState |= (1 << i);
        }
        prevStableState[i] = pressed;
    }
    return debouncedState;
}
