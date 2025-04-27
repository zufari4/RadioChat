#include "QueueMessageKeyboardCommand.h"

QueueMessageKeyboardCommand::QueueMessageKeyboardCommand(KeyCommand cmd)
    : QueueMessageBase(QueueMessageType::KeyboardCommand)
    , cmd_(cmd)
{
}

QueueMessageKeyboardCommand::~QueueMessageKeyboardCommand()
{
}

KeyCommand QueueMessageKeyboardCommand::getCommand() const
{
    return cmd_;
}