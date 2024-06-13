#pragma once

#include <stdint.h>
#include <string>
#include <thread>
#include <atomic>

class Flash;
class Settings;
class Esp;
class KeyHandler;
class Display;
class Radio;
class LedIndicator;
class UI;
class Sound;
class MessageQeueManager;
enum class KeyCommand;


class RadioChat
{
public:
    RadioChat();
    ~RadioChat();
    void init();
    void loop();

private:
    void svc();

    std::atomic_bool workFlag_;
    Settings*     settings_;
    Flash*        flash_;
    Esp*          esp_;
    KeyHandler*   keyHandler_;
    Display*      display_;
    Radio*        radio_;
    UI*           ui_;
    LedIndicator* ledIndicator_;
    Sound*        sound_;
    MessageQeueManager* qeueManager_;
    std::thread   svcThread_;
};