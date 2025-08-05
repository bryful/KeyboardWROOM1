# ESP32-S3 WROOM1 キーパッドプロジェクト

ESP32-S3を使用したプログラマブル・キーパッドファームウェア。
16個のボタンと3個のロータリーエンコーダーを備えた多機能入力デバイスです。

## 主な機能

- **4x4キーマトリックス**: 16個のプログラマブルキー
- **3個のロータリーエンコーダー**: 回転とスイッチ機能をサポート
- **USB HID対応**: キーボード・マウス両機能
- **プロファイル管理**: 複数の設定を切り替え可能
- **TFTディスプレイ**: リアルタイムでキー状態とプロファイルを表示
- **モジュラー設計**: 保守性と拡張性を重視した設計

## ハードウェア仕様

### TFTディスプレイ接続 (ST7789)

| ディスプレイピン | ESP32-S3ピン | 説明 |
|-----------------|-------------|------|
| GND | GND | グランド |
| VCC | 3.3V | 電源 |
| SCL (SCLK) | GPIO12 (FSPICLK) | SPIクロック |
| SDA (MOSI) | GPIO11 (FSPID) | SPIデータ |
| RES (RST) | GPIO13 | リセット |
| DC | GPIO14 | データ/コマンド |
| CS | GPIO10 (FSPICSO) | チップセレクト |
| BLK | GPIO09 | バックライト |

### キーマトリックス接続

| 機能 | ESP32-S3ピン |
|------|-------------|
| 行1-4 | GPIO4, 5, 6, 7 |
| 列1-4 | GPIO15, 16, 17, 18 |

### ロータリーエンコーダー接続

| エンコーダー | CLK | DT | SW |
|-------------|-----|----|----|
| 1 | GPIO1 | GPIO2 | GPIO42 |
| 2 | GPIO41 | GPIO40 | GPIO39 |
| 3 | GPIO38 | GPIO37 | GPIO36 |

### その他

- **内蔵LED**: GPIO48 (ステータス表示)

## ソフトウェア構成

### アーキテクチャ

```
src/
├── main.cpp                 # メインファームウェア
├── profile_manager.hpp/cpp  # プロファイル管理
├── display_manager.hpp/cpp  # ディスプレイ制御
├── keypad_matrix.hpp/cpp    # キーマトリックススキャン
├── hid_sender.hpp/cpp       # USB HID送信
├── keymap_config.hpp/cpp    # キーマップ設定
└── keypad_common.hpp        # 共通定義とenum
```

### 主要クラス

- **ProfileManager**: プロファイルの管理と切り替え
- **DisplayManager**: TFTディスプレイの制御と表示
- **キーマトリックス**: ハードウェアスキャンとデバウンス処理
- **HIDSender**: USB HIDレポートの送信

## 開発環境

### 必要なソフトウェア

- **PlatformIO**: ESP32開発環境
- **Visual Studio Code**: 推奨エディタ
- **Git**: バージョン管理

### 依存ライブラリ

- `lovyan03/LovyanGFX@^1.2.7`: TFTディスプレイ制御
- `USBHIDKeyboard`: USB HIDキーボード機能
- `USBHIDMouse`: USB HIDマウス機能
- `ArduinoJson`: JSON設定ファイル処理

## ビルドと実行

### 1. クローンとセットアップ

```bash
git clone <repository-url>
cd TestWROOM1
```

### 2. PlatformIOでビルド

```bash
pio run
```

### 3. ファームウェア書き込み

```bash
pio run --target upload
```

### 4. シリアルモニター

```bash
pio device monitor
```

## 使用方法

### プロファイル切り替え

- 設定されたキーの短押し（1.2秒以内）でプロファイル切り替え
- PREF_CHANGE_KEY: 次のプロファイル
- PREF_PREV_KEY: 前のプロファイル

### キー設定

各キーは以下の機能を設定可能：

- **COMBO_KEY**: キーボードキー（修飾キー組み合わせ可能）
- **MOUSE_LEFT_CLICK / MOUSE_RIGHT_CLICK**: マウスクリック
- **PREF_CHANGE_KEY / PREF_PREV_KEY**: プロファイル切り替え

### ロータリーエンコーダー

- **回転**: 設定されたキーを送信
- **スイッチ**: 設定された機能を実行

## 設定とカスタマイズ

### アイコンID管理

`keypad_common.hpp`でアイコンIDをenum形式で管理：

```cpp
enum IconId {
    ICON_001 = 1,
    ICON_002 = 2,
    // ...
};
```

### プロファイル定義

`profile_manager.cpp`でデフォルトプロファイルを定義。
将来的にはJSON形式での外部設定ファイル対応予定。

## トラブルシューティング

### よくある問題

1. **ディスプレイが表示されない**
   - ピン接続を確認
   - 電源供給（3.3V）を確認

2. **キーが反応しない**
   - キーマトリックス配線を確認
   - デバウンス設定を調整

3. **USB HIDが認識されない**
   - USBケーブルがデータ対応か確認
   - ESP32-S3のUSB設定を確認

## 今後の開発予定

- [ ] JSON設定ファイルによる外部プロファイル管理
- [ ] Web UIによる設定変更
- [ ] マクロ機能の追加
- [ ] 無線接続対応（Wi-Fi/Bluetooth）
- [ ] カスタムアイコン対応

## 貢献

プルリクエストやイシューの報告を歓迎します。
開発に参加する場合は、コーディング規約に従ってください。

## ライセンス

このソフトウェアはMITライセンスの下でリリースされています。詳細はLICENSEファイルを参照してください。

## 作者

bry-ful (Hiroshi Furuhashi)

- Twitter: [@bryful](https://twitter.com/bryful)
- Email: <bryful@gmail.com>

## 更新履歴

### v1.0.0 (2025-07-06)

- 基本機能の実装完了
- モジュラー設計へのリファクタリング完了
- 4x4キーマトリックス対応
- 3個のロータリーエンコーダー対応
- TFTディスプレイ表示機能
- プロファイル管理機能
- USB HID機能（キーボード・マウス）
