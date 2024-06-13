#include "QeueMessageKeyboardCommand.h"

QeueMessageKeyboardCommand::QeueMessageKeyboardCommand(KeyCommand cmd)
    : QeueMessageBase(QeueMessageType::KeyboardCommand)
    , cmd_(cmd)
{
}

QeueMessageKeyboardCommand::~QeueMessageKeyboardCommand()
{
}

KeyCommand QeueMessageKeyboardCommand::getCommand() const
{
    return cmd_;
}