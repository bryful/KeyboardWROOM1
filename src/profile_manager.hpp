#pragma once
#define DISABLE_FS_H_WARNING // この行を追加

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h> // ESP32 LittleFSライブラリ
#include "keypad_common.hpp"
#include "LGFX_ESP32S3WROOM1N8R2_SPI_ST7789.hpp"

// プロファイル管理クラス
class ProfileManager
{
public:
    static const int MAX_PROFILES = 20;

    // プロファイル定義
    static CompleteProfile profiles[];
    static const int profileCount;
    static int currentProfileIndex;

    // 初期化・管理関数
    static void initDefaultProfiles();
    static void saveProfiles();
    static void loadProfiles();

    static int indexOfProfile(const char *name);
    static void setActiveProfile(int index);
    static void setActiveProfile(const char *name);
    static void switchToNextProfile();
    static void switchToPrevProfile();
    static CompleteProfile &getCurrentProfile();

    // JSON処理関数
    static String profileToJson(const CompleteProfile &profile);
    static bool jsonToProfile(const String &jsonString, CompleteProfile profile);
    static String activeProfilesToJson();
    static String allProfilesToJson();
    static bool jsonToAllProfiles(const String &jsonString);

    // LittleFS読み書き関数
    static bool saveProfilesToFile(const String &filename);
    static bool loadProfilesFromFile(const String &filename);
    static bool saveFile(const String &filename, String data);
    static bool loadFile(const String &filename, String *data);

    static void saveDefaultProfiles();
    static void checkDefaultProfiles();
};
