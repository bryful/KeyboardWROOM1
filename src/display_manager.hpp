#pragma once

#include <Arduino.h>
#include "LGFX_ESP32S3WROOM1N8R2_SPI_ST7789.hpp"
#include "keypad_common.hpp"

// ディスプレイ管理クラス
class DisplayManager
{
public:
    // ディスプレイ状態管理
    struct DisplayState
    {
        bool keyPressed[KEY_MATRIX_SIZE];
        bool rotaryPressed[ROTARY_COUNT];
        bool profileActive[10]; // プロファイル数に応じて調整
        int selectedProfileIndex;
        bool activeKeyDisplayed; // アクティブキー表示の状態
        String lastActiveKeys;   // 前回表示したアクティブキー文字列
    };

    // アイコン関連定数
    static const int ICON_COUNT = 54;
    static const int ICON_SIZE = 48;        // スプライト用アイコンサイズを拡大
    static const int PROFILE_NAME_Y = 5;    // プロファイル名を上部に表示
    static const int KEY_GRID_X = 5;        // キーグリッドを左端に近づける
    static const int KEY_GRID_Y = 25;       // キーグリッドY開始位置
    static const int KEY_ICON_SIZE = 48;    // キーアイコンサイズを拡大
    static const int KEY_GRID_SPACING = 55; // アイコン間隔を拡大（横幅最大活用）
    static const int ROTARY_X = 250;        // ロータリーを右端に移動
    static const int ROTARY_Y = 25;         // ロータリーもキーグリッドと同じY座標
    static const int ROTARY_SPACING = 70;

    static const u_int16_t BACK_COLOR = 0x0000;               // 背景色を黒に設定
    static const u_int16_t BACK_COLOR1 = 0b01111000001000001; // 背景色を黒に設定
    static const u_int16_t BACK_COLOR2 = 0b00001011111000001; // 背景色を黒に設定
    static const u_int16_t BACK_COLOR3 = 0b00001000001011111; // 背景色を黒に設定
    static const u_int16_t BACK_COLOR4 = 0b00111000001000001; // 背景色を黒に設定
    static const u_int16_t BACK_COLOR5 = 0b00001001111000001; // 背景色を黒に設定
    static const u_int16_t BACK_COLOR6 = 0b00001000001000111; // 背景色を黒に設定
private:
    static LGFX_ESP32S3WROOM1N8R2_SPI_ST7789 *tft;
    static DisplayState displayState;
    static LGFX_Sprite *headerScr;
    static LGFX_Sprite *iconScr1;
    static LGFX_Sprite *iconScr2;
    static LGFX_Sprite *iconScr3;
    static LGFX_Sprite *iconScr4;
    static LGFX_Sprite *rotaryScr;
    static LGFX_Sprite *sideScr1;
    static LGFX_Sprite *sideScr2;

public:
    // 初期化関数
    static void init(LGFX_ESP32S3WROOM1N8R2_SPI_ST7789 *tftDisplay);
    // ディスプレイ描画関数
    static void updateDisplay(int keyCode = 0, bool *keyPressed = nullptr,
                              CompleteProfile *profiles = nullptr, int profileCount = 0, int currentProfileIndex = 0);
    static void drawProfileIcons(CompleteProfile *profiles, int profileCount, int currentProfileIndex);
    static void drawKeypadGrid(CompleteProfile &currentProfile, bool *keyPressed);
    static void drawRotaryEncoders(CompleteProfile &currentProfile);
    static void drawSide();

    // 状態管理
    static void setKeyPressed(int keyIndex, bool pressed);
    static void setRotaryPressed(int rotaryIndex, bool pressed);
    static void clearScreen();
};
