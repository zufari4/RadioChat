#pragma once 

#include "QueueMessageBase.h"
#include "../Keyboard/KeyCommand.h"
#include <stdint.h>

class QueueMessageKeyboardCommand: public QueueMessageBase
{
public:
    QueueMessageKeyboardCommand(KeyCommand cmd);
    ~QueueMessageKeyboardCommand();
    KeyCommand getCommand() const;
    
private:
    KeyCommand cmd_;
};
