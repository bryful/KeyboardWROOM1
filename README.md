# ESP32-S3 WROOM1 キーパッドプロジェクト

ESP32-S3を使用したプログラマブル・キーパッドファームウェア。
16個のボタンと3個のロータリーエンコーダーを備えた多機能入力デバイスです。

## 主な機能

- **4x4キーマトリックス**: 16個のプログラマブルキー
- **3個のロータリーエンコーダー**: 回転とスイッチ機能をサポート
- **USB HID対応**: キーボード・マウス両機能（JIS配列対応）
- **プロファイル管理**: 7つのプリセットプロファイル（JSON設定対応）
- **TFTディスプレイ**: リアルタイムでキー状態とプロファイルを表示
- **モジュラー設計**: 保守性と拡張性を重視した設計
- **ファイルシステム**: LittleFSによる設定の永続化

## ハードウェア仕様

### TFTディスプレイ接続 (ST7789)

| ディスプレイピン | ESP32-S3ピン     | 説明            |
| ---------------- | ---------------- | --------------- |
| GND              | GND              | グランド        |
| VCC              | 3.3V             | 電源            |
| SCL (SCLK)       | GPIO12 (FSPICLK) | SPIクロック     |
| SDA (MOSI)       | GPIO11 (FSPID)   | SPIデータ       |
| RES (RST)        | GPIO13           | リセット        |
| DC               | GPIO14           | データ/コマンド |
| CS               | GPIO10 (FSPICSO) | チップセレクト  |
| BLK              | GPIO09           | バックライト    |

### キーマトリックス接続

| 機能  | ESP32-S3ピン       |
| ----- | ------------------ |
| 行1-4 | GPIO4, 5, 6, 7     |
| 列1-4 | GPIO15, 16, 17, 18 |

### ロータリーエンコーダー接続

| エンコーダー | CLK    | DT     | SW     |
| ------------ | ------ | ------ | ------ |
| 1            | GPIO1  | GPIO2  | GPIO42 |
| 2            | GPIO41 | GPIO40 | GPIO39 |
| 3            | GPIO38 | GPIO37 | GPIO36 |

### その他

- **内蔵LED**: GPIO48 (ステータス表示)

## ソフトウェア構成

### アーキテクチャ

```
src/
├── main.cpp                 # メインファームウェア
├── profile_manager.hpp/cpp  # プロファイル管理（JSON対応）
├── display_manager.hpp/cpp  # ディスプレイ制御
├── keypad_matrix.hpp/cpp    # キーマトリックススキャン
├── hid_sender.hpp/cpp       # USB HID送信
├── FsUtils.hpp/cpp          # ファイルシステムユーティリティ
├── keypad_common.hpp        # 共通定義とenum
├── USBHIDKeyboard_JIS.h     # JIS配列キーコード定義
└── Icondata/                # アイコンデータファイル群
    ├── Icon_ae.h
    ├── Icon_ai.h
    ├── Icon_Alpaca.h
    └── ...
```

### プリセットプロファイル

1. **Explorer**: ファイル操作用プロファイル
2. **After Effects**: Adobe After Effects用
3. **Photoshop**: Adobe Photoshop用
4. **Illustrator**: Adobe Illustrator用
5. **GIMP**: GIMP用
6. **Inkscape**: Inkscape用
7. **FireAlpaca**: FireAlpaca用

### 主要クラス

- **ProfileManager**: プロファイル管理・JSON保存・復元機能
- **DisplayManager**: TFTディスプレイの制御と表示
- **KeypadMatrix**: ハードウェアスキャンとデバウンス処理
- **HIDSender**: USB HIDレポートの送信

## 開発環境

### 必要なソフトウェア

- **PlatformIO**: ESP32開発環境
- **Visual Studio Code**: 推奨エディタ
- **Git**: バージョン管理

### 依存ライブラリ

- `lovyan03/LovyanGFX@^1.2.7`: TFTディスプレイ制御
- `ArduinoJson`: JSON設定ファイル処理
- `ESP32 LittleFS`: ファイルシステム
- `ESP32 Preferences`: 設定保存
- `USBHIDKeyboard`: USB HIDキーボード機能（JIS配列対応）
- `USBHIDMouse`: USB HIDマウス機能

## 使用方法

### プロファイル切り替え

- 設定されたキーの短押し（1.2秒以内）でプロファイル切り替え
- PREF_CHANGE_KEY: 次のプロファイル
- PREF_PREV_KEY: 前のプロファイル

### キー設定

各キーは以下の機能を設定可能：

- **COMBO_KEY**: キーボードキー（修飾キー組み合わせ可能）
- **HOLD_KEY**: 押している間有効なキー（修飾キーなど）
- **MOUSE_LEFT_CLICK / MOUSE_RIGHT_CLICK**: マウスクリック
- **PREF_CHANGE_KEY / PREF_PREV_KEY**: プロファイル切り替え

### キー機能（KeyFunction enum）

```cpp
enum KeyFunction {
    NO_KEY = 0,              // 無効なキー
    COMBO_KEY = 1,           // コンビネーションキー
    HOLD_KEY = 2,            // ホールドキー
    MOUSE_LEFT_CLICK = 3,    // マウス左クリック
    MOUSE_RIGHT_CLICK = 4,   // マウス右クリック
    MOUSE_MIDDLE_CLICK = 5,  // マウス中ボタンクリック
    MOUSE_WHEEL_UP = 6,      // マウスホイールアップ
    MOUSE_WHEEL_DOWN = 7,    // マウスホイールダウン
    PREF_PREV_KEY = 8,       // 前のプロファイル
    PREF_CHANGE_KEY = 9      // 次のプロファイル
};
```

### ロータリーエンコーダー

- **回転**: 設定されたキーを送信
- **スイッチ**: 設定された機能を実行

## 設定とカスタマイズ

### プロファイル設定ファイル

プロファイルはLittleFSを使用してJSON形式で保存されます：

- `/profiles.json`: 現在のプロファイル設定
- `initJson`: コード内のデフォルトプロファイル定義（文字列定数）

### プロファイル構造

```cpp
struct CompleteProfile {
    const char* name;        // プロファイル名
    uint16_t color;          // 表示色
    KeyConfig keys[16];      // 16個のキー設定
    RotaryConfig rotaries[3]; // 3個のロータリーエンコーダー設定
};
```

### アイコンID管理

`keypad_common.hpp`でアイコンIDをenum形式で管理：

```cpp
enum IconId {
    ICON_PROFILE_PREV = 0,
    ICON_PROFILE_NEXT = 1,
    ICON_UNDO = 2,
    ICON_BRUSH = 3,
    // ... 50以上のアイコンを定義
};
```

### JSON設定

プロファイルはJSON形式で管理され、以下の形式で保存されます：

```json
{
  "profiles": [
    {
      "name": "explorer",
      "color": 65535,
      "keys": [8,0,0,0,7,0,0,1,...],
      "rotaries": [0,0,0,0,49,0,0,0,...]
    }
  ]
}
```

## トラブルシューティング

### よくある問題

1. **ディスプレイが表示されない**
   - ピン接続を確認（ST7789用LGFX設定）
   - 電源供給（3.3V）を確認

2. **キーが反応しない**
   - キーマトリックス配線を確認
   - デバウンス設定を調整

3. **USB HIDが認識されない**
   - USBケーブルがデータ対応か確認
   - ESP32-S3のUSB設定を確認

4. **プロファイルが保存されない**
   - LittleFSのマウント状況を確認
   - シリアルモニターでエラーメッセージを確認

5. **アイコンが表示されない**
   - Icondataフォルダ内のヘッダーファイルを確認
   - アイコンIDの定義を確認

## 今後の開発予定

- [x] JSON設定ファイルによる外部プロファイル管理
- [x] LittleFSによる設定の永続化
- [x] JIS配列キーボード対応
- [x] 複数アプリケーション対応プロファイル
- [ ] シリアル接続されたPCで専用アプリによる設定変更
- [ ] PCでアクティブになったアプリを検出してプロファイルを自動的に変更する機能
- [ ] リアルタイムプロファイル編集機能


## ライセンス

このソフトウェアはMITライセンスの下でリリースされています。詳細はLICENSEファイルを参照してください。

## 作者

bry-ful (Hiroshi Furuhashi)

- Twitter: [@bryful](https://twitter.com/bryful)


## 更新履歴

### v1.1.0 (2025-08-05)

- JSON形式プロファイル管理機能を実装
- LittleFSによる設定ファイルの永続化
- 7つのプリセットプロファイル追加（Explorer, After Effects, Photoshop, Illustrator, GIMP, Inkscape, FireAlpaca）
- JIS配列キーボード対応
- 50以上のアイコン定義追加
- モジュラーファイル構成の改善
- ProfileManagerクラスにJSON保存・復元機能追加

### v1.0.0 (2025-07-06)

- 基本機能の実装完了
- モジュラー設計へのリファクタリング完了
- 4x4キーマトリックス対応
- 3個のロータリーエンコーダー対応
- TFTディスプレイ表示機能
- プロファイル管理機能
- USB HID機能（キーボード・マウス）
