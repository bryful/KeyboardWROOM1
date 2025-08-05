/*
  Keyboard.h

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once
#include "Print.h"
#include "USBHID.h"
#if CONFIG_TINYUSB_HID_ENABLED

#include "esp_event.h"

// HID Usage ID (Keyboard/Keypad Page 0x07) definitions
#define HID_KEY_A 0x04
#define HID_KEY_B 0x05
#define HID_KEY_C 0x06
#define HID_KEY_D 0x07
#define HID_KEY_E 0x08
#define HID_KEY_F 0x09
#define HID_KEY_G 0x0A
#define HID_KEY_H 0x0B
#define HID_KEY_I 0x0C
#define HID_KEY_J 0x0D
#define HID_KEY_K 0x0E
#define HID_KEY_L 0x0F
#define HID_KEY_M 0x10
#define HID_KEY_N 0x11
#define HID_KEY_O 0x12
#define HID_KEY_P 0x13
#define HID_KEY_Q 0x14
#define HID_KEY_R 0x15
#define HID_KEY_S 0x16
#define HID_KEY_T 0x17
#define HID_KEY_U 0x18
#define HID_KEY_V 0x19
#define HID_KEY_W 0x1A
#define HID_KEY_X 0x1B
#define HID_KEY_Y 0x1C
#define HID_KEY_Z 0x1D

#define HID_KEY_1 0x1E
#define HID_KEY_2 0x1F
#define HID_KEY_3 0x20
#define HID_KEY_4 0x21
#define HID_KEY_5 0x22
#define HID_KEY_6 0x23
#define HID_KEY_7 0x24
#define HID_KEY_8 0x25
#define HID_KEY_9 0x26
#define HID_KEY_0 0x27

#define HID_KEY_KP_1 0x59
#define HID_KEY_KP_2 0x5A
#define HID_KEY_KP_3 0x5B
#define HID_KEY_KP_4 0x5C
#define HID_KEY_KP_5 0x5D
#define HID_KEY_KP_6 0x5E
#define HID_KEY_KP_7 0x5F
#define HID_KEY_KP_8 0x60
#define HID_KEY_KP_9 0x62
#define HID_KEY_KP_0 0x63
#define HID_KEY_KP_ENDER 0x58

#define HID_KEY_ENTER 0x28
#define HID_KEY_ESC 0x29
#define HID_KEY_BACKSPACE 0x2A
#define HID_KEY_TAB 0x2B
#define HID_KEY_SPACE 0x2C

#define HID_KEY_DELETE 0x4C   // Delete (JIS: _ (同上))
#define HID_KEY_PAGEUP 0x4B   // Delete (JIS: _ (同上))
#define HID_KEY_PAGEDOWN 0x4E // Delete (JIS: _ (同上))

#define HID_KEY_ARROWRIGHT 0x4F // Delete (JIS: _ (同上))
#define HID_KEY_ARROWLEFT 0x50  // Delete (JIS: _ (同上))
#define HID_KEY_ARROWDOWN 0x51  // Delete (JIS: _ (同上))
#define HID_KEY_ARROWUP 0x52    // Delete (JIS: _ (同上))

// JIS配列基準のHID Usage ID（Keyboard/Keypad Page 0x07）
#define HID_KEY_MINUS 0x2D // - =
// #define HID_KEY_YEN 0x89        // JIS: ¥ (バックスラッシュ位置)
#define HID_KEY_CARET 0x2E      // JIS: ^ (日本語キーボード独自)
#define HID_KEY_AT 0x2F         // JIS: @ 91
#define HID_KEY_LEFTBRACE 0x30  // JIS: [ 2F
#define HID_KEY_RIGHTBRACE 0x32 // JIS: ] 30
// #define HID_KEY_RIGHTBRACE2 0x32     // JIS: ] 30
#define HID_KEY_COLON 0x92     // JIS: :
#define HID_KEY_SEMICOLON 0x33 // ;
// #define HID_KEY_APOSTROPHE 0x34      // '
// #define HID_KEY_GRAVE 0x35           // `
#define HID_KEY_COMMA 0x36           // ,
#define HID_KEY_DOT 0x37             // .
#define HID_KEY_SLASH 0x38           // /
#define HID_KEY_UNDERSCORE 0x93      // JIS: _
#define HID_KEY_KP_PLUS 0x57         // Keypad +
#define HID_KEY_KP_MINUS 0x56        // Keypad -
#define HID_KEY_KP_ASTERISK 0x55     // Keypad *
#define HID_KEY_KP_SLASH 0x54        // Keypad /
#define HID_KEY_KP_DOT 0x63          // Keypad .
#define HID_KEY_KP_COMMA 0x85        // JIS: Keypad ,
#define HID_KEY_KP_EQUAL 0x67        // Keypad =
#define HID_KEY_KP_ENTER 0x58        // Keypad Enter
#define HID_KEY_RO 0x87              // JIS: ろ
#define HID_KEY_KANA 0x88            // JIS: かな
#define HID_KEY_EISU 0x94            // JIS: 英数
#define HID_KEY_HENKAN 0x8A          // JIS: 変換
#define HID_KEY_MUHENKAN 0x8B        // JIS: 無変換
#define HID_KEY_HIRAGANA 0x8C        // JIS: ひらがな/カタカナ
#define HID_KEY_ZENKAKU_HANKAKU 0x8D // JIS: 全角/半角
#define HID_KEY_BACKSLASH 0x31       // \ (US配列)
#define HID_KEY_PIPE 0x64            // | (JIS: Shift+\)
#define HID_KEY_INTL1 0x87           // JIS: ろ (同上)
#define HID_KEY_INTL2 0x89           // JIS: ¥ (同上)
#define HID_KEY_INTL3 0x8A           // JIS: 変換 (同上)
#define HID_KEY_INTL4 0x8B           // JIS: 無変換 (同上)
#define HID_KEY_INTL5 0x8C           // JIS: ひらがな/カタカナ (同上)
#define HID_KEY_INTL6 0x8D           // JIS: 全角/半角 (同上)
#define HID_KEY_INTL7 0x94           // JIS: 英数 (同上)
#define HID_KEY_INTL8 0x92           // JIS: : (同上)
#define HID_KEY_INTL9 0x93           // JIS: _ (同上)

// 必要に応じてさらに追加してください（JIS配列の特殊キーやFnキーなど）

ESP_EVENT_DECLARE_BASE(ARDUINO_USB_HID_KEYBOARD_EVENTS);

typedef enum
{
  ARDUINO_USB_HID_KEYBOARD_ANY_EVENT = ESP_EVENT_ANY_ID,
  ARDUINO_USB_HID_KEYBOARD_LED_EVENT = 0,
  ARDUINO_USB_HID_KEYBOARD_MAX_EVENT,
} arduino_usb_hid_keyboard_event_t;

typedef union
{
  struct
  {
    uint8_t numlock : 1;
    uint8_t capslock : 1;
    uint8_t scrolllock : 1;
    uint8_t compose : 1;
    uint8_t kana : 1;
    uint8_t reserved : 3;
  };
  uint8_t leds;
} arduino_usb_hid_keyboard_event_data_t;

#define KEY_LEFT_CTRL 0x80
#define KEY_LEFT_SHIFT 0x81
#define KEY_LEFT_ALT 0x82
#define KEY_LEFT_GUI 0x83
#define KEY_RIGHT_CTRL 0x84
#define KEY_RIGHT_SHIFT 0x85
#define KEY_RIGHT_ALT 0x86
#define KEY_RIGHT_GUI 0x87

#define KEY_UP_ARROW 0xDA
#define KEY_DOWN_ARROW 0xD9
#define KEY_LEFT_ARROW 0xD8
#define KEY_RIGHT_ARROW 0xD7
#define KEY_BACKSPACE 0xB2
#define KEY_TAB 0xB3
#define KEY_RETURN 0xB0
#define KEY_ESC 0xB1
#define KEY_INSERT 0xD1
#define KEY_DELETE 0xD4
#define KEY_PAGE_UP 0xD3
#define KEY_PAGE_DOWN 0xD6
#define KEY_HOME 0xD2
#define KEY_END 0xD5
#define KEY_CAPS_LOCK 0xC1
#define KEY_F1 0xC2
#define KEY_F2 0xC3
#define KEY_F3 0xC4
#define KEY_F4 0xC5
#define KEY_F5 0xC6
#define KEY_F6 0xC7
#define KEY_F7 0xC8
#define KEY_F8 0xC9
#define KEY_F9 0xCA
#define KEY_F10 0xCB
#define KEY_F11 0xCC
#define KEY_F12 0xCD
#define KEY_F13 0xF0
#define KEY_F14 0xF1
#define KEY_F15 0xF2
#define KEY_F16 0xF3
#define KEY_F17 0xF4
#define KEY_F18 0xF5
#define KEY_F19 0xF6
#define KEY_F20 0xF7
#define KEY_F21 0xF8
#define KEY_F22 0xF9
#define KEY_F23 0xFA
#define KEY_F24 0xFB

#define LED_NUMLOCK 0x01
#define LED_CAPSLOCK 0x02
#define LED_SCROLLLOCK 0x04
#define LED_COMPOSE 0x08
#define LED_KANA 0x10

//  Low level key report: up to 6 keys and shift, ctrl etc at once
typedef struct
{
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyReport;

class USBHIDKeyboard : public USBHIDDevice, public Print
{
private:
  USBHID hid;
  KeyReport _keyReport;

public:
  USBHIDKeyboard(void);
  void begin(void);
  void end(void);
  size_t write(uint8_t k);
  size_t write(const uint8_t *buffer, size_t size);
  size_t press(uint8_t k);
  size_t release(uint8_t k);
  void releaseAll(void);
  void sendReport(KeyReport *keys);

  // raw functions work with TinyUSB's HID_KEY_* macros
  size_t pressRaw(uint8_t k);
  size_t releaseRaw(uint8_t k);

  void onEvent(esp_event_handler_t callback);
  void onEvent(arduino_usb_hid_keyboard_event_t event, esp_event_handler_t callback);

  // internal use
  uint16_t _onGetDescriptor(uint8_t *buffer);
  void _onOutput(uint8_t report_id, const uint8_t *buffer, uint16_t len);
};

#endif
