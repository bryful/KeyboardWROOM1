// KeyboardModifiers.cs
namespace ProfileEditor
{
    [Flags]
    public enum KeyboardModifiers : byte
    {
        None = 0,
        KEYBOARD_MODIFIER_LEFTCTRL = 0x01,
        KEYBOARD_MODIFIER_LEFTSHIFT = 0x02,
        KEYBOARD_MODIFIER_LEFTALT = 0x04,
        KEYBOARD_MODIFIER_LEFTGUI = 0x08,
        KEYBOARD_MODIFIER_RIGHTCTRL = 0x10,
        KEYBOARD_MODIFIER_RIGHTSHIFT = 0x20,
        KEYBOARD_MODIFIER_RIGHTALT = 0x40,
        KEYBOARD_MODIFIER_RIGHTGUI = 0x80
    }
}