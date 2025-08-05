#include "FsUtils.hpp"

// -----------------------------------------------------------------------------
FsUtils::FsUtils()
{
    // コンストラクタ - 特に初期化処理は不要
}

FsUtils::~FsUtils()
{
    // デストラクタ - 特にクリーンアップ処理は不要
}

// -----------------------------------------------------------------------------
int FsUtils::split(String data, char delimiter, String *dst)
{
    int index = 0;
    int datalength = data.length();

    for (int i = 0; i < datalength; i++)
    {
        char tmp = data.charAt(i);
        if (tmp == delimiter)
        {
            index++;
        }
        else
            dst[index] += tmp;
    }

    return (index + 1);
}
bool FsUtils::ipStrToBytes(String str, uint8_t *bytes, int length)
{
    bool ret = false;
    if ((str != "") && (length >= 4))
    {
        String ipParts[4];
        int partCount = split(str, '.', ipParts);
        if (partCount >= 4)
        {
            for (int i = 0; i < 4; i++)
            {
                bytes[i] = (uint8_t)(atoi(ipParts[i].c_str()) & 0xFF);
            }
            ret = true;
        }
    }
    return ret;
}

// -----------------------------------------------------------------------------
bool FsUtils::prefClear()
{
    bool ret = false;
    Preferences pref;
    if (pref.begin(PREF_NAMESPACE, false) == false)
        return ret;
    ret = pref.clear();
    pref.end();
    return ret;
}
bool FsUtils::setPrefULong(String key, u64_t value)
{
    bool result = false;
    Preferences pref;

    if (pref.begin(PREF_NAMESPACE, false) == false)
        return result;

    result = pref.putULong64(key.c_str(), value);
    pref.end();
    return result;
}
u64_t FsUtils::getPrefULong(String key, u64_t defaultValue)
{
    u64_t result = defaultValue;
    Preferences pref;

    if (pref.begin(PREF_NAMESPACE, false) == false)
        return result;

    result = pref.getULong64(key.c_str(), defaultValue);
    pref.end();
    return result;
}
// -----------------------------------------------------------------------------
String FsUtils::getPrefString(String key, String def)
{
    String ret = def;
    Preferences pref;
    if (pref.begin(PREF_NAMESPACE, false) == false)
        return ret;
    ret = pref.getString(key.c_str(), def);

    pref.end();
    return ret;
}
// -----------------------------------------------------------------------------
bool FsUtils::setPrefString(String key, String str)
{
    bool ret = false;
    Preferences pref;

    if (pref.begin(PREF_NAMESPACE, false) == false)
        return ret;

    ret = pref.putString(key.c_str(), str);
    delay(100);
    Serial.printf("putString結果: %d\n", ret);
    pref.end();
    return ret;
}
// -----------------------------------------------------------------------------
bool FsUtils::getPrefIPA(String key, uint8_t *adr)
{
    Serial.println("getPrefIPA");
    bool ret = false;
    String s = getPrefString(key, "");
    Serial.println("IP文字列: " + s);

    if (s != "")
    {
        String ipParts[4];
        int partCount = split(s, '.', ipParts);
        if (partCount >= 4)
        {
            for (int i = 0; i < 4; i++)
            {
                adr[i] = atoi(ipParts[i].c_str()) & 0xFF;
            }
            ret = true;
        }
    }
    return ret;
}
// -----------------------------------------------------------------------------
String FsUtils::getBoardName(String def)
{
    String ret = def;
    ret = getPrefString(BOARD_KEY, def);

    return ret;
}
// -----------------------------------------------------------------------------
bool FsUtils::setBoardName(String bn)
{
    bool ret = false;

    ret = setPrefString(BOARD_KEY, bn);
    return ret;
}

void FsUtils::loadWiFiPref()
{
    //
    // saveBoardNameToPref("");
    // saveGatewayIPToPref("");
    // saveMyIPToPref("");
    // saveTargetIPToPref("");

    m_boardName = getBoardName("");
    if (m_boardName == "")
    {
        m_boardName = getDefaultMacAddress();
    }
    Serial.println("loadWiFiPref:boardName: " + m_boardName);

    m_myIP = getPrefString(MyIP_KEY, "");
    if (m_myIP == "")
    {
        Serial.println("loadWiFiPref:MyIP ERROR!");
    }
    else
    {
        Serial.println("loadWiFiPref:MyIP OK!: " + m_myIP);
    }

    m_gatewayIP = getPrefString(GATWAY_IP_KEY, "");
    if (m_gatewayIP == "")
    {
        Serial.println("loadWiFiPref:gatewayIP ERROR!");
    }
    else
    {
        Serial.println("loadWiFiPref:gatewayIP OK!: " + m_gatewayIP);
    }
    m_targetIP = getPrefString(TARGET_IP_KEY, "");
    if (m_targetIP == "")
    {
        Serial.println("loadWiFiPref:targetIP ERROR!");
    }
    else
    {
        Serial.println("loadWiFiPref:targetIP OK!: " + m_targetIP);
    }
}
bool FsUtils::saveBoardNameToPref(String boardName)
{
    return setPrefString(BOARD_KEY, boardName);
}
bool FsUtils::saveMyIPToPref(String myIP)
{
    return setPrefString(MyIP_KEY, myIP);
}
bool FsUtils::saveGatewayIPToPref(String myGatewayIP)
{
    return setPrefString(GATWAY_IP_KEY, myGatewayIP);
}
bool FsUtils::saveTargetIPToPref(String myTargetIP)
{
    return setPrefString(TARGET_IP_KEY, myTargetIP);
}

bool FsUtils::setupNTP()
{
    bool result = false;
    Serial.println("[setupNTP] NTP時刻同期を開始します");

    // WiFi接続確認
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("[setupNTP] エラー: WiFiに接続されていません");
        return result;
    }

    Serial.println("[setupNTP] NTPサーバーに接続中...");

    // NTP設定（日本標準時 JST-9）
    configTzTime("JST-9", "ntp.nict.jp", "ntp.jst.mfeed.ad.jp", "time.google.com");

    // 設定後の待機時間を追加
    delay(1000);

    // 時刻同期の確認
    time_t now;
    struct tm timeinfo;
    const int MAX_RETRY = 30;    // 最大試行回数を30回に増加
    const int RETRY_DELAY = 500; // リトライ間隔（ミリ秒）
    int retryCount = 0;

    Serial.print("[setupNTP] 時刻同期中");

    while (retryCount < MAX_RETRY)
    {
        time(&now);
        if (getLocalTime(&timeinfo, 1000))
        {
            // 時刻が正常に取得できた場合（1970年以降かチェック）
            if (now > 946684800) // 2000年1月1日のタイムスタンプ
            {
                result = true;
                break;
            }
        }

        Serial.print(".");
        delay(RETRY_DELAY);
        retryCount++;
    }

    Serial.println(); // 改行

    if (result)
    {
        // 同期成功時の詳細情報表示
        char timeStr[64];
        strftime(timeStr, sizeof(timeStr), "%Y/%m/%d %H:%M:%S", &timeinfo);

        Serial.println("[setupNTP] NTP時刻同期成功!");
        Serial.printf("[setupNTP] 現在時刻: %s JST\n", timeStr);
        Serial.printf("[setupNTP] Unix時刻: %ld\n", now);
        Serial.printf("[setupNTP] 試行回数: %d回\n", retryCount + 1);
    }
    else
    {
        Serial.printf("[setupNTP] NTP時刻同期失敗 (試行回数: %d回)\n", retryCount);
        Serial.println("[setupNTP] 以下を確認してください:");
        Serial.println("  - WiFi接続の安定性");
        Serial.println("  - DNSサーバーの設定");
        Serial.println("  - ファイアウォールの設定");
        Serial.println("  - NTPポート(123)の開放");

        // 現在の時刻を表示（同期されていない可能性あり）
        time(&now);
        if (getLocalTime(&timeinfo, 100))
        {
            char timeStr[64];
            strftime(timeStr, sizeof(timeStr), "%Y/%m/%d %H:%M:%S", &timeinfo);
            Serial.printf("[setupNTP] 現在の内部時刻: %s (未同期の可能性)\n", timeStr);
        }
    }

    return result;
}

bool FsUtils::setupWiFi()
{
    bool ret = false;
    Serial.println("[setupWiFi] WiFi接続を開始します...");

    // 既存の接続がある場合は切断
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("[setupWiFi] 既存の接続を切断します");
        WiFi.disconnect();
        delay(100);
    }

    // 固定IP設定
    bool prefErr = false;
    uint8_t lc[4] = {0, 0, 0, 0};
    uint8_t gw[4] = {0, 0, 0, 0};

    // ゲートウェイIPの取得と設定
    if (m_gatewayIP == "")
    {
        m_gatewayIP = getPrefString(GATWAY_IP_KEY, "");
    }
    if (m_gatewayIP != "")
    {
        if (ipStrToBytes(m_gatewayIP, (uint8_t *)gw, 4))
        {
            Serial.printf("[setupWiFi] ゲートウェイIP: %d.%d.%d.%d\n", gw[0], gw[1], gw[2], gw[3]);
        }
        else
        {
            Serial.println("[setupWiFi] エラー: ゲートウェイIPの形式が不正です");
            prefErr = true;
        }
    }
    else
    {
        Serial.println("[setupWiFi] 警告: ゲートウェイIPが未設定（DHCPを使用）");
        prefErr = true;
    }

    // ローカルIPの取得と設定
    if (m_myIP == "")
    {
        m_myIP = getPrefString(MyIP_KEY, "");
    }
    if (m_myIP != "")
    {
        if (ipStrToBytes(m_myIP, (uint8_t *)lc, 4))
        {
            Serial.printf("[setupWiFi] ローカルIP: %d.%d.%d.%d\n", lc[0], lc[1], lc[2], lc[3]);
        }
        else
        {
            Serial.println("[setupWiFi] エラー: ローカルIPの形式が不正です");
            prefErr = true;
        }
    }
    else
    {
        Serial.println("[setupWiFi] 警告: ローカルIPが未設定（DHCPを使用）");
        prefErr = true;
    }

    // 固定IP設定の適用
    if (!prefErr)
    {
        IPAddress local_IP(lc[0], lc[1], lc[2], lc[3]);
        IPAddress gateway(gw[0], gw[1], gw[2], gw[3]);
        IPAddress subnet(255, 255, 255, 0);
        IPAddress primaryDNS(8, 8, 8, 8);
        IPAddress secondaryDNS(8, 8, 4, 4);

        if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
        {
            Serial.println("[setupWiFi] 固定IP設定に失敗しました");
        }
        else
        {
            Serial.println("[setupWiFi] 固定IP設定が完了しました");
        }
    }
    else
    {
        Serial.println("[setupWiFi] DHCPを使用して接続します");
    }

    // ホスト名設定
    if (!m_boardName.isEmpty())
    {
        WiFi.setHostname(m_boardName.c_str());
        Serial.println("[setupWiFi] ホスト名: " + m_boardName);
    }

    // WiFi接続開始（保存された認証情報を使用）
#ifdef SSID_PASS_USE
    if (!m_SSID.isEmpty() && !m_password.isEmpty())
    {
        WiFi.begin(m_SSID.c_str(), m_password.c_str());
        Serial.println("[setupWiFi] 指定されたSSIDで接続開始");
    }
    else
    {
        WiFi.begin();
        Serial.println("[setupWiFi] 保存された認証情報で接続開始");
    }
#else
    WiFi.begin(); // ESP32内蔵の保存された認証情報を使用
    Serial.println("[setupWiFi] 保存された認証情報で接続開始");
#endif

    WiFi.setTxPower(WIFI_POWER_8_5dBm);

    // 接続待機ループ
    const int MAX_RETRY = 50;
    const int RECONNECT_INTERVAL = 10;
    int retry = 0;

    Serial.print("[setupWiFi] 接続中");
    while (WiFi.status() != WL_CONNECTED && retry < MAX_RETRY)
    {
        Serial.print(".");
        delay(500);

        // 定期的に再接続を試行
        if (retry > 0 && (retry % RECONNECT_INTERVAL) == 0)
        {
            Serial.print("\n[setupWiFi] 再接続を試行します...");
            WiFi.disconnect();
            delay(100);
            WiFi.reconnect();
        }

        retry++;
    }

    // 接続結果の確認
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\n[setupWiFi] WiFi接続成功!");
        Serial.printf("[setupWiFi] 割り当てられたIP: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("[setupWiFi] ゲートウェイ: %s\n", WiFi.gatewayIP().toString().c_str());
        Serial.printf("[setupWiFi] サブネット: %s\n", WiFi.subnetMask().toString().c_str());
        Serial.printf("[setupWiFi] SSID: %s\n", WiFi.SSID().c_str());
        Serial.printf("[setupWiFi] RSSI: %d dBm\n", WiFi.RSSI());
        ret = true;
    }
    else
    {
        Serial.printf("\n[setupWiFi] WiFi接続失敗 (試行回数: %d)\n", retry);
        Serial.printf("[setupWiFi] WiFiステータス: %d\n", WiFi.status());

        // エラーの詳細表示
        switch (WiFi.status())
        {
        case WL_NO_SSID_AVAIL:
            Serial.println("[setupWiFi] エラー: 保存されたSSIDが見つかりません");
            break;
        case WL_CONNECT_FAILED:
            Serial.println("[setupWiFi] エラー: 認証に失敗しました");
            break;
        case WL_CONNECTION_LOST:
            Serial.println("[setupWiFi] エラー: 接続が失われました");
            break;
        case WL_DISCONNECTED:
            Serial.println("[setupWiFi] エラー: 切断されています");
            break;
        default:
            Serial.println("[setupWiFi] エラー: 不明なエラーが発生しました");
            break;
        }
        ret = false;
    }

    return ret;
}
int FsUtils::setupWiFiAndNTP()
{
    int result = 0;
    if (setupWiFi())
    {
        Serial.println("[setupWiFiAndNTP] WiFi接続成功");
        result = 1; // WiFi接続成功
        if (setupNTP())
        {
            result += 2; // WiFi接続とNTP同期成功
        }
    }

    return result;
}

// -----------------------------------------------------------------------------
String FsUtils::getDefaultMacAddress()
{
    String mac = "";
    unsigned char macBase[6] = {0};

    if (esp_efuse_mac_get_default(macBase) == ESP_OK)
    {
        char buffer[18]; // MACアドレス用のバッファサイズを適切に設定
        sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X",
                macBase[0], macBase[1], macBase[2],
                macBase[3], macBase[4], macBase[5]);
        mac = buffer;
    }

    return mac;
}
// -----------------------------------------------------------------------------
void FsUtils::ESP_Status_json(JsonDocument *doc)
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    (*doc)["BoardName"] = m_boardName;
    (*doc)["IP"] = WiFi.localIP().toString();
    (*doc)["MyIP"] = m_myIP;
    (*doc)["MAC"] = getDefaultMacAddress();
    (*doc)["Gateway"] = m_gatewayIP;
    (*doc)["TargetIP"] = m_targetIP;
    (*doc)["cores"] = chip_info.cores;
    (*doc)["revision"] = chip_info.revision;
    (*doc)["flashsize"] = spi_flash_get_chip_size();
    (*doc)["ESP-IDF"] = esp_get_idf_version();
    (*doc)["ChipModel"] = ESP.getChipModel();
    (*doc)["ChipRevision"] = ESP.getChipRevision();
    (*doc)["FreeHeap"] = ESP.getFreeHeap();
    (*doc)["HeapSize"] = ESP.getHeapSize();
    (*doc)["PsramSize"] = ESP.getPsramSize();
    (*doc)["FreePsram"] = ESP.getFreePsram();
}
String FsUtils::GetSerialReadAll()
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
JsonDocument FsUtils::GetJsonData(String text, bool *isValid)
{
    JsonDocument result;
    *isValid = true;

    if (text.length() > 0)
    {
        int startIdx = text.indexOf("{");
        int endIdx = text.lastIndexOf("}");

        if ((startIdx >= 0) && (startIdx < endIdx))
        {
            String jsonText = text.substring(startIdx, endIdx + 1);
            DeserializationError error = deserializeJson(result, jsonText);
            *isValid = (error == DeserializationError::Ok);
        }
        else
        {
            *isValid = false;
        }
    }

    return result;
}
int FsUtils::JsonCMDCheck(JsonDocument request, JsonDocument *response)
{
    int result = 0;
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    if (!request["cmd"].isNull())
    {
        String cmd = request["cmd"].as<String>();
        cmd.toLowerCase(); // コマンドを小文字に変換
        if (cmd == "getstatus" || cmd == "status")
        {
            ESP_Status_json(response);
        }
        else if (cmd == "getboardname" | cmd == "boardname")
        {
            (*response)["BoardName"] = m_boardName;
        }
        else if (cmd == "setBoardName")
        {
            String boardName = request["boardName"].as<String>();
            if (setBoardName(boardName))
            {
                (*response)["BoardName"] = getBoardName("");
            }
            else
            {
                Serial.println("setBoardNameエラー: " + boardName);
                result = -1;
            }
        }
        else if (cmd == "restart")
        {
            ESP.restart();
        }
        else
        {
            result = -1;
        }
    }

    else if (!request["getStatus"].isNull() || !request["status"].isNull())
    {
        ESP_Status_json(response);
    }

    if (!request["getBoardName"].isNull())
    {
        (*response)["BoardName"] = getBoardName("");
    }

    if (!request["setBoardName"].isNull())
    {
        String boardName = request["setBoardName"].as<String>();
        if (setBoardName(boardName) == false)
        {
            Serial.println("setBoardNameエラー: " + boardName);
            result = -1;
        }
        else
        {
            Serial.println("setBoardName成功");
            (*response)["BoardName"] = getBoardName("");
        }
    }

    if (!request["setGateway"].isNull())
    {
        String gatewayIP = request["setGateway"].as<String>();
        bool success = false;

        if (gatewayIP != "")
        {
            if (setPrefString(GATWAY_IP_KEY, gatewayIP))
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
            result = -1;
    }

    if (!request["getGateway"].isNull())
    {
        String gatewayIP = getPrefString(GATWAY_IP_KEY, "");
        if (gatewayIP != "")
        {
            (*response)["getGateway"] = gatewayIP;
        }
        else
        {
            Serial.println("getGatewayエラー");
            result = -1;
        }
    }

    if (!request["wifi"].isNull())
    {
        JsonArray wifiParams = request["wifi"].as<JsonArray>();

        if (wifiParams.size() < 2)
        {
            Serial.println("WiFiパラメータエラー: SSIDとパスワードが必要です");
            result = -1;
        }
        else
        {
            String ssid = wifiParams[0].as<String>();
            String password = wifiParams[1].as<String>();
            if (ssid.isEmpty() || password.isEmpty())
            {
                Serial.println("WiFiパラメータエラー: SSIDまたはパスワードが空です");
                result = -1;
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
            }
        }
    }

    if (!request["restart"].isNull())
    {
        ESP.restart();
    }
    if (!request["index"].isNull())
    {
        (*response)["index"] = request["index"];
    }
    return result;
}

int FsUtils::handleSerial()
{
    int ret = 0;
    String tx = GetSerialReadAll();
    tx.trim();
    if (tx == "")
    {
        ret = -1; // エラー
        return ret;
    }
    bool ok = false;
    JsonDocument jd = GetJsonData(tx, &ok);

    if (ok == true)
    {
        JsonDocument res;
        ok = JsonCMDCheck(jd, &res);
        if (ok)
        {
            String ret;
            serializeJson(res, ret);
            Serial.println(ret); // 修正：print → println
            ret = 1;             // 成功
        }
        else
        {
            Serial.println("Serial / ERROR: Invalid command");
        }
    }
    else
    {
        double value = 0;
        if (expr(tx.c_str(), &value) == true)
        {
            Serial.println("Result: " + String(value));
            ret = 1; // 成功
        }
        else
        {
            Serial.println("Serial / ERROR: Invalid expression");
        }
    }
    return ret; // 成功時は1、エラー時は-1を返す 0はJsonのパースエラー
}
int FsUtils::handleTcpClient()
{
    int ret = 0;
    WiFiClient client = server.available();
    bool ok = false;

    if (client && client.connected())
    {
        Serial.println("New client connected");
        String json = client.readStringUntil('\n');
        JsonDocument doc = GetJsonData(json, &ok);

        if (ok)
        {
            JsonDocument reply;
            ok = JsonCMDCheck(doc, &reply);
            if (ok)
            {
                reply["ack"] = true;
                String responseStr;
                serializeJson(reply, responseStr);
                client.println(responseStr);
                Serial.println(responseStr);
                ret = 1; // 成功
            }
            else
            {
                client.println("tcp / ERROR: Invalid command or parameters");
                Serial.println("tcp / ERROR: Invalid command or parameters");
            }
        }
        else
        {

            client.println("tcp / ERROR: Invalid JSON format - " + json);
        }
        client.stop();
    }
    return ret; // 成功時は1、エラー時は-1を返す 0はJsonのパースエラー
}
bool FsUtils::expr(const char *s, double *v)
{
    bool ret = false;
    /*
    int error;
    double result = te_interp(s, &error);
    if (error == 0)
    {
        *v = result;
        ret = true;
    }
    else
    {
        Serial.printf("Expression error: %d\n", error);
    }
        */
    return ret;
}
