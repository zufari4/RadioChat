#pragma once

enum class KeyCommand
{
    ChangeLang,
    Enter,
    Backspace,
    Left,
    Right,
    Up,
    Down,
    Escape
};

const char* key_cmd_2_str(KeyCommand cmd);
