#include <USBHIDKeyboard.h>
#include <USBHIDMouse.h>
#include "hid_sender.hpp"
#include "keypad_common.hpp"

extern USBHIDKeyboard Keyboard;
extern USBHIDMouse Mouse;

// 新: modifiersをHIDレポートで送信する
void hidSendModifierKey(uint8_t modifiers, bool press)
{
    KeyReport report = {0};
    report.modifiers = press ? modifiers : 0;
    report.keys[0] = 0;
    Keyboard.sendReport(&report);
}

void hidSendCurrentKeyReport(int keyCode, KeyConfig *keyConfigs, bool *keyPressed, uint8_t *prevKeys)
{
    KeyReport report = {0};
    bool anyPressed = false;
    for (int i = 0; i < KEY_MATRIX_SIZE; i++)
    {
        bool pressed = (keyPressed[i] || (keyCode & (1 << i)));
        bool wasPressed = prevKeys[i];
        KeyFunction func = keyConfigs[i].function;
        uint8_t key = keyConfigs[i].primaryKey;
        uint8_t mods = keyConfigs[i].modifiers;
        if (pressed)
        {
            anyPressed = true;
            // マウスクリック対応
            if (func == MOUSE_LEFT_CLICK)
            {
                if (!wasPressed)
                    Mouse.press(MOUSE_BUTTON_LEFT);
            }
            else if (func == MOUSE_RIGHT_CLICK)
            {
                if (!wasPressed)
                    Mouse.press(MOUSE_BUTTON_RIGHT);
            }
            else
            {
                report.modifiers = mods;
                report.keys[0] = key;
            }
        }
        else
        {
            if (func == MOUSE_LEFT_CLICK && wasPressed)
                Mouse.release(MOUSE_BUTTON_LEFT);
            else if (func == MOUSE_RIGHT_CLICK && wasPressed)
                Mouse.release(MOUSE_BUTTON_RIGHT);
        }
        prevKeys[i] = pressed;
    }
    if (anyPressed)
    {
        Keyboard.sendReport(&report);
    }
    else
    {
        // すべて離された場合は必ずリリースレポートを送信
        KeyReport empty = {0};
        Keyboard.sendReport(&empty);
    }
}
