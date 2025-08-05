/**
 * @file main.cpp
 * @brief ESP32-S3 キーパッドメインファームウェア
 *
 * 機能:
 * - 4x4キーマトリックススキャン
 * - 3つのロータリーエンコーダー制御
 * - USB HID キーボード/マウス機能
 * - プロファイル管理とディスプレイ表示
 *
 * ハードウェア: ESP32-S3 + ST7789 TFTディスプレイ
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <USB.h>
#include <USBHIDKeyboard.h>
#include <USBHIDMouse.h>
#include <Preferences.h>
#include <LittleFS.h>

#include "LGFX_ESP32S3WROOM1N8R2_SPI_ST7789.hpp"
#include "keypad_common.hpp"
#include "keypad_matrix.hpp"
#include "hid_sender.hpp"
#include "profile_manager.hpp"
#include "display_manager.hpp"
#include "FsUtils.hpp"

FsUtils fsu;
//  =============================================================================
//  ハードウェア定義
//  =============================================================================

LGFX_ESP32S3WROOM1N8R2_SPI_ST7789 tft;

// ロータリーエンコーダーピン定義
#define ROTARY1_CLK 38
#define ROTARY1_DT 37
#define ROTARY1_SW 36

#define ROTARY2_CLK 41
#define ROTARY2_DT 40
#define ROTARY2_SW 39

#define ROTARY3_CLK 1
#define ROTARY3_DT 2
#define ROTARY3_SW 42

// キーマトリックス設定
const int ROWS = 4;
const int COLS = 4;
const int rowPins[4] = {4, 5, 6, 7};
const int colPins[4] = {18, 17, 16, 15};

// =============================================================================
// システム定数
// =============================================================================

// ディスプレイ更新制御
#define DISPLAY_UPDATE_INTERVAL_MS 100

// ロータリーエンコーダー制御
#define ROTARY_DEBOUNCE_DELAY_MS 10

// プロファイル切替設定
const unsigned long PROFILE_SWITCH_HOLD_MS = 1200; // プロファイル切替の長押し判定時間(ms)

// システム制御
#define MAIN_LOOP_DELAY_MS 1
#define USB_INIT_DELAY_MS 500

// =============================================================================
// グローバル変数
// =============================================================================

// ディスプレイ更新制御
static unsigned long lastDisplayUpdate = 0;
static bool needsDisplayUpdate = false;

// ロータリーエンコーダー状態管理
struct RotaryState
{
  int lastCLK;
  int lastDT;
  int lastState; // 2bit: (CLK << 1) | DT
  int step;      // 1クリック分の進捗カウント
  bool lastSW;
  unsigned long lastSWPress;
};
RotaryState rotaryStates[ROTARY_COUNT];

// キー管理
bool keyPressed[KEY_MATRIX_SIZE] = {false};
uint8_t prevKeys[KEY_MATRIX_SIZE] = {0};

// USB HID
USBHIDKeyboard Keyboard;
USBHIDMouse Mouse;
// USBHID hid;
//  =============================================================================
//  関数プロトタイプ宣言
//  =============================================================================

// ロータリーエンコーダー関数
void initRotaryEncoders();
void handleRotaryEncoder(int encoderIndex, int clkPin, int dtPin, int swPin);
void scanRotaryEncoders();

// ディスプレイ制御関数
void requestDisplayUpdate();
void updateDisplayIfNeeded();
void forceDisplayUpdate();

// イベント処理関数
void handleKeyEvent(int keyCode, int prevKeyCode);
void handleProfileSwitching(int keyCode);
void handleKeyEvent(int keyIndex, bool pressed);

// =============================================================================
// ロータリーエンコーダー関数
// =============================================================================

/**
 * ロータリーエンコーダーのピンを初期化し、初期状態を読み取る
 */
void initRotaryEncoders()
{
  // ロータリーエンコーダー1
  pinMode(ROTARY1_CLK, INPUT_PULLUP);
  pinMode(ROTARY1_DT, INPUT_PULLUP);
  pinMode(ROTARY1_SW, INPUT_PULLUP);

  // ロータリーエンコーダー2
  pinMode(ROTARY2_CLK, INPUT_PULLUP);
  pinMode(ROTARY2_DT, INPUT_PULLUP);
  pinMode(ROTARY2_SW, INPUT_PULLUP);

  // ロータリーエンコーダー3
  pinMode(ROTARY3_CLK, INPUT_PULLUP);
  pinMode(ROTARY3_DT, INPUT_PULLUP);
  pinMode(ROTARY3_SW, INPUT_PULLUP);

  // 初期状態を読み取り
  rotaryStates[0].lastCLK = digitalRead(ROTARY1_CLK);
  rotaryStates[0].lastDT = digitalRead(ROTARY1_DT);
  rotaryStates[0].lastState = (rotaryStates[0].lastCLK << 1) | rotaryStates[0].lastDT;
  rotaryStates[0].step = 0;
  rotaryStates[0].lastSW = digitalRead(ROTARY1_SW);

  rotaryStates[1].lastCLK = digitalRead(ROTARY2_CLK);
  rotaryStates[1].lastDT = digitalRead(ROTARY2_DT);
  rotaryStates[1].lastState = (rotaryStates[1].lastCLK << 1) | rotaryStates[1].lastDT;
  rotaryStates[1].step = 0;
  rotaryStates[1].lastSW = digitalRead(ROTARY2_SW);

  rotaryStates[2].lastCLK = digitalRead(ROTARY3_CLK);
  rotaryStates[2].lastDT = digitalRead(ROTARY3_DT);
  rotaryStates[2].lastState = (rotaryStates[2].lastCLK << 1) | rotaryStates[2].lastDT;
  rotaryStates[2].step = 0;
  rotaryStates[2].lastSW = digitalRead(ROTARY3_SW);
}

/**
 * 単一のロータリーエンコーダーの状態を処理
 * @param encoderIndex エンコーダーのインデックス（0-2）
 * @param clkPin CLKピン番号
 * @param dtPin DTピン番号
 * @param swPin スイッチピン番号
 */
void handleRotaryEncoder(int encoderIndex, int clkPin, int dtPin, int swPin)
{
  int currentCLK = digitalRead(clkPin);
  int currentDT = digitalRead(dtPin);
  int currentSW = digitalRead(swPin);

  // 1クリック1出力: フルサイクル検出方式
  static const uint8_t cw_seq[4] = {0b00, 0b01, 0b11, 0b10};  // 時計回り
  static const uint8_t ccw_seq[4] = {0b00, 0b10, 0b11, 0b01}; // 反時計回り
  int currentState = (currentCLK << 1) | currentDT;
  int *step = &rotaryStates[encoderIndex].step;
  int *lastState = &rotaryStates[encoderIndex].lastState;
  if (currentState != *lastState)
  {
    // 時計回りシーケンス
    if (currentState == cw_seq[(*step + 1) & 0x3] && *lastState == cw_seq[*step & 0x3])
    {
      (*step)++;
      if (*step == 3 && currentState == cw_seq[3])
      {
        // 1クリック分進んだ
        CompleteProfile &currentProfile = ProfileManager::getCurrentProfile();
        RotaryConfig &config = currentProfile.rotaries[encoderIndex];
        KeyReport report = {0};
        report.modifiers = config.clockwiseModifier;
        report.keys[0] = config.clockwiseKey;
        Keyboard.sendReport(&report);
        delay(ROTARY_DEBOUNCE_DELAY_MS);
        KeyReport empty = {0};
        Keyboard.sendReport(&empty);
        *step = 0;
      }
    }
    // 反時計回りシーケンス
    else if (currentState == ccw_seq[(*step + 1) & 0x3] && *lastState == ccw_seq[*step & 0x3])
    {
      (*step)++;
      if (*step == 3 && currentState == ccw_seq[3])
      {
        // 1クリック分進んだ
        CompleteProfile &currentProfile = ProfileManager::getCurrentProfile();
        RotaryConfig &config = currentProfile.rotaries[encoderIndex];
        KeyReport report = {0};
        report.modifiers = config.counterClockwiseModifier;
        report.keys[0] = config.counterClockwiseKey;
        Keyboard.sendReport(&report);
        delay(ROTARY_DEBOUNCE_DELAY_MS);
        KeyReport empty = {0};
        Keyboard.sendReport(&empty);
        *step = 0;
      }
    }
    else
    {
      // シーケンスが崩れたらリセット
      *step = 0;
    }
    *lastState = currentState;
  }

  // スイッチ押下検出
  if (currentSW != rotaryStates[encoderIndex].lastSW)
  {
    if (currentSW == LOW)
    {
      // スイッチが押された
      rotaryStates[encoderIndex].lastSWPress = millis();
      DisplayManager::setRotaryPressed(encoderIndex, true);
      requestDisplayUpdate();

      // スイッチ機能実行
      CompleteProfile &currentProfile = ProfileManager::getCurrentProfile();
      RotaryConfig &config = currentProfile.rotaries[encoderIndex];

      if (config.function == COMBO_KEY)
      {
        KeyReport report = {0};
        report.modifiers = config.modifiers;
        report.keys[0] = config.primaryKey;
        Keyboard.sendReport(&report);
        delay(ROTARY_DEBOUNCE_DELAY_MS);
        KeyReport empty = {0};
        Keyboard.sendReport(&empty);
      }
      else if (config.function == MOUSE_LEFT_CLICK)
      {
        Mouse.click(MOUSE_LEFT);
      }
      else if (config.function == MOUSE_RIGHT_CLICK)
      {
        Mouse.click(MOUSE_RIGHT);
      }
    }
    else
    {
      // スイッチが離された
      DisplayManager::setRotaryPressed(encoderIndex, false);
      requestDisplayUpdate();
    }
  }

  // 状態を更新
  rotaryStates[encoderIndex].lastCLK = currentCLK;
  rotaryStates[encoderIndex].lastDT = currentDT;
  rotaryStates[encoderIndex].lastSW = currentSW;
}

/**
 * 全てのロータリーエンコーダーをスキャン
 */
void scanRotaryEncoders()
{
  handleRotaryEncoder(0, ROTARY1_CLK, ROTARY1_DT, ROTARY1_SW);
  handleRotaryEncoder(1, ROTARY2_CLK, ROTARY2_DT, ROTARY2_SW);
  handleRotaryEncoder(2, ROTARY3_CLK, ROTARY3_DT, ROTARY3_SW);
}

// =============================================================================
// ディスプレイ更新制御関数
// =============================================================================

/**
 * ディスプレイ更新フラグを設定
 */
void requestDisplayUpdate()
{
  needsDisplayUpdate = true;
}

/**
 * 必要に応じてディスプレイを更新（間隔制御あり）
 */
void updateDisplayIfNeeded()
{
  unsigned long now = millis();
  if (needsDisplayUpdate && (now - lastDisplayUpdate) >= DISPLAY_UPDATE_INTERVAL_MS)
  {
    // 現在のキー状態を計算
    int currentKeyCode = 0;
    for (int i = 0; i < KEY_MATRIX_SIZE; i++)
    {
      if (keyPressed[i])
      {
        currentKeyCode |= (1 << i);
      }
    }

    DisplayManager::updateDisplay(currentKeyCode, keyPressed, ProfileManager::profiles, ProfileManager::profileCount, ProfileManager::currentProfileIndex);
    lastDisplayUpdate = now;
    needsDisplayUpdate = false;
  }
}

// 強制的に即座にディスプレイを更新する関数
void forceDisplayUpdate()
{
  // 現在のキー状態を計算
  int currentKeyCode = 0;
  for (int i = 0; i < KEY_MATRIX_SIZE; i++)
  {
    if (keyPressed[i])
    {
      currentKeyCode |= (1 << i);
    }
  }

  DisplayManager::updateDisplay(currentKeyCode, keyPressed, ProfileManager::profiles, ProfileManager::profileCount, ProfileManager::currentProfileIndex);
  lastDisplayUpdate = millis();
  needsDisplayUpdate = false;
}
String GetSerialReadAll()
{
  String result = "";
  std::vector<byte> buffer;

  if (Serial.available() <= 0)
    return result;

  const int MAX_BYTES = 2048;
  int byteCount = 0;

  while (byteCount < MAX_BYTES)
  {
    if (Serial.available())
    {
      int byteRead = Serial.read();
      if (byteRead >= 0)
      {
        buffer.push_back(byteRead);
        byteCount++;
      }
    }
    else
    {
      break;
    }
  }
  if (byteCount > 0)
  {
    buffer.push_back(0); // null terminator
    result = String((char *)buffer.data());
  }

  return result;
}
bool JsonCMDCheck(JsonDocument request, JsonDocument *response)
{
  bool result = true;
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);

  if (!request["cmd"].isNull())
  {
    String cmd = request["cmd"].as<String>();
    cmd.toLowerCase(); // コマンドを小文字に変換
    if (cmd == "getstatus" || cmd == "status")
    {
      fsu.ESP_Status_json(response);
      result = true;
    }
    else if (cmd == "getboardname" | cmd == "boardname")
    {
      (*response)["BoardName"] = fsu.getBoardName("");
      result = true;
    }
    else if (cmd == "setBoardName")
    {
      String boardName = request["boardName"].as<String>();
      if (fsu.setBoardName(boardName))
      {
        (*response)["BoardName"] = fsu.getBoardName("");
        result = true;
      }
      else
      {
        Serial.println("setBoardNameエラー: ");
        result = false;
      }
    }
    else if (cmd == "restart")
    {
      ESP.restart();
      result = true;
    }
    else
    {
      result = false;
    }
  }

  else if (!request["getStatus"].isNull() || !request["status"].isNull())
  {
    fsu.ESP_Status_json(response);
    result = true;
  }

  if (!request["getBoardName"].isNull())
  {
    (*response)["BoardName"] = fsu.getBoardName("");
    result = true;
  }

  if (!request["setBoardName"].isNull())
  {
    String boardName = request["setBoardName"].as<String>();
    if (fsu.setBoardName(boardName) == false)
    {
      Serial.println("setBoardNameエラー: " + boardName);
      result = false;
    }
    else
    {
      result = true;
      Serial.println("setBoardName成功");
      (*response)["BoardName"] = fsu.getBoardName("");
    }
  }

  if (!request["setGateway"].isNull())
  {
    String gatewayIP = request["setGateway"].as<String>();
    bool success = false;

    if (gatewayIP != "")
    {
      if (fsu.setPrefString(GATWAY_IP_KEY, gatewayIP))
      {
        Serial.println("setGateway成功!");
        (*response)["serverip"] = gatewayIP;
        success = true;
      }
      else
      {
        Serial.println("setGatewayエラー");
      }
    }

    if (!success)
      result = false;
  }

  if (!request["getGateway"].isNull())
  {
    String gatewayIP = fsu.getPrefString(GATWAY_IP_KEY, "");
    if (gatewayIP != "")
    {
      (*response)["getGateway"] = gatewayIP;
      result = true;
    }
    else
    {
      Serial.println("getGatewayエラー");
      result = false;
    }
  }

  if (!request["wifi"].isNull())
  {
    JsonArray wifiParams = request["wifi"].as<JsonArray>();

    if (wifiParams.size() < 2)
    {
      Serial.println("WiFiパラメータエラー: SSIDとパスワードが必要です");
      result = false;
    }
    else
    {
      String ssid = wifiParams[0].as<String>();
      String password = wifiParams[1].as<String>();
      if (ssid.isEmpty() || password.isEmpty())
      {
        Serial.println("WiFiパラメータエラー: SSIDまたはパスワードが空です");
        result = false;
      }
      else
      {

        // WiFi接続を開始
        WiFi.begin(ssid.c_str(), password.c_str());
        delay(1000); // 接続開始後の待機
      }
      result = (WiFi.status() == WL_CONNECTED);
      if (result)
      {
        (*response)["wifi"] = WiFi.localIP().toString();
        result = true;
      }
    }
  }

  if (!request["restart"].isNull())
  {
    ESP.restart();
    result = true;
  }
  if (!request["index"].isNull())
  {
    int index = request["index"].as<int>();
    (*response)["index"] = index;
    ProfileManager::setActiveProfile(index);
    Serial.println("Active profile set to index: " + String(index));
    Serial.flush();
    forceDisplayUpdate();
    result = true;
  }
  if (!request["profileName"].isNull())
  {
    String profileName = request["profileName"].as<String>();
    (*response)["profileName"] = profileName;
    ProfileManager::setActiveProfile(profileName.c_str());
    Serial.println("Active profile set to name: " + profileName);
    Serial.flush();
    forceDisplayUpdate();
    result = true;
  }
  if (!request["getProfileData"].isNull())
  {
    (*response)["profileData"] = ProfileManager::activeProfilesToJson();
    result = true;
  }
  if (!request["getProfiles"].isNull())
  {
    (*response)["profiles"] = ProfileManager::allProfilesToJson();
    result = true;
  }
  return result;
}

int handleSerial()
{
  // シリアルデータが利用可能かチェック
  if (Serial.available() <= 0)
  {
    return 0; // データなし（正常）
  }

  int ret = 0;
  String tx = GetSerialReadAll();
  tx.trim();

  if (tx == "")
  {
    return 0; // 空データ（正常、データ待ち）
  }

  bool ok = false;
  JsonDocument jd = fsu.GetJsonData(tx, &ok);

  if (ok == true)
  {
    JsonDocument res;
    ok = JsonCMDCheck(jd, &res);
    if (ok)
    {
      String ret_str;
      serializeJson(res, ret_str);
      Serial.println(ret_str);
      Serial.flush();
      ret = 1; // 成功
    }
    else
    {
      Serial.println("Serial / ERROR: Invalid command1");
      Serial.println(tx);
      Serial.flush();
      ret = -1; // コマンドエラー
    }
  }
  else
  {
    Serial.println("Serial / ERROR: Invalid expression2");
    Serial.println(tx);
    ret = -2; // JSONパースエラー
  }
  return ret;
}

// =============================================================================
// セットアップ関数
// =============================================================================

/**
 * システム初期化
 * - ハードウェア初期化（シリアル、USB HID）
 * - プロファイル管理初期化
 * - キーマトリックス、ロータリーエンコーダー初期化
 * - ディスプレイ初期化と初期表示
 */
void setup()
{
  Serial.begin(115200);
  delay(500); // USB初期化待機

  /*
  USB.VID(0xCafe);
  USB.PID(0x3899);
  USB.productName("Fs Keyboard");
  USB.manufacturerName("bry-ful");
  USB.serialNumber("001");
  */
  USB.begin();
  if (!USB)
  {
    USB.begin();
    delay(1000);
  }
  Keyboard.begin();
  Mouse.begin();

  Serial.println("=== USB Serial Test Start ===");
  Serial.flush();
#if ARDUINO_USB_CDC_ON_BOOT
  Serial.println("USBSerial is available");
#else
  Serial.println("USBSerial is NOT available");
#endif

  // テスト出力
  Serial.flush();
  Serial.println("key init");

  if (!LittleFS.begin(true))
  {
    LittleFS.format();
    LittleFS.begin();
  }

  // プロファイル管理の初期化

  ProfileManager::checkDefaultProfiles();
  ProfileManager::loadProfiles();

  // ★ここに追加：Preferencesからアクティブプロファイルを復元
  Preferences preferences;
  preferences.begin("keypad", false);
  String savedProfileName = preferences.getString("activeProfile", "");
  preferences.end();

  if (savedProfileName.length() > 0)
  {
    ProfileManager::setActiveProfile(savedProfileName.c_str());
  }

  // ハードウェア初期化
  keypadMatrixInit();
  initRotaryEncoders();

  DisplayManager::init(&tft);

  // キー状態配列を初期化
  memset(keyPressed, false, sizeof(keyPressed));

  // 初回表示を強制実行
  forceDisplayUpdate();

  // hid.begin();

  delay(1000); // USB初期化待機

  // シリアル通信初期化
  Serial.println("Setup complete, entering main loop...");
}

// =============================================================================
// メインループ関数
// =============================================================================

/**
 * メインループ処理
 * - キーマトリックススキャン
 * - ロータリーエンコーダースキャン
 * - プロファイル切替処理
 * - キーイベント処理
 * - ディスプレイ更新
 */
void loop()
{

  static int prevKeyCode = 0;
  handleSerial();

  // ウォッチドッグタイマー対策
  yield();

  // キーマトリックススキャン
  int keyCode = scanMatrixDebounced();

  // ロータリーエンコーダースキャン
  scanRotaryEncoders();

  // プロファイル切替処理
  handleProfileSwitching(keyCode);

  // キー状態変化時のイベント処理
  if (keyCode != prevKeyCode)
  {
    handleKeyEvent(keyCode, prevKeyCode);
  }

  // ディスプレイ更新制御
  updateDisplayIfNeeded();

  prevKeyCode = keyCode;

  // CPUリソース解放
  delay(MAIN_LOOP_DELAY_MS);
  yield();
}

// =============================================================================
// イベント処理関数
// =============================================================================

/**
 * プロファイル切替キーの処理
 * PREF_CHANGE_KEY: 次のプロファイルに切替
 * PREF_PREV_KEY: 前のプロファイルに切替
 * @param keyCode 現在のキー状態
 */
void handleProfileSwitching(int keyCode)
{
  static unsigned long prefChangePressTime[KEY_MATRIX_SIZE] = {0};
  static bool prefChangePrev[KEY_MATRIX_SIZE] = {false};
  unsigned long now = millis();

  CompleteProfile &currentProfile = ProfileManager::getCurrentProfile();

  for (int i = 0; i < KEY_MATRIX_SIZE; i++)
  {
    // 次のプロファイルに切替
    if (currentProfile.keys[i].function == PREF_CHANGE_KEY)
    {
      bool pressed = (keyCode & (1 << i)) != 0;
      if (pressed && !prefChangePrev[i])
      {
        prefChangePressTime[i] = now;
      }
      if (!pressed && prefChangePrev[i])
      {
        unsigned long pressDuration = now - prefChangePressTime[i];
        if (pressDuration > 0 && pressDuration <= PROFILE_SWITCH_HOLD_MS)
        {
          ProfileManager::switchToNextProfile();
          requestDisplayUpdate();
        }
        prefChangePressTime[i] = 0;
      }
      prefChangePrev[i] = pressed;
    }
    // 前のプロファイルに切替
    else if (currentProfile.keys[i].function == PREF_PREV_KEY)
    {
      bool pressed = (keyCode & (1 << i)) != 0;
      if (pressed && !prefChangePrev[i])
      {
        prefChangePressTime[i] = now;
      }
      if (!pressed && prefChangePrev[i])
      {
        unsigned long pressDuration = now - prefChangePressTime[i];
        if (pressDuration > 0 && pressDuration <= PROFILE_SWITCH_HOLD_MS)
        {
          ProfileManager::switchToPrevProfile();
          requestDisplayUpdate();
        }
        prefChangePressTime[i] = 0;
      }
      prefChangePrev[i] = pressed;
    }
  }
}

/**
 * キーイベント処理とHIDレポート送信
 * キーの押下/開放状態を追跡し、HIDレポートを送信
 * @param keyCode 現在のキー状態
 * @param prevKeyCode 前回のキー状態
 */
void handleKeyEvent(int keyCode, int prevKeyCode)
{
  bool updated = false;

  // キー状態の更新チェック
  for (int i = 0; i < KEY_MATRIX_SIZE; i++)
  {
    int mask = (1 << i);
    bool prev = keyPressed[i];
    keyPressed[i] = (keyCode & mask) != 0;
    if (keyPressed[i] != prev)
      updated = true;
  }

  // HIDレポート送信
  CompleteProfile &currentProfile = ProfileManager::getCurrentProfile();
  hidSendCurrentKeyReport(keyCode, currentProfile.keys, keyPressed, prevKeys);

  // ディスプレイ更新要求
  if (updated || keyCode != prevKeyCode)
  {
    requestDisplayUpdate();
  }
}

/**
 * キーイベント処理
 * @param keyIndex キーのインデックス（0-15）
 * @param pressed 押下状態（true: 押された、false: 離された）
 */
void handleKeyEvent(int keyIndex, bool pressed)
{
  if (keyIndex < 0 || keyIndex >= 16)
    return;

  KeyConfig &key = ProfileManager::profiles[ProfileManager::currentProfileIndex].keys[keyIndex];

  // デバッグログ追加
  Serial.printf("Key %d  %s - Function: %d\n",
                keyIndex,
                pressed ? "PRESSED" : "RELEASED",
                key.function);

  keyPressed[keyIndex] = pressed;

  // HOLD_KEYの場合も必ずディスプレイ更新を要求
  if (key.function == HOLD_KEY)
  {
    Serial.println("HOLD_KEY detected - requesting display update");
    requestDisplayUpdate();
  }

  // 他のキータイプの処理...
  if (pressed && key.function != NO_KEY)
  {
    requestDisplayUpdate(); // 全キータイプでディスプレイ更新
  }
}
