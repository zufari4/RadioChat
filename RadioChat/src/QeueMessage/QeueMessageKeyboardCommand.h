#pragma once 

#include "QeueMessageBase.h"
#include "../Keyboard/KeyCommand.h"
#include <stdint.h>

class QeueMessageKeyboardCommand: public QeueMessageBase
{
public:
    QeueMessageKeyboardCommand(KeyCommand cmd);
    ~QeueMessageKeyboardCommand();
    KeyCommand getCommand() const;
    
private:
    KeyCommand cmd_;
};
