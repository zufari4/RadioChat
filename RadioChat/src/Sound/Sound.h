#pragma once

#include "SoundSettings.h"
#include "Melody.h"
#include <thread>
#include <atomic>

class Sound
{
public:
    Sound();
    ~Sound();
    void init(const SoundSettings& settings);
    void play(Melody::Name melodyName);

private:
    static void playThread(void* thisPtr);
    void playImpl();
    void myTone(unsigned int frequency, unsigned long duration);
    void myNoTone();

    SoundSettings settings_;
    int wholenote_ = 0;
    std::atomic_bool isPlaying_ {false};
    Melody::Name currentMelody_ = Melody::Name::Undefined;
};