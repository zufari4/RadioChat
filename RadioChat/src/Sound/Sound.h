#pragma once

#include "SoundSettings.h"
#include "Melody.h"
#include <thread>
#include <atomic>

class Settings;

class Sound
{
public:
    Sound();
    ~Sound();
    void init(Settings& settings);
    void play(Melody::Name melodyName);
    const SoundSettings& getSettings() const;

private:
    static void playThread(void* thisPtr);
    void playImpl();
    void myTone(unsigned int frequency, unsigned long duration);
    void myNoTone();
    void loadSettings(Settings& settings);

    SoundSettings settings_;
    int wholenote_ = 0;
    std::atomic_bool isPlaying_ {false};
    Melody::Name currentMelody_ = Melody::Name::Undefined;
};