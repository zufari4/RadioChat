#include "KeyCommand.h"

const char* key_cmd_2_str(KeyCommand cmd)
{
    switch (cmd) {
    case KeyCommand::Backspace: return "Backspace";
    case KeyCommand::Enter    : return "Enter";
    case KeyCommand::Left     : return "Left";
    case KeyCommand::Right    : return "Right";
    case KeyCommand::Up       : return "Up";
    case KeyCommand::Down     : return "Down";
    case KeyCommand::Escape   : return "Escape";
    default: return "Unknown";
    }
}