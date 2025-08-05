// ProfileModels.cs
using System.Text.Json.Serialization;

namespace ProfileEditor
{
    public class KeyConfig
    {
        [JsonPropertyName("function")]
        public KeyFunction Function { get; set; }

        [JsonPropertyName("primaryKey")]
        public byte PrimaryKey { get; set; }

        [JsonPropertyName("modifiers")]
        public KeyboardModifiers Modifiers { get; set; }

        [JsonPropertyName("displayName")]
        public string DisplayName { get; set; } = "";

        [JsonPropertyName("iconId")]
        public IconId IconId { get; set; }
    }

    public class RotaryConfig
    {
        [JsonPropertyName("clockwiseKey")]
        public byte ClockwiseKey { get; set; }

        [JsonPropertyName("counterClockwiseKey")]
        public byte CounterClockwiseKey { get; set; }

        [JsonPropertyName("clockwiseModifier")]
        public KeyboardModifiers ClockwiseModifier { get; set; }

        [JsonPropertyName("counterClockwiseModifier")]
        public KeyboardModifiers CounterClockwiseModifier { get; set; }

        [JsonPropertyName("displayName")]
        public string DisplayName { get; set; } = "";

        [JsonPropertyName("iconId")]
        public IconId IconId { get; set; }

        [JsonPropertyName("switchConfig")]
        public KeyConfig SwitchConfig { get; set; } = new KeyConfig();
    }

    public class Profile
    {
        [JsonPropertyName("name")]
        public string Name { get; set; } = "";

        [JsonPropertyName("keys")]
        public KeyConfig[] Keys { get; set; } = new KeyConfig[16];

        [JsonPropertyName("rotaries")]
        public RotaryConfig[] Rotaries { get; set; } = new RotaryConfig[3];

        public Profile()
        {
            // 配列を初期化
            for (int i = 0; i < 16; i++)
            {
                Keys[i] = new KeyConfig();
            }
            for (int i = 0; i < 3; i++)
            {
                Rotaries[i] = new RotaryConfig();
            }
        }
    }

    public class ProfileCollection
    {
        [JsonPropertyName("profiles")]
        public List<Profile> Profiles { get; set; } = new List<Profile>();
    }
}