#pragma once

enum class QueueMessageType
{
    TypingChar,
    KeyboardCommand,
    AcceptMessage,
    DeliveryMessage,
    PingDone,
    ShowPage,
    TypingMessage,
    ShowProperties,
    EditProperty,
    ShowChatContact,
    ShowContactActions,
    ShowPagePing,
};