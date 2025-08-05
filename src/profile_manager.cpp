#include "USBHIDKeyboard.h" // HIDキーコード定義（JIS配列対応）
#include "USBHIDKeyboard_JIS.h"

#include "keypad_common.hpp"   // 共通定義（キーID・アイコンID等）
#include "profile_manager.hpp" // プロファイル管理クラス
#include <Preferences.h>       // ESP32 Preferences（NVS）ライブラリ

// =============================
// プリセットプロファイル定義
// =============================

static const String profileFileName = "/profiles.json"; // デフォルトプロファイルファイル名

const String initJson = "{\"profiles\":[{\"name\":\"explorer\",\"color\":65535,\"keys\":[6,0,0,0,5,0,0,1,2,4,1,16,2,76,1,15,2,58,14,32,2,59,14,33,2,60,14,34,2,61,14,48,2,62,14,36,2,63,14,50,2,29,0,18,2,24,0,46,2,6,1,14,2,26,1,38,2,25,1,39,2,29,1,2],\"rotaries\":[0,0,0,0,49,0,0,0,0,0,0,0,49,0,0,0,0,0,0,0,49,0,0,0]},{\"name\":\"After Effects\",\"color\":31,\"keys\":[6,0,0,0,5,0,0,1,2,76,4,16,2,76,0,15,1,0,2,7,1,0,1,8,1,0,4,9,4,0,0,10,2,25,0,17,2,11,0,45,2,29,0,18,2,24,0,46,2,8,0,47,2,15,0,12,1,44,0,13,2,29,1,2],\"rotaries\":[45,9,0,0,35,2,74,0,75,78,0,0,19,0,0,0,0,0,0,0,49,0,0,0]},{\"name\":\"Photoshop\",\"color\":63488,\"keys\":[6,0,0,0,5,0,0,1,2,76,4,16,2,76,0,15,1,0,1,8,1,0,2,7,1,0,4,9,4,0,0,10,2,6,0,4,2,10,0,6,2,5,0,3,2,4,1,37,2,12,6,30,2,7,1,29,1,44,0,13,2,29,1,31],\"rotaries\":[30,31,0,0,19,0,0,0,47,48,0,0,20,0,0,0,87,86,1,1,18,0,0,0]},{\"name\":\"Illustrator\",\"color\":65504,\"keys\":[6,0,0,0,5,0,0,1,2,25,0,17,2,76,0,15,1,0,2,7,1,0,1,8,1,0,4,9,4,0,0,10,2,19,0,21,2,4,0,22,2,46,2,23,2,45,0,24,2,6,2,25,2,13,1,26,1,44,0,13,2,29,1,2],\"rotaries\":[46,45,1,1,18,2,39,1,0,0,0,0,19,2,30,6,47,48,0,0,20,0,0,0]},{\"name\":\"GIMP\",\"color\":2047,\"keys\":[6,0,0,0,5,0,0,1,2,54,1,16,2,76,0,15,1,0,2,7,1,0,1,8,1,0,4,9,4,0,0,10,2,19,0,3,2,17,0,4,2,8,2,5,2,5,2,6,2,21,0,11,2,9,0,12,1,44,0,13,2,29,1,2],\"rotaries\":[78,75,1,1,19,2,30,2,47,48,0,0,20,2,49,0,87,86,0,0,18,2,13,6]},{\"name\":\"Inkscape\",\"color\":63519,\"keys\":[6,0,0,0,5,0,0,1,2,17,0,17,2,76,0,15,1,0,2,7,1,0,1,8,1,0,4,9,4,0,0,10,2,5,0,21,2,4,0,27,2,7,0,28,2,24,0,6,2,29,0,11,2,23,0,28,1,44,0,13,2,29,1,2],\"rotaries\":[46,45,1,1,18,2,30,0,0,0,0,0,19,2,30,6,54,55,0,0,20,0,0,0]},{\"name\":\"FireAlpaca\",\"color\":64512,\"keys\":[6,0,0,0,5,0,0,1,2,73,0,16,2,76,0,15,1,0,2,7,1,0,1,8,1,0,4,9,4,0,0,10,2,5,0,3,2,5,2,4,2,8,0,5,2,10,0,6,2,16,0,11,2,15,0,12,1,44,0,13,2,29,1,2],\"rotaries\":[79,80,0,0,19,2,82,0,47,48,0,0,20,2,49,0,87,86,1,1,18,2,39,1]}]}";

// プロファイル定義
// 各プロファイルを個別に定義
static const CompleteProfile ExplorerProfile = {
    "explorer",
    TFT_WHITE,
    {{PREF_PREV_KEY, HID_KEY_NONE, HID_KEY_NONE, ICON_PROFILE_PREV},
     {PREF_CHANGE_KEY, HID_KEY_NONE, HID_KEY_NONE, ICON_PROFILE_NEXT},
     {COMBO_KEY, HID_KEY_A, KEYBOARD_MODIFIER_LEFTCTRL, ICON_SELECTALL},
     {COMBO_KEY, HID_KEY_DELETE, KEYBOARD_MODIFIER_LEFTCTRL, ICON_DELETE},

     {COMBO_KEY, HID_KEY_F1, KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTCTRL | KEYBOARD_MODIFIER_RIGHTALT, ICON_PS},
     {COMBO_KEY, HID_KEY_F2, KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTCTRL | KEYBOARD_MODIFIER_RIGHTALT, ICON_AI},
     {COMBO_KEY, HID_KEY_F3, KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTCTRL | KEYBOARD_MODIFIER_RIGHTALT, ICON_AE},
     {COMBO_KEY, HID_KEY_F4, KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTCTRL | KEYBOARD_MODIFIER_RIGHTALT, ICON_ALPACA},

     {COMBO_KEY, HID_KEY_F5, KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTCTRL | KEYBOARD_MODIFIER_RIGHTALT, ICON_GIMP},
     {COMBO_KEY, HID_KEY_F6, KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTCTRL | KEYBOARD_MODIFIER_RIGHTALT, ICON_INKSCAPE},
     {COMBO_KEY, HID_KEY_Z, HID_KEY_NONE, ICON_ZOOM},
     {COMBO_KEY, HID_KEY_U, HID_KEY_NONE, ICON_U},

     {COMBO_KEY, HID_KEY_C, KEYBOARD_MODIFIER_LEFTCTRL, ICON_COPY},
     {COMBO_KEY, HID_KEY_X, KEYBOARD_MODIFIER_LEFTCTRL, ICON_CUT},
     {COMBO_KEY, HID_KEY_V, KEYBOARD_MODIFIER_LEFTCTRL, ICON_PASTE},
     {COMBO_KEY, HID_KEY_Z, KEYBOARD_MODIFIER_LEFTCTRL, ICON_UNDO}},

    {{HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, ICON_OTHER1, NO_KEY, HID_KEY_NONE, HID_KEY_NONE},
     {HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, ICON_OTHER1, NO_KEY, HID_KEY_NONE, HID_KEY_NONE},
     {HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE, ICON_OTHER1, NO_KEY, HID_KEY_NONE, HID_KEY_NONE}}};

static const CompleteProfile afterEffectsProfile = {
    "After Effects",
    TFT_BLUE,
    {{PREF_PREV_KEY, HID_KEY_NONE, HID_KEY_NONE, ICON_PROFILE_PREV},
     {PREF_CHANGE_KEY, HID_KEY_NONE, HID_KEY_NONE, ICON_PROFILE_NEXT},
     {COMBO_KEY, HID_KEY_DELETE, KEYBOARD_MODIFIER_LEFTALT, ICON_FILL},
     {COMBO_KEY, HID_KEY_DELETE, HID_KEY_NONE, ICON_DELETE},
     {HOLD_KEY, HID_KEY_NONE, KEYBOARD_MODIFIER_LEFTSHIFT, ICON_SHIFT},
     {HOLD_KEY, HID_KEY_NONE, KEYBOARD_MODIFIER_LEFTCTRL, ICON_CTRL},
     {HOLD_KEY, HID_KEY_NONE, KEYBOARD_MODIFIER_LEFTALT, ICON_ALT},
     {MOUSE_RIGHT_CLICK, HID_KEY_NONE, 0, ICON_MOUSE_RIGHT},
     {COMBO_KEY, HID_KEY_V, HID_KEY_NONE, ICON_SELECTION},
     {COMBO_KEY, HID_KEY_H, HID_KEY_NONE, ICON_HAND},
     {COMBO_KEY, HID_KEY_Z, HID_KEY_NONE, ICON_ZOOM},
     {COMBO_KEY, HID_KEY_U, HID_KEY_NONE, ICON_U},
     {COMBO_KEY, HID_KEY_E, HID_KEY_NONE, ICON_E},
     {COMBO_KEY, HID_KEY_L, HID_KEY_NONE, ICON_LASSO},
     {HOLD_KEY, HID_KEY_SPACE, HID_KEY_NONE, ICON_SPACE},
     {COMBO_KEY, HID_KEY_Z, KEYBOARD_MODIFIER_LEFTCTRL, ICON_UNDO}},
    {{HID_KEY_MINUS, HID_KEY_6, HID_KEY_NONE, HID_KEY_NONE, ICON_TIMELINE, COMBO_KEY, HID_KEY_HOME, HID_KEY_NONE},
     {HID_KEY_PAGEUP, HID_KEY_PAGEDOWN, HID_KEY_NONE, HID_KEY_NONE, ICON_ROTATE, NO_KEY, HID_KEY_NONE, HID_KEY_NONE},
     {0, 0, 0, 0, ICON_OTHER1, NO_KEY, HID_KEY_NONE, HID_KEY_NONE}}};

static const CompleteProfile photoshopProfile = {
    "Photoshop",
    TFT_RED,
    {{PREF_PREV_KEY, 0, 0, ICON_PROFILE_PREV},
     {PREF_CHANGE_KEY, 0, 0, ICON_PROFILE_NEXT},
     {COMBO_KEY, HID_KEY_DELETE, KEYBOARD_MODIFIER_LEFTALT, ICON_FILL},
     {COMBO_KEY, HID_KEY_DELETE, 0, ICON_DELETE},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTCTRL, ICON_CTRL},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTSHIFT, ICON_SHIFT},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTALT, ICON_ALT},
     {MOUSE_RIGHT_CLICK, 0, 0, ICON_MOUSE_RIGHT},
     {COMBO_KEY, HID_KEY_C, 0, ICON_PENCIL},
     {COMBO_KEY, HID_KEY_G, 0, ICON_PAINT},
     {COMBO_KEY, HID_KEY_B, 0, ICON_BRUSH},
     {COMBO_KEY, HID_KEY_A, KEYBOARD_MODIFIER_LEFTCTRL, ICON_SALL},
     {COMBO_KEY, HID_KEY_I, KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_LEFTCTRL, ICON_SINVERT},
     {COMBO_KEY, HID_KEY_D, KEYBOARD_MODIFIER_LEFTCTRL, ICON_SDELETE},
     {HOLD_KEY, HID_KEY_SPACE, 0, ICON_SPACE},
     {COMBO_KEY, HID_KEY_Z, KEYBOARD_MODIFIER_LEFTCTRL, ICON_GIRL}},
    {{HID_KEY_1, HID_KEY_2, 0, 0, ICON_ROTATE, NO_KEY, 0, 0},
     {HID_KEY_LEFTBRACE, HID_KEY_RIGHTBRACE, 0, 0, ICON_BRUSH_SIZE, NO_KEY, 0, 0},
     {HID_KEY_KP_PLUS, HID_KEY_KP_MINUS, KEYBOARD_MODIFIER_LEFTCTRL, KEYBOARD_MODIFIER_LEFTCTRL, ICON_ZOOM, NO_KEY, 0, 0}}};

static const CompleteProfile illustratorProfile = {
    "Illustrator",
    TFT_YELLOW,
    {{PREF_PREV_KEY, 0, 0, ICON_PROFILE_PREV},
     {PREF_CHANGE_KEY, 0, 0, ICON_PROFILE_NEXT},
     {COMBO_KEY, HID_KEY_V, 0, ICON_SELECTION},
     {COMBO_KEY, HID_KEY_DELETE, 0, ICON_DELETE},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTSHIFT, ICON_SHIFT},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTCTRL, ICON_CTRL},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTALT, ICON_ALT},
     {MOUSE_RIGHT_CLICK, 0, 0, ICON_MOUSE_RIGHT},
     {COMBO_KEY, HID_KEY_P, 0, ICON_PEN},
     {COMBO_KEY, HID_KEY_A, 0, ICON_DIRECT},
     {COMBO_KEY, HID_KEY_EQUAL, KEYBOARD_MODIFIER_LEFTSHIFT, ICON_PEN_ADD},
     {COMBO_KEY, HID_KEY_MINUS, 0, ICON_PEN_DEL},
     {COMBO_KEY, HID_KEY_C, KEYBOARD_MODIFIER_LEFTSHIFT, ICON_ANC},
     {COMBO_KEY, HID_KEY_J, KEYBOARD_MODIFIER_LEFTCTRL, ICON_JOIN},
     {HOLD_KEY, HID_KEY_SPACE, 0, ICON_SPACE},
     {COMBO_KEY, HID_KEY_Z, KEYBOARD_MODIFIER_LEFTCTRL, ICON_UNDO}},
    {{HID_KEY_EQUAL, HID_KEY_MINUS, KEYBOARD_MODIFIER_LEFTCTRL, KEYBOARD_MODIFIER_LEFTCTRL, ICON_ZOOM, COMBO_KEY, HID_KEY_0, KEYBOARD_MODIFIER_LEFTCTRL},
     {0, 0, 0, 0, ICON_ROTATE, COMBO_KEY, HID_KEY_1, KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTSHIFT},
     {HID_KEY_LEFTBRACE, HID_KEY_RIGHTBRACE, 0, 0, ICON_BRUSH_SIZE, NO_KEY, 0, 0}}};

// --- GIMP用プロファイル ---

static const CompleteProfile gimpProfile = {
    "GIMP",
    TFT_CYAN,
    {{PREF_PREV_KEY, 0, 0, ICON_PROFILE_PREV},
     {PREF_CHANGE_KEY, 0, 0, ICON_PROFILE_NEXT},
     {COMBO_KEY, HID_KEY_COMMA, KEYBOARD_MODIFIER_LEFTCTRL, ICON_FILL},
     {COMBO_KEY, HID_KEY_DELETE, 0, ICON_DELETE},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTSHIFT, ICON_SHIFT},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTCTRL, ICON_CTRL},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTALT, ICON_ALT},
     {MOUSE_RIGHT_CLICK, 0, 0, ICON_MOUSE_RIGHT},
     {COMBO_KEY, HID_KEY_P, 0, ICON_BRUSH},
     {COMBO_KEY, HID_KEY_N, 0, ICON_PENCIL},
     {COMBO_KEY, HID_KEY_E, KEYBOARD_MODIFIER_LEFTSHIFT, ICON_ERASER},
     {COMBO_KEY, HID_KEY_B, KEYBOARD_MODIFIER_LEFTSHIFT, ICON_PAINT},
     {COMBO_KEY, HID_KEY_R, 0, ICON_MARQUEE},
     {COMBO_KEY, HID_KEY_F, 0, ICON_LASSO},
     {HOLD_KEY, HID_KEY_SPACE, 0, ICON_SPACE},
     {COMBO_KEY, HID_KEY_Z, KEYBOARD_MODIFIER_LEFTCTRL, ICON_UNDO}},
    {{HID_KEY_PAGEDOWN, HID_KEY_PAGEUP, KEYBOARD_MODIFIER_LEFTCTRL, KEYBOARD_MODIFIER_LEFTCTRL, ICON_ROTATE, COMBO_KEY, HID_KEY_1, KEYBOARD_MODIFIER_LEFTSHIFT},
     {HID_KEY_LEFTBRACE, HID_KEY_RIGHTBRACE, 0, 0, ICON_BRUSH_SIZE, COMBO_KEY, HID_KEY_BACKSLASH, 0},
     {HID_KEY_KP_PLUS, HID_KEY_KP_MINUS, 0, 0, ICON_ZOOM, COMBO_KEY, HID_KEY_J, KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTSHIFT}}};

// --- 追加プロファイル: Inkscape ---
static const CompleteProfile inkscapeProfile = {
    "Inkscape",
    TFT_VIOLET,
    {{PREF_PREV_KEY, 0, 0, ICON_PROFILE_PREV},
     {PREF_CHANGE_KEY, 0, 0, ICON_PROFILE_NEXT},
     {COMBO_KEY, HID_KEY_N, 0, ICON_SELECTION},
     {COMBO_KEY, HID_KEY_DELETE, 0, ICON_DELETE},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTSHIFT, ICON_SHIFT},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTCTRL, ICON_CTRL},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTALT, ICON_ALT},
     {MOUSE_RIGHT_CLICK, 0, 0, ICON_MOUSE_RIGHT},
     {COMBO_KEY, HID_KEY_B, 0, ICON_PEN},
     {COMBO_KEY, HID_KEY_A, 0, ICON_NODE},
     {COMBO_KEY, HID_KEY_D, 0, ICON_SPOIT},
     {COMBO_KEY, HID_KEY_U, 0, ICON_PAINT},
     {COMBO_KEY, HID_KEY_Z, 0, ICON_MARQUEE},
     {COMBO_KEY, HID_KEY_T, 0, ICON_TEXT},
     {HOLD_KEY, HID_KEY_SPACE, 0, ICON_SPACE},
     {COMBO_KEY, HID_KEY_Z, KEYBOARD_MODIFIER_LEFTCTRL, ICON_UNDO}},
    {{HID_KEY_EQUAL, HID_KEY_MINUS, KEYBOARD_MODIFIER_LEFTCTRL, KEYBOARD_MODIFIER_LEFTCTRL, ICON_ZOOM, COMBO_KEY, HID_KEY_1, 0},
     {0, 0, 0, 0, ICON_ROTATE, COMBO_KEY, HID_KEY_1, KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTSHIFT},
     {HID_KEY_COMMA, HID_KEY_PERIOD, 0, 0, ICON_BRUSH_SIZE, NO_KEY, 0, 0}}};

// --- 追加プロファイル: FireAlpaca ---
static const CompleteProfile fireAlpacaProfile = {
    "FireAlpaca",
    TFT_ORANGE,
    {{PREF_PREV_KEY, 0, 0, ICON_PROFILE_PREV},
     {PREF_CHANGE_KEY, 0, 0, ICON_PROFILE_NEXT},
     {COMBO_KEY, HID_KEY_INSERT, 0, ICON_FILL},
     {COMBO_KEY, HID_KEY_DELETE, 0, ICON_DELETE},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTSHIFT, ICON_SHIFT},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTCTRL, ICON_CTRL},
     {HOLD_KEY, 0, KEYBOARD_MODIFIER_LEFTALT, ICON_ALT},
     {MOUSE_RIGHT_CLICK, 0, 0, ICON_MOUSE_RIGHT},
     {COMBO_KEY, HID_KEY_B, 0, ICON_BRUSH},
     {COMBO_KEY, HID_KEY_B, KEYBOARD_MODIFIER_LEFTSHIFT, ICON_PENCIL},
     {COMBO_KEY, HID_KEY_E, 0, ICON_ERASER},
     {COMBO_KEY, HID_KEY_G, 0, ICON_PAINT},
     {COMBO_KEY, HID_KEY_M, 0, ICON_MARQUEE},
     {COMBO_KEY, HID_KEY_L, 0, ICON_LASSO},
     {HOLD_KEY, HID_KEY_SPACE, 0, ICON_SPACE},
     {COMBO_KEY, HID_KEY_Z, KEYBOARD_MODIFIER_LEFTCTRL, ICON_UNDO}},
    {{HID_KEY_ARROWRIGHT, HID_KEY_ARROWLEFT, 0, 0, ICON_ROTATE, COMBO_KEY, HID_KEY_ARROWUP, 0},
     {HID_KEY_LEFTBRACE, HID_KEY_RIGHTBRACE, 0, 0, ICON_BRUSH_SIZE, COMBO_KEY, HID_KEY_BACKSLASH, 0},
     {HID_KEY_KP_PLUS, HID_KEY_KP_MINUS, KEYBOARD_MODIFIER_LEFTCTRL, KEYBOARD_MODIFIER_LEFTCTRL, ICON_ZOOM, COMBO_KEY, HID_KEY_0, KEYBOARD_MODIFIER_LEFTCTRL}}};

CompleteProfile ProfileManager::profiles[] = {
    ExplorerProfile,
    afterEffectsProfile,
    photoshopProfile,
    illustratorProfile,
    gimpProfile,
    inkscapeProfile,
    fireAlpacaProfile};
const int ProfileManager::profileCount = sizeof(ProfileManager::profiles) / sizeof(ProfileManager::profiles[0]);
int ProfileManager::currentProfileIndex = 0;

// --- プロファイル管理関数 ---
void ProfileManager::initDefaultProfiles()
{

    if (loadProfilesFromFile(profileFileName))
    {
        currentProfileIndex = 0;
        Serial.println("Default profiles initialized successfully");
    }
    else
    {
        saveProfilesToFile(initJson);
        Serial.println("Failed to initialize default profiles");
    }
};
void ProfileManager::saveProfiles()
{
    if (saveProfilesToFile(profileFileName))
    {
        Serial.println("Profiles saved successfully");
    }
    else
    {
        Serial.println("Failed to save profiles");
    }
};

void ProfileManager::loadProfiles()
{
    // LittleFSにハード的な障害があるので機能を封印
    if (loadProfilesFromFile(profileFileName))
    {
        Serial.println("Profiles loaded successfully");
    }
    else
    {
        saveProfilesToFile(profileFileName);

        Serial.println("Failed to load profiles");
    }
};
int ProfileManager::indexOfProfile(const char *name)
{
    for (int i = 0; i < profileCount; i++)
    {
        if (strcmp(profiles[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}
void ProfileManager::setActiveProfile(int index)
{
    if (index < 0 || index >= profileCount)
    {
        Serial.println("Invalid profile index");
        return;
    }
    currentProfileIndex = index % profileCount;

    // ★Preferencesに現在のプロファイル名を保存
    Preferences preferences;
    preferences.begin("keypad", false);
    preferences.putString("activeProfile", profiles[currentProfileIndex].name);
    preferences.end();
}
void ProfileManager::setActiveProfile(const char *name)
{
    setActiveProfile(indexOfProfile(name));
}
void ProfileManager::switchToNextProfile()
{
    setActiveProfile((currentProfileIndex + 1) % profileCount);
}

void ProfileManager::switchToPrevProfile()
{
    setActiveProfile((currentProfileIndex - 1 + profileCount) % profileCount);
}
CompleteProfile &ProfileManager::getCurrentProfile()
{
    return profiles[currentProfileIndex];
}

// --- JSON処理関数 ---
String ProfileManager::profileToJson(const CompleteProfile &profile)
{
    JsonDocument doc;

    doc["name"] = profile.name;
    doc["color"] = profile.color; // プロファイルの色を追加
    // キー設定をJSONに変換
    JsonArray keys = doc["keys"].to<JsonArray>();
    for (int i = 0; i < KEY_MATRIX_SIZE; i++)
    {
        keys.add(profile.keys[i].function);
        keys.add(profile.keys[i].primaryKey);
        keys.add(profile.keys[i].modifiers);
        keys.add(profile.keys[i].iconId);
    }

    // ロータリーエンコーダー設定をJSONに変換
    JsonArray rotaries = doc["rotaries"].to<JsonArray>();
    for (int i = 0; i < ROTARY_COUNT; i++)
    {
        rotaries.add(profile.rotaries[i].clockwiseKey);
        rotaries.add(profile.rotaries[i].counterClockwiseKey);
        rotaries.add(profile.rotaries[i].clockwiseModifier);
        rotaries.add(profile.rotaries[i].counterClockwiseModifier);
        rotaries.add(profile.rotaries[i].iconId);
        rotaries.add(profile.rotaries[i].function);
        rotaries.add(profile.rotaries[i].primaryKey);
        rotaries.add(profile.rotaries[i].modifiers);
    }

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

bool ProfileManager::jsonToProfile(const String &jsonString, CompleteProfile profile)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error)
    {
        return false;
    }

    // プロファイル名とアイコンIDを設定
    profile.name = doc["name"].as<const char *>();
    profile.color = doc["color"].as<uint16_t>(); // プロファイルの色を復元

    // キー設定を復元
    JsonArray keys = doc["keys"];

    int cnt = keys.size();
    int idx = 0;
    for (int i = 0; i < KEY_MATRIX_SIZE; i++)
    {
        profile.keys[i].function = (KeyFunction)keys[idx].as<int>();
        idx++;
        if (idx >= cnt)
            break;
        profile.keys[i].primaryKey = keys[idx].as<uint8_t>();
        idx++;
        if (idx >= cnt)
            break;
        profile.keys[i].modifiers = keys[idx].as<uint8_t>();
        idx++;
        if (idx >= cnt)
            break;
        profile.keys[i].iconId = (IconId)keys[idx].as<uint8_t>();
        idx++;
        if (idx >= cnt)
            break;
    }

    // ロータリーエンコーダー設定を復元
    JsonArray rotaries = doc["rotaries"];
    idx = 0;
    cnt = rotaries.size();
    for (int i = 0; i < ROTARY_COUNT; i++)
    {
        profile.rotaries[i].clockwiseKey = rotaries[idx].as<uint8_t>();
        idx++;
        if (idx >= cnt)
            break;

        profile.rotaries[i].counterClockwiseKey = rotaries[idx].as<uint8_t>();
        idx++;
        if (idx >= cnt)
            break;

        profile.rotaries[i].clockwiseModifier = rotaries[idx].as<uint8_t>();
        idx++;
        if (idx >= cnt)
            break;

        profile.rotaries[i].counterClockwiseModifier = rotaries[idx].as<uint8_t>();
        idx++;
        if (idx >= cnt)
            break;

        profile.rotaries[i].iconId = (IconId)rotaries[idx].as<uint8_t>();
        idx++;
        if (idx >= cnt)
            break;

        profile.rotaries[i].function = (KeyFunction)rotaries[idx].as<int>();
        idx++;
        if (idx >= cnt)
            break;

        profile.rotaries[i].primaryKey = rotaries[idx].as<uint8_t>();
        idx++;
        if (idx >= cnt)
            break;

        profile.rotaries[i].modifiers = rotaries[idx].as<uint8_t>();
        idx++;
        if (idx >= cnt)
            break;
    }

    return true;
}

String ProfileManager::activeProfilesToJson()
{
    return profileToJson(getCurrentProfile());
}

String ProfileManager::allProfilesToJson()
{
    JsonDocument doc;
    JsonArray profilesArray = doc["profiles"].to<JsonArray>();

    for (int i = 0; i < profileCount; i++)
    {
        String profileJson = profileToJson(profiles[i]);
        JsonDocument profileDoc;
        deserializeJson(profileDoc, profileJson);
        profilesArray.add(profileDoc);
    }

    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

bool ProfileManager::jsonToAllProfiles(const String &jsonString)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error)
    {
        return false;
    }

    JsonArray profilesArray = doc["profiles"];
    int loadedCount = 0;

    for (JsonObject profileObj : profilesArray)
    {
        if (loadedCount >= profileCount)
            break;

        String profileJson;
        serializeJson(profileObj, profileJson);

        if (jsonToProfile(profileJson, profiles[loadedCount]))
        {
            loadedCount++;
        }
    }

    return true;
}

bool ProfileManager::saveProfilesToFile(const String &filename)
{
    bool ret = false;

    File file = LittleFS.open(filename, "w");
    if (file)
    {
        // プロファイルをJSON形式でシリアライズ
        String jsonString = allProfilesToJson();
        file.print(jsonString);
        file.close();
        ret = true;
    }
    else
    {
        Serial.println("Failed to open profile file for writing");
    }
    return ret;
}
bool ProfileManager::saveFile(const String &filename, const String data)
{
    bool ret = false;

    File file = LittleFS.open(filename, "w");
    if (file)
    {
        file.print(data);
        file.close();
        ret = true;
    }
    return ret;
}
bool ProfileManager::loadFile(const String &filename, String *data)
{
    bool ret = false;
    File file = LittleFS.open(filename, "r");
    if (file)
    {
        *data = file.readString();
        file.close();
        ret = true;
    }
    return ret;
}

bool ProfileManager::loadProfilesFromFile(const String &filename)
{
    bool ret = false;
    if (!LittleFS.exists(filename))
    {
        Serial.println("Profile file does not exist");
        return ret;
    }
    String buffer = "";
    if (loadFile(filename, &buffer) == true)
    {
        ret = jsonToAllProfiles(buffer);
    }
    else
    {
        Serial.println("Failed to open profile file for reading");
    }
    return ret;
}
void ProfileManager::saveDefaultProfiles()
{
    saveFile(profileFileName, initJson);
}
void ProfileManager::checkDefaultProfiles()
{
    if (!LittleFS.exists(profileFileName))
    {
        Serial.println("Profile file does not exist");
        saveDefaultProfiles();
    }
}
