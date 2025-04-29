#include "RadioCommand.h"

const char* commandStr(RadioCommand cmd)
{
    switch (cmd)
    {
    case RadioCommand::MessageNew: return "MessageNew";
    case RadioCommand::MessageDelivered: return "MessageDelivered";
    case RadioCommand::Ping: return "Ping";
    case RadioCommand::PingDelivered: return "PingDelivered";
    default: return "Unknown";
    }
}
