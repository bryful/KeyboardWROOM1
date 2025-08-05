// KeyCodeHelper.cs
using System.Reflection;

namespace ProfileEditor
{
    public static class KeyCodeHelper
    {
        private static Dictionary<byte, string>? _keyCodeNames;
        private static Dictionary<string, byte>? _nameToKeyCode;

        public static Dictionary<byte, string> KeyCodeNames
        {
            get
            {
                if (_keyCodeNames == null)
                {
                    InitializeKeyCodeMaps();
                }
                return _keyCodeNames!;
            }
        }

        public static Dictionary<string, byte> NameToKeyCode
        {
            get
            {
                if (_nameToKeyCode == null)
                {
                    InitializeKeyCodeMaps();
                }
                return _nameToKeyCode!;
            }
        }

        private static void InitializeKeyCodeMaps()
        {
            _keyCodeNames = new Dictionary<byte, string>();
            _nameToKeyCode = new Dictionary<string, byte>();

            var fields = typeof(HIDKeyCodes).GetFields(BindingFlags.Public | BindingFlags.Static | BindingFlags.Constant);
            
            foreach (var field in fields)
            {
                if (field.FieldType == typeof(byte) && field.IsLiteral)
                {
                    byte value = (byte)field.GetValue(null)!;
                    string name = field.Name.Replace("HID_KEY_", "");
                    
                    _keyCodeNames[value] = name;
                    _nameToKeyCode[name] = value;
                }
            }
        }

        public static string GetKeyName(byte keyCode)
        {
            return KeyCodeNames.TryGetValue(keyCode, out string? name) ? name : $"Unknown({keyCode:X2})";
        }

        public static byte GetKeyCode(string name)
        {
            return NameToKeyCode.TryGetValue(name.ToUpper(), out byte code) ? code : (byte)0;
        }
    }
}