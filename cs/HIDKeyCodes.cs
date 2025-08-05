// HIDKeyCodes.cs
namespace ProfileEditor
{
    public static class HIDKeyCodes
    {
        // アルファベット
        public const byte HID_KEY_A = 0x04;
        public const byte HID_KEY_B = 0x05;
        public const byte HID_KEY_C = 0x06;
        public const byte HID_KEY_D = 0x07;
        public const byte HID_KEY_E = 0x08;
        public const byte HID_KEY_F = 0x09;
        public const byte HID_KEY_G = 0x0A;
        public const byte HID_KEY_H = 0x0B;
        public const byte HID_KEY_I = 0x0C;
        public const byte HID_KEY_J = 0x0D;
        public const byte HID_KEY_K = 0x0E;
        public const byte HID_KEY_L = 0x0F;
        public const byte HID_KEY_M = 0x10;
        public const byte HID_KEY_N = 0x11;
        public const byte HID_KEY_O = 0x12;
        public const byte HID_KEY_P = 0x13;
        public const byte HID_KEY_Q = 0x14;
        public const byte HID_KEY_R = 0x15;
        public const byte HID_KEY_S = 0x16;
        public const byte HID_KEY_T = 0x17;
        public const byte HID_KEY_U = 0x18;
        public const byte HID_KEY_V = 0x19;
        public const byte HID_KEY_W = 0x1A;
        public const byte HID_KEY_X = 0x1B;
        public const byte HID_KEY_Y = 0x1C;
        public const byte HID_KEY_Z = 0x1D;

        // 数字
        public const byte HID_KEY_1 = 0x1E;
        public const byte HID_KEY_2 = 0x1F;
        public const byte HID_KEY_3 = 0x20;
        public const byte HID_KEY_4 = 0x21;
        public const byte HID_KEY_5 = 0x22;
        public const byte HID_KEY_6 = 0x23;
        public const byte HID_KEY_7 = 0x24;
        public const byte HID_KEY_8 = 0x25;
        public const byte HID_KEY_9 = 0x26;
        public const byte HID_KEY_0 = 0x27;

        // テンキー
        public const byte HID_KEY_KP_1 = 0x59;
        public const byte HID_KEY_KP_2 = 0x5A;
        public const byte HID_KEY_KP_3 = 0x5B;
        public const byte HID_KEY_KP_4 = 0x5C;
        public const byte HID_KEY_KP_5 = 0x5D;
        public const byte HID_KEY_KP_6 = 0x5E;
        public const byte HID_KEY_KP_7 = 0x5F;
        public const byte HID_KEY_KP_8 = 0x60;
        public const byte HID_KEY_KP_9 = 0x62;
        public const byte HID_KEY_KP_0 = 0x63;
        public const byte HID_KEY_KP_ENTER = 0x58;

        // 制御キー
        public const byte HID_KEY_ENTER = 0x28;
        public const byte HID_KEY_ESC = 0x29;
        public const byte HID_KEY_BACKSPACE = 0x2A;
        public const byte HID_KEY_TAB = 0x2B;
        public const byte HID_KEY_SPACE = 0x2C;
        public const byte HID_KEY_DELETE = 0x4C;
        public const byte HID_KEY_PAGEUP = 0x4B;
        public const byte HID_KEY_PAGEDOWN = 0x4E;

        // 矢印キー
        public const byte HID_KEY_ARROWRIGHT = 0x4F;
        public const byte HID_KEY_ARROWLEFT = 0x50;
        public const byte HID_KEY_ARROWDOWN = 0x51;
        public const byte HID_KEY_ARROWUP = 0x52;

        // 記号・特殊キー
        public const byte HID_KEY_MINUS = 0x2D;
        public const byte HID_KEY_CARET = 0x2E;
        public const byte HID_KEY_AT = 0x2F;
        public const byte HID_KEY_LEFTBRACE = 0x30;
        public const byte HID_KEY_RIGHTBRACE = 0x32;
        public const byte HID_KEY_COLON = 0x92;
        public const byte HID_KEY_SEMICOLON = 0x33;
        public const byte HID_KEY_COMMA = 0x36;
        public const byte HID_KEY_DOT = 0x37;
        public const byte HID_KEY_SLASH = 0x38;
        public const byte HID_KEY_UNDERSCORE = 0x93;
        public const byte HID_KEY_BACKSLASH = 0x31;

        // テンキー記号
        public const byte HID_KEY_KP_PLUS = 0x57;
        public const byte HID_KEY_KP_MINUS = 0x56;
        public const byte HID_KEY_KP_ASTERISK = 0x55;
        public const byte HID_KEY_KP_SLASH = 0x54;
        public const byte HID_KEY_KP_DOT = 0x63;
        public const byte HID_KEY_KP_COMMA = 0x85;
        public const byte HID_KEY_KP_EQUAL = 0x67;

        // JIS日本語特有キー
        public const byte HID_KEY_RO = 0x87;              // ろ
        public const byte HID_KEY_KANA = 0x88;            // かな
        public const byte HID_KEY_EISU = 0x94;            // 英数
        public const byte HID_KEY_HENKAN = 0x8A;          // 変換
        public const byte HID_KEY_MUHENKAN = 0x8B;        // 無変換
        public const byte HID_KEY_HIRAGANA = 0x8C;        // ひらがな/カタカナ
        public const byte HID_KEY_ZENKAKU_HANKAKU = 0x8D; // 全角/半角

        // ファンクションキー
        public const byte HID_KEY_F1 = 0x3A;
        public const byte HID_KEY_F2 = 0x3B;
        public const byte HID_KEY_F3 = 0x3C;
        public const byte HID_KEY_F4 = 0x3D;
        public const byte HID_KEY_F5 = 0x3E;
        public const byte HID_KEY_F6 = 0x3F;
        public const byte HID_KEY_F7 = 0x40;
        public const byte HID_KEY_F8 = 0x41;
        public const byte HID_KEY_F9 = 0x42;
        public const byte HID_KEY_F10 = 0x43;
        public const byte HID_KEY_F11 = 0x44;
        public const byte HID_KEY_F12 = 0x45;
    }
}