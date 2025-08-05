#pragma once
#ifndef FSUTILS_H
#define FSUTILS_H

// =============================================================================
// システムライブラリ
// =============================================================================
#include <Arduino.h>
#include <String>
#include <sstream>
#include <vector>
// =============================================================================
//#include "tinyexpr.h"
// =============================================================================
// ESP32専用ライブラリ
// =============================================================================
#include <Preferences.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "esp_mac.h" // ESP32 MACアドレス関連
#include "esp_chip_info.h"
#include "esp_system.h"
#include <ArduinoJson.h>

// =============================================================================
// 定数定義
// =============================================================================
// サーバー設定
#define SERVER_PORT 12345

// Preferences キー定義
#define PREF_NAMESPACE "settings"
#define MyIP_KEY "myip"
#define GATWAY_IP_KEY "gatewayadr"
#define BOARD_KEY "boardname"
#define TARGET_IP_KEY "targetip"

// 機能切り替え
// #define SSID_PASS_USE  // SSID/パスワードの直接指定を使用する場合はコメントアウト

// 名前空間使用宣言
using namespace std;

// =============================================================================
// FsUtilsクラス定義
// =============================================================================
/**
 * @brief ファイルシステムとネットワーク機能を提供するユーティリティクラス
 *
 * このクラスはESP32向けのWiFi接続、NTP時刻同期、設定保存などの
 * 基本的な機能を提供します。
 */
class FsUtils
{
private:
    // =============================================================================
    // プライベートメンバ変数
    // =============================================================================
    String m_myIP = "";      ///< ローカルIPアドレス
    String m_gatewayIP = ""; ///< ゲートウェイIPアドレス
    String m_boardName = ""; ///< ボード名（ホスト名）
    String m_targetIP = "";  ///< ターゲットIPアドレス

#ifdef SSID_PASS_USE
    String m_SSID = "";     ///< WiFi SSID（オプション）
    String m_password = ""; ///< WiFi パスワード（オプション）
#endif

public:
    // =============================================================================
    // パブリックメンバ変数
    // =============================================================================
    /**
     * @brief WiFiサーバーインスタンス
     * @details 指定されたポートでTCP接続を待機するサーバー
     */
    WiFiServer server = WiFiServer(SERVER_PORT);

    // =============================================================================
    // コンストラクタ・デストラクタ
    // =============================================================================
    /**
     * @brief コンストラクタ
     */
    FsUtils();

    /**
     * @brief デストラクタ
     */
    ~FsUtils();

    // =============================================================================
    // ゲッター関数（const関数）
    // =============================================================================
    /**
     * @brief ローカルIPアドレスを取得
     * @return String ローカルIPアドレス
     */
    String MyIP() const { return m_myIP; }

    /**
     * @brief ゲートウェイIPアドレスを取得
     * @return String ゲートウェイIPアドレス
     */
    String GatewayIP() const { return m_gatewayIP; }

    /**
     * @brief ボード名を取得
     * @return String ボード名
     */
    String BoardName() const { return m_boardName; }

    /**
     * @brief ターゲットIPアドレスを取得
     * @return String ターゲットIPアドレス
     */
    String TargetIP() const { return m_targetIP; }

    // =============================================================================
    // 文字列・データ操作関数
    // =============================================================================
    /**
     * @brief 文字列を指定した区切り文字で分割
     * @param data 分割対象の文字列
     * @param delimiter 区切り文字
     * @param dst 分割結果を格納する配列
     * @return int 分割された要素数
     */
    int split(String data, char delimiter, String *dst);

    /**
     * @brief IP文字列をバイト配列に変換
     * @param str IPアドレス文字列（例: "192.168.1.1"）
     * @param bytes 変換結果を格納するバイト配列
     * @param length バイト配列の長さ
     * @return bool 変換成功時はtrue
     */
    bool ipStrToBytes(String str, uint8_t *bytes, int length);

    // =============================================================================
    // Preferences（設定保存）関数
    // =============================================================================
    /**
     * @brief 全ての設定をクリア
     * @return bool 成功時はtrue
     */
    bool prefClear();

    /**
     * @brief 文字列設定を取得
     * @param key 設定キー
     * @param def デフォルト値
     * @return String 設定値
     */
    String getPrefString(String key, String def);

    /**
     * @brief 文字列設定を保存
     * @param key 設定キー
     * @param str 保存する文字列
     * @return bool 成功時はtrue
     */
    bool setPrefString(String key, String str);

    /**
     * @brief 64bit整数設定を保存
     * @param key 設定キー
     * @param value 保存する値
     * @return bool 成功時はtrue
     */
    bool setPrefULong(String key, u64_t value);

    /**
     * @brief 64bit整数設定を取得
     * @param key 設定キー
     * @param defaultValue デフォルト値
     * @return u64_t 設定値
     */
    u64_t getPrefULong(String key, u64_t defaultValue);

    /**
     * @brief IPアドレス設定をバイト配列で取得
     * @param key 設定キー
     * @param adr IPアドレスを格納するバイト配列（4バイト）
     * @return bool 成功時はtrue
     */
    bool getPrefIPA(String key, uint8_t *adr);

    // =============================================================================
    // ボード名関連関数
    // =============================================================================
    /**
     * @brief ボード名を取得
     * @param def デフォルト値
     * @return String ボード名
     */
    String getBoardName(String def);

    /**
     * @brief ボード名を設定
     * @param bn ボード名
     * @return bool 成功時はtrue
     */
    bool setBoardName(String bn);

    // =============================================================================
    // WiFi設定関連関数
    // =============================================================================
    /**
     * @brief WiFi設定をPreferencesから読み込み
     */
    void loadWiFiPref();

    /**
     * @brief ボード名をPreferencesに保存
     * @param boardName ボード名
     * @return bool 成功時はtrue
     */
    bool saveBoardNameToPref(String boardName);

    /**
     * @brief ローカルIPをPreferencesに保存
     * @param myIP ローカルIP
     * @return bool 成功時はtrue
     */
    bool saveMyIPToPref(String myIP);

    /**
     * @brief ゲートウェイIPをPreferencesに保存
     * @param myGatewayIP ゲートウェイIP
     * @return bool 成功時はtrue
     */
    bool saveGatewayIPToPref(String myGatewayIP);

    /**
     * @brief ターゲットIPをPreferencesに保存
     * @param myTargetIP ターゲットIP
     * @return bool 成功時はtrue
     */
    bool saveTargetIPToPref(String myTargetIP);

    // =============================================================================
    // ネットワーク機能関数
    // =============================================================================
    /**
     * @brief WiFi接続を設定・実行
     * @return bool 接続成功時はtrue
     */
    bool setupWiFi();

    /**
     * @brief NTP時刻同期を実行
     * @return bool 同期成功時はtrue
     */
    bool setupNTP();

    /**
     * @brief WiFi接続とNTP同期を順次実行
     * @return int 実行結果コード
     */
    int setupWiFiAndNTP();

    // =============================================================================
    // システム情報関数
    // =============================================================================
    /**
     * @brief デフォルトMACアドレスを取得
     * @return String MACアドレス文字列
     */
    String getDefaultMacAddress();

    /**
     * @brief ESP32ステータス情報をJsonDocumentに設定
     * @param doc ステータス情報を格納するJsonDocumentのポインタ
     */
    void ESP_Status_json(JsonDocument *doc);

    // =============================================================================
    // シリアル通信・JSON処理関数
    // =============================================================================
    /**
     * @brief シリアルポートから全データを読み取り
     * @return String 読み取った文字列
     */
    String GetSerialReadAll();

    /**
     * @brief JSON文字列をパースしてJsonDocumentに変換
     * @param text JSON文字列
     * @param isValid パース成功フラグのポインタ
     * @return JsonDocument パース結果
     */
    JsonDocument GetJsonData(String text, bool *isValid);

    /**
     * @brief JSONコマンドをチェックして応答を生成
     * @param request リクエストのJsonDocument
     * @param response 応答のJsonDocumentポインタ
     * @return bool 処理成功時はtrue
     */
    int JsonCMDCheck(JsonDocument request, JsonDocument *response);

    // =============================================================================
    // 通信処理関数
    // =============================================================================
    /**
     * @brief シリアル通信を処理
     * @return int 処理結果コード
     */
    int handleSerial();

    /**
     * @brief TCPクライアントとの通信を処理
     * @return int 処理結果コード
     */
    int handleTcpClient();

    bool expr(const char *s, double *v);
};

#endif // FSUTILS_H
