#pragma once

#include "SoundSettings.h"
#include "Melody.h"
#include <thread>

class Sound
{
public:
    Sound();
    ~Sound();
    void init(const SoundSettings& settings);
    void play(Melody::Name melodyName);

private:
    void playThreadFn(Melody::Name melodyName);
    void myTone(unsigned int frequency, unsigned long duration);
    void myNoTone();

    SoundSettings settings_;
    std::thread playThread_;
    int wholenote_;
};