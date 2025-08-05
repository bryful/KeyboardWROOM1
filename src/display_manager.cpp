#include "display_manager.hpp"
// --- アイコン画像データのインクルード ---
#include "./icondata/Icon_profile_prev.h"
#include "./icondata/Icon_profile_next.h"
#include "./icondata/Icon_undo.h"
#include "./icondata/Icon_brush.h"
#include "./icondata/Icon_pencil.h"
#include "./icondata/Icon_eraser.h"
#include "./icondata/Icon_paint.h"
#include "./icondata/Icon_shift.h"
#include "./icondata/Icon_ctrl.h"
#include "./icondata/Icon_alt.h"
#include "./icondata/Icon_mouse_right.h"
#include "./icondata/Icon_marquee.h"
#include "./icondata/Icon_lasso.h"
#include "./icondata/Icon_space.h"
#include "./icondata/Icon_delete.h"
#include "./icondata/Icon_fill.h"
#include "./icondata/Icon_selection.h"
#include "./icondata/Icon_zoom.h"
#include "./icondata/Icon_rotate.h"
#include "./icondata/Icon_brush_size.h"
#include "./icondata/Icon_pen.h"
#include "./icondata/Icon_direct.h"
#include "./icondata/Icon_pen_add.h"
#include "./icondata/Icon_pen_del.h"
#include "./icondata/Icon_anc.h"
#include "./icondata/Icon_join.h"
#include "./icondata/Icon_node.h"
#include "./icondata/Icon_spoit.h"
#include "./icondata/Icon_text.h"
#include "./icondata/Icon_SDelete.h"
#include "./icondata/Icon_SInvert.h"
#include "./icondata/Icon_girl.h"
#include "./icondata/Icon_mouse.h"
#include "./icondata/Icon_mouse_left.h"
#include "./icondata/Icon_quickmask.h"
#include "./icondata/Icon_redo.h"
#include "./icondata/Icon_timeline.h"
#include "./icondata/Icon_zoomArea.h"
#include "./icondata/Icon_SAll.h"
#include "./icondata/Icon_ae.h"
#include "./icondata/Icon_ai.h"
#include "./icondata/Icon_Alpaca.h"
#include "./icondata/Icon_gimp.h"
#include "./icondata/Icon_inkscape.h"
#include "./icondata/Icon_ps.h"
#include "./icondata/Icon_hand.h"
#include "./icondata/Icon_u.h"
#include "./icondata/Icon_e.h"
#include "./icondata/Icon_cut.h"
#include "./icondata/Icon_copy.h"
#include "./icondata/Icon_past.h"
#include "./icondata/Icon_select.h"
#include "./icondata/Icon_selectall.h"

// --- IconId→画像データのマッピングテーブル ---
const uint16_t *iconDataTable[] = {
    Icon_profile_prev_data, // ICON_PROFILE_PREV
    Icon_profile_next_data, // ICON_PROFILE_NEXT
    Icon_undo_data,         // ICON_UNDO
    Icon_brush_data,        // ICON_BRUSH
    Icon_pencil_data,       // ICON_PENCIL
    Icon_eraser_data,       // ICON_ERASER
    Icon_paint_data,        // ICON_PAINT
    Icon_shift_data,        // ICON_SHIFT
    Icon_ctrl_data,         // ICON_CTRL
    Icon_alt_data,          // ICON_ALT
    Icon_mouse_right_data,  // ICON_MOUSE_RIGHT
    Icon_marquee_data,      // ICON_MARQUEE
    Icon_lasso_data,        // ICON_LASSO
    Icon_space_data,        // ICON_SPACE
    Icon_delete_data,       // ICON_DELETE
    Icon_fill_data,         // ICON_DELETE2
    Icon_selection_data,    // ICON_SELECTION
    Icon_zoom_data,         // ICON_ZOOM
    Icon_rotate_data,       // ICON_ROTATE
    Icon_brush_size_data,   // ICON_BRUSH_SIZE
    Icon_pen_data,          // ICON_PEN
    Icon_direct_data,       // ICON_DIRECT
    Icon_pen_add_data,      // ICON_PEN_ADD
    Icon_pen_del_data,      // ICON_PEN_DEL
    Icon_anc_data,          // ICON_ANC
    Icon_join_data,         // ICON_JOIN
    Icon_node_data,         // ICON_NODE
    Icon_spoit_data,        // ICON_SPOIT
    Icon_text_data,         // ICON_TEXT
    Icon_SDelete_data,
    Icon_SInvert_data,
    Icon_girl_data,
    Icon_mouse_data,      // ICON_MOUSE
    Icon_mouse_left_data, // ICON_MOUSE_LEFT
    Icon_quickmask_data,  // ICON_QUICKMASK
    Icon_redo_data,       // ICON_REDO
    Icon_timeline_data,   // ICON_TIMELINE
    Icon_zoomArea_data,   // ICON_ZOOMAREA
    Icon_SAll_data,       // ICON_SALL
    Icon_ae_data,         // ICON_AE
    Icon_ai_data,         // ICON_AI
    Icon_Alpaca_data,     // ICON_ALPACA
    Icon_gimp_data,       // ICON_GIMP
    Icon_inkscape_data,   // ICON_INKSCAPE
    Icon_ps_data,         // ICON_PS
    Icon_hand_data,       // ICON_HAND
    Icon_u_data,          // ICON_U
    Icon_e_data,          // ICON_E
    Icon_cut_data,        // ICON_CUT
    Icon_copy_data,       // ICON_COPY
    Icon_past_data,       // ICON_PAST
    Icon_select_data,     // ICON_SELECT
    Icon_selectall_data,  // ICON_SELECTALL
    nullptr               // ICON_OTHER0
};

const u16_t headerBack[] = {
    TFT_BLUE,
    TFT_RED,
    TFT_RED,
    TFT_YELLOW,
    TFT_CYAN,
    TFT_VIOLET,
    TFT_ORANGE};
// 静的メンバ変数の定義
LGFX_ESP32S3WROOM1N8R2_SPI_ST7789 *DisplayManager::tft = nullptr;
// LGFX_Sprite DisplayManager::iconSprites[ICON_COUNT];
DisplayManager::DisplayState DisplayManager::displayState = {0};
LGFX_Sprite *DisplayManager::headerScr = nullptr;
LGFX_Sprite *DisplayManager::iconScr1 = nullptr;
LGFX_Sprite *DisplayManager::iconScr2 = nullptr;
LGFX_Sprite *DisplayManager::iconScr3 = nullptr;
LGFX_Sprite *DisplayManager::iconScr4 = nullptr;
LGFX_Sprite *DisplayManager::rotaryScr = nullptr;
LGFX_Sprite *DisplayManager::sideScr1 = nullptr;
LGFX_Sprite *DisplayManager::sideScr2 = nullptr;

void DisplayManager::init(LGFX_ESP32S3WROOM1N8R2_SPI_ST7789 *tftDisplay)
{
    if (!tftDisplay)
    {
        Serial.println("ERROR: tftDisplay is null!");
        return;
    }

    tft = tftDisplay;
    bool initResult = tft->init();

    if (!initResult)
    {
        Serial.println("ERROR: TFT init failed!");
    }

    tft->setRotation(3);
    tft->setColorDepth(16);

    // ★色順序修正を追加
    tft->setSwapBytes(true); // バイト順序を入れ替え

    tft->setBrightness(255);

    // 描画バッファーの初期化
    headerScr = new LGFX_Sprite(tft);
    headerScr->setColorDepth(16);
    headerScr->setSwapBytes(true); // バイト順序を入れ替え

    if (!headerScr->createSprite(320, 14))
    {
        headerScr = nullptr;
        Serial.println("ERROR: Failed to create header sprite!");
    }
    iconScr1 = new LGFX_Sprite(tft);
    iconScr1->setColorDepth(16);
    iconScr1->setSwapBytes(true); // バイト順序を入れ替え

    if (!iconScr1->createSprite(230, 61))
    {
        iconScr1 = nullptr;
        Serial.println("ERROR: Failed to create header sprite!");
    }
    iconScr2 = new LGFX_Sprite(tft);
    iconScr2->setColorDepth(16);
    iconScr2->setSwapBytes(true); // バイト順序を入れ替え

    if (!iconScr2->createSprite(230, 55))
    {
        iconScr2 = nullptr;
        Serial.println("ERROR: Failed to create header sprite!");
    }
    iconScr3 = new LGFX_Sprite(tft);
    iconScr3->setColorDepth(16);
    iconScr3->setSwapBytes(true); // バイト順序を入れ替え

    if (!iconScr3->createSprite(230, 55))
    {
        iconScr3 = nullptr;
        Serial.println("ERROR: Failed to create header sprite!");
    }
    iconScr4 = new LGFX_Sprite(tft);
    iconScr4->setColorDepth(16);  // バイト順序を入れ替え
    iconScr4->setSwapBytes(true); // バイト順序を入れ替え
    if (!iconScr4->createSprite(230, 55))
    {
        iconScr4 = nullptr;
        Serial.println("ERROR: Failed to create header sprite!");
    }
    rotaryScr = new LGFX_Sprite(tft);
    rotaryScr->setColorDepth(16);  // バイト順序を入れ替え
    rotaryScr->setSwapBytes(true); // バイト順序を入れ替え
    if (!rotaryScr->createSprite(60, 240 - 14))
    {
        rotaryScr = nullptr;
        Serial.println("ERROR: Failed to create header sprite!");
    }
    sideScr1 = new LGFX_Sprite(tft);
    sideScr1->setColorDepth(16);  // バイト順序を入れ替え
    sideScr1->setSwapBytes(true); // バイト順序を入れ替え
    if (!sideScr1->createSprite(10, 240 - 14))
    {
        sideScr1 = nullptr;
        Serial.println("ERROR: Failed to create header sprite!");
    }
    sideScr2 = new LGFX_Sprite(tft);
    sideScr2->setColorDepth(16);  // バイト順序を入れ替え
    sideScr2->setSwapBytes(true); // バイト順序を入れ替え
    if (!sideScr2->createSprite(10, 240 - 14))
    {
        sideScr2 = nullptr;
        Serial.println("ERROR: Failed to create header sprite!");
    }
    // ディスプレイ状態を初期化
    displayState.activeKeyDisplayed = false;
    displayState.lastActiveKeys = "";

    for (int i = 0; i < KEY_MATRIX_SIZE; i++)
    {
        displayState.keyPressed[i] = false;
    }
    for (int i = 0; i < ROTARY_COUNT; i++)
    {
        displayState.rotaryPressed[i] = false;
    }
    for (int i = 0; i < 10; i++)
    {
        displayState.profileActive[i] = false;
    }

    // 画面をクリア
    tft->fillScreen(TFT_BLACK);
}

void DisplayManager::updateDisplay(int keyCode, bool *keyPressed,
                                   CompleteProfile *profiles, int profileCount, int currentProfileIndex)
{
    if (!tft || !profiles || currentProfileIndex >= profileCount || currentProfileIndex < 0)
    {
        return;
    }

    // ディスプレイ状態を更新
    for (int i = 0; i < KEY_MATRIX_SIZE; i++)
    {
        displayState.keyPressed[i] = (keyCode & (1 << i)) != 0;
    }

    tft->startWrite();
    // tft->fillScreen(TFT_BLACK);

    if (profiles && profileCount > 0)
    {
        drawProfileIcons(profiles, profileCount, currentProfileIndex);
        drawKeypadGrid(profiles[currentProfileIndex], keyPressed);
        drawRotaryEncoders(profiles[currentProfileIndex]);
        drawSide();
    }
    tft->endWrite();
}

void DisplayManager::drawProfileIcons(CompleteProfile *profiles, int profileCount, int currentProfileIndex)
{
    if (!tft || !profiles)
    {
        return;
    }
    uint16_t c = profiles[currentProfileIndex].color;
    headerScr->fillScreen(c);
    uint16_t r = (c >> 11 & 0b11111); // 色を反転
    uint16_t g = (c >> 6 & 0b11111);  // 色を反転
    uint16_t b = (c & 0b11111);
    uint16_t c2 = TFT_WHITE; // 暗い色なら白文字
    uint16_t ch = ((r >> 1) << 11 & 0b1111100000000000) | ((g >> 1) << 6 & 0b0000011111100000) | (b >> 1 & 0b0000000000011111);
    if ((2 * r + 3 * g + b) / 6 > 16)
    {
        c2 = TFT_BLACK; // 明るい色なら黒文字
        ch = ((r << 1) << 11 & 0b1111100000000000) | ((g << 1) << 6 & 0b0000011111100000) | (b << 1 & 0b0000000000011111);
    }

    for (int i = 0; i < profileCount; i++)
    {
        if (i == currentProfileIndex)
        {
            headerScr->fillRect(4 + i * 10, 3, 8, 8, c2);
        }
        else
        {
            headerScr->fillRect(4 + i * 10, 3, 8, 8, ch);
        }
    }

    // headerScr->fillRect(15, 4, 8, 8, c2);
    headerScr->setTextColor(c2);
    headerScr->setTextSize(1);
    headerScr->drawString(profiles[currentProfileIndex].name, 80, 3);
    headerScr->pushSprite(0, 0);
}

void DisplayManager::drawKeypadGrid(CompleteProfile &currentProfile, bool *keyPressed)
{
    if (!tft)
        return;

    int x = 0;
    int y = 0;
    int idx = 0;
    int Xpos = 10;
    KeyConfig key;

    // 01
    iconScr1->fillScreen(TFT_BLACK);
    for (int i = 0; i < 4; i++)
    {
        idx = i;
        x = 5 + i * KEY_GRID_SPACING;
        y = 3;
        if (i == 2)
        {
            y += 8;
        }
        key = currentProfile.keys[idx];
        if ((int)key.iconId < ICON_COUNT && iconDataTable[(int)key.iconId])
        {
            iconScr1->pushImage(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, iconDataTable[(int)key.iconId]);
        }
        else
        {
            iconScr1->fillRect(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, TFT_BLACK);
        }
        iconScr1->drawRect(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, TFT_SKYBLUE);
        if (keyPressed && keyPressed[idx] && key.function != NO_KEY)
        {
            if (key.function == HOLD_KEY)
            {
                iconScr1->drawRect(x - 2, y - 2, KEY_ICON_SIZE + 4, KEY_ICON_SIZE + 4, TFT_ORANGE);
                iconScr1->drawRect(x - 1, y - 1, KEY_ICON_SIZE + 2, KEY_ICON_SIZE + 2, TFT_ORANGE);
            }
            else
            {
                iconScr1->drawRect(x - 2, y - 2, KEY_ICON_SIZE + 4, KEY_ICON_SIZE + 4, TFT_GREEN);
                iconScr1->drawRect(x - 1, y - 1, KEY_ICON_SIZE + 2, KEY_ICON_SIZE + 2, TFT_GREEN);
            }
        }
    }
    iconScr1->pushSprite(Xpos, 14);
    // 02
    iconScr2->fillScreen(TFT_BLACK);
    for (int i = 0; i < 4; i++)
    {
        idx = i + 4;
        x = 5 + i * KEY_GRID_SPACING;
        y = 3;
        ;
        key = currentProfile.keys[idx];
        if ((int)key.iconId < ICON_COUNT && iconDataTable[(int)key.iconId])
        {
            iconScr2->pushImage(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, iconDataTable[(int)key.iconId]);
        }
        else
        {
            iconScr2->fillRect(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, TFT_BLACK);
        }
        iconScr2->drawRect(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, TFT_SKYBLUE);

        if (keyPressed && keyPressed[idx] && key.function != NO_KEY)
        {
            if (key.function == HOLD_KEY)
            {
                iconScr2->drawRect(x - 2, y - 2, KEY_ICON_SIZE + 4, KEY_ICON_SIZE + 4, TFT_ORANGE);
                iconScr2->drawRect(x - 1, y - 1, KEY_ICON_SIZE + 2, KEY_ICON_SIZE + 2, TFT_ORANGE);
            }
            else
            {
                iconScr2->drawRect(x - 2, y - 2, KEY_ICON_SIZE + 4, KEY_ICON_SIZE + 4, TFT_GREEN);
                iconScr2->drawRect(x - 1, y - 1, KEY_ICON_SIZE + 2, KEY_ICON_SIZE + 2, TFT_GREEN);
            }
        }
    }
    iconScr2->pushSprite(Xpos, 14 + 61);
    // 03
    iconScr3->fillScreen(TFT_BLACK);
    for (int i = 0; i < 4; i++)
    {
        idx = i + 8;
        x = 5 + i * KEY_GRID_SPACING;
        y = 3;

        key = currentProfile.keys[idx];
        if ((int)key.iconId < ICON_COUNT && iconDataTable[(int)key.iconId])
        {
            iconScr3->pushImage(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, iconDataTable[(int)key.iconId]);
        }
        else
        {
            iconScr3->fillRect(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, TFT_BLACK);
        }
        iconScr3->drawRect(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, TFT_SKYBLUE);
        if (keyPressed && keyPressed[idx] && key.function != NO_KEY)
        {
            if (key.function == HOLD_KEY)
            {
                iconScr3->drawRect(x - 2, y - 2, KEY_ICON_SIZE + 4, KEY_ICON_SIZE + 4, TFT_ORANGE);
                iconScr3->drawRect(x - 1, y - 1, KEY_ICON_SIZE + 2, KEY_ICON_SIZE + 2, TFT_ORANGE);
            }
            else
            {
                iconScr3->drawRect(x - 2, y - 2, KEY_ICON_SIZE + 4, KEY_ICON_SIZE + 4, TFT_GREEN);
                iconScr3->drawRect(x - 1, y - 1, KEY_ICON_SIZE + 2, KEY_ICON_SIZE + 2, TFT_GREEN);
            }
        }
    }
    iconScr3->pushSprite(Xpos, 14 + 61 + 55);
    // 04
    iconScr4->fillScreen(TFT_BLACK);
    for (int i = 0; i < 4; i++)
    {
        idx = i + 12;
        x = 5 + i * KEY_GRID_SPACING;
        y = 3;
        if (i == 3)
        {
            x += 10;
        }

        key = currentProfile.keys[idx];
        if ((int)key.iconId < ICON_COUNT && iconDataTable[(int)key.iconId])
        {
            iconScr4->pushImage(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, iconDataTable[(int)key.iconId]);
        }
        else
        {
            iconScr4->fillRect(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, TFT_BLACK);
        }
        iconScr4->drawRect(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, TFT_SKYBLUE);
        if (keyPressed && keyPressed[idx] && key.function != NO_KEY)
        {
            if (key.function == HOLD_KEY)
            {
                iconScr4->drawRect(x - 2, y - 2, KEY_ICON_SIZE + 4, KEY_ICON_SIZE + 4, TFT_ORANGE);
                iconScr4->drawRect(x - 1, y - 1, KEY_ICON_SIZE + 2, KEY_ICON_SIZE + 2, TFT_ORANGE);
            }
            else
            {
                iconScr4->drawRect(x - 2, y - 2, KEY_ICON_SIZE + 4, KEY_ICON_SIZE + 4, TFT_GREEN);
                iconScr2->drawRect(x - 1, y - 1, KEY_ICON_SIZE + 2, KEY_ICON_SIZE + 2, TFT_GREEN);
            }
        }
    }
    iconScr4->pushSprite(Xpos, 14 + 61 + 55 + 55);
}

void DisplayManager::drawRotaryEncoders(CompleteProfile &currentProfile)
{
    if (!tft)
        return;

    rotaryScr->fillScreen(TFT_BLACK); // 画面全体を黒で塗りつぶす
    for (int i = 0; i < ROTARY_COUNT; i++)
    {
        int x = 5;
        int y = 18 + i * ROTARY_SPACING;
        RotaryConfig &rotary = currentProfile.rotaries[i];
        if ((int)rotary.iconId < ICON_COUNT && iconDataTable[(int)rotary.iconId])
        {
            rotaryScr->pushImage(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, iconDataTable[(int)rotary.iconId]);
        }
        else
        {
            rotaryScr->fillRect(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, TFT_BLACK);
        }
        // rotaryScr->drawRect(x, y, KEY_ICON_SIZE, KEY_ICON_SIZE, TFT_DARKGRAY);
        rotaryScr->drawCircle(
            x + KEY_ICON_SIZE / 2,
            y + KEY_ICON_SIZE / 2,
            KEY_ICON_SIZE / 2 + 5,
            TFT_SKYBLUE);
        // スイッチ押下状態のハイライト
        if (displayState.rotaryPressed[i])
        {
            // rotaryScr->drawRect(x - 2, y - 2, KEY_ICON_SIZE + 4, KEY_ICON_SIZE + 4, TFT_LIGHTGRAY);
            // rotaryScr->drawRect(x - 1, y - 1, KEY_ICON_SIZE + 2, KEY_ICON_SIZE + 2, TFT_LIGHTGRAY);
            rotaryScr->drawCircle(
                x + KEY_ICON_SIZE / 2,
                y + KEY_ICON_SIZE / 2,
                KEY_ICON_SIZE / 2 + 6,
                TFT_SKYBLUE);
            rotaryScr->drawCircle(
                x + KEY_ICON_SIZE / 2,
                y + KEY_ICON_SIZE / 2,
                KEY_ICON_SIZE / 2 + 7,
                TFT_SKYBLUE);
        }
    }
    rotaryScr->pushSprite(230 + 20, 14);
}

void DisplayManager::drawSide()
{
    if (!tft)
        return;

    sideScr1->fillScreen(TFT_BLACK); // 画面全体を黒で塗りつぶす
    sideScr2->fillScreen(TFT_BLACK); // 画面全体を黒で塗りつぶす
    int yy = 10;
    sideScr1->fillRect(2, yy, 6, 6, TFT_SKYBLUE);
    sideScr1->fillRect(2, 240 - 14 - 6 - yy, 6, 6, TFT_SKYBLUE);
    sideScr1->fillRect(2, 240 - 14 - 6 - yy, 6, 6, TFT_SKYBLUE);
    sideScr1->fillRect(2, (240 - 14) / 2 - 1, 8, 2, TFT_SKYBLUE);

    yy = 40;
    sideScr2->fillRect(2, yy, 6, 4, TFT_SKYBLUE);
    sideScr2->fillRect(2, 240 - 14 - 4 - yy, 6, 4, TFT_SKYBLUE);
    sideScr2->fillRect(2, (240 - 14) / 2 - 2, 6, 4, TFT_SKYBLUE);

    sideScr1->pushSprite(0, 14);
    sideScr2->pushSprite(310, 14);
}

void DisplayManager::setKeyPressed(int keyIndex, bool pressed)
{
    if (keyIndex >= 0 && keyIndex < KEY_MATRIX_SIZE)
    {
        displayState.keyPressed[keyIndex] = pressed;
    }
}

void DisplayManager::setRotaryPressed(int rotaryIndex, bool pressed)
{
    if (rotaryIndex >= 0 && rotaryIndex < ROTARY_COUNT)
    {
        displayState.rotaryPressed[rotaryIndex] = pressed;
    }
}

void DisplayManager::clearScreen()
{
    if (tft)
    {
        tft->fillScreen(TFT_BLACK);
    }
}
