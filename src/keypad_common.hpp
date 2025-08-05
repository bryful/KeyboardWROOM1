#ifndef KEYPAD_COMMON_HPP
#define KEYPAD_COMMON_HPP

#include <stdint.h>

// アイコンID定義
enum IconId
{
    ICON_PROFILE_PREV = 0, // プロファイル前
    ICON_PROFILE_NEXT,     // プロファイル次
    ICON_UNDO,             // Undo
    ICON_BRUSH,            // ブラシ
    ICON_PENCIL,           // 鉛筆
    ICON_ERASER,           // 消しゴム
    ICON_PAINT,            // 塗りつぶし
    ICON_SHIFT,            // Shift
    ICON_CTRL,             // Ctrl
    ICON_ALT,              // Alt
    ICON_MOUSE_RIGHT,      // マウス右クリック
    ICON_MARQUEE,          // 選択/マーキー
    ICON_LASSO,            // 投げ縄
    ICON_SPACE,            // スペース
    ICON_DELETE,           // 削除
    ICON_FILL,             // Alt+Delete専用
    ICON_SELECTION,        // 選択ツール
    ICON_ZOOM,             // ズーム
    ICON_ROTATE,           // 回転
    ICON_BRUSH_SIZE,       // ブラシサイズ/太さ
    ICON_PEN,              // ペン（Illustrator/Inkscape）
    ICON_DIRECT,           // ダイレクト選択（Illustrator）
    ICON_PEN_ADD,          // パス追加（Illustrator）
    ICON_PEN_DEL,          // パス削除（Illustrator）
    ICON_ANC,              // アンカーポイント（Illustrator）
    ICON_JOIN,             // パス結合（Illustrator）
    ICON_NODE,             // ノード（Inkscape）
    ICON_SPOIT,            // スポイト（Inkscape）
    ICON_TEXT,             // テキスト（Inkscape）
    ICON_SDELETE,
    ICON_SINVERT,
    ICON_GIRL, // 女の子アイコン（例）
    ICON_MOUSE,
    ICON_MOUSE_LEFT,
    ICON_QUICKMASK,
    ICON_REDO,
    ICON_TIMELINE,
    ICON_ZOOMAREA,
    ICON_SALL,
    ICON_AE,        // After Effects
    ICON_AI,        // Illustrator
    ICON_ALPACA,    // Alpaca
    ICON_GIMP,      // GIMP
    ICON_INKSCAPE,  // Inkscape
    ICON_PS,        // Photoshop
    ICON_HAND,      // Hand tool
    ICON_U,         // Uキー（例）
    ICON_E,         // Eキー（例）
    ICON_CUT,       // Cut
    ICON_COPY,      // Copy
    ICON_PASTE,     // Paste
    ICON_SELECT,    // Select
    ICON_SELECTALL, // Select All
    ICON_OTHER1     // 予備1
};

// キー機能の種類
enum KeyFunction
{
    NO_KEY,
    HOLD_KEY,
    COMBO_KEY,
    MACRO_KEY,
    MOUSE_LEFT_CLICK,  // マウス左クリック
    MOUSE_RIGHT_CLICK, // マウス右クリック
    PREF_CHANGE_KEY,   // プロファイル切り替えキー
    PREF_PREV_KEY,     // プロファイル逆順切り替えキー
    ROTARY_SWITCH      // ロータリーエンコーダーのスイッチ
};

// KeyConfig構造体を先に定義
struct KeyConfig
{
    KeyFunction function;
    uint8_t primaryKey;
    uint8_t modifiers;
    IconId iconId; // アイコンID
};

// ロータリーエンコーダーの機能種類
struct RotaryConfig
{
    uint8_t clockwiseKey;             // 時計回り時のキー
    uint8_t counterClockwiseKey;      // 反時計回り時のキー
    uint8_t clockwiseModifier;        // 時計回り時のモディファイア
    uint8_t counterClockwiseModifier; // 反時計回り時のモディファイア
    IconId iconId;                    // アイコンID
    KeyFunction function;
    uint8_t primaryKey;
    uint8_t modifiers;
};

#define KEY_MATRIX_SIZE 16
#define MAX_KEYS 12
#define ROTARY_COUNT 3

// 統合プロファイル構造体
struct CompleteProfile
{
    const char *name;
    uint16_t color;
    KeyConfig keys[KEY_MATRIX_SIZE];
    RotaryConfig rotaries[ROTARY_COUNT];
};
#endif