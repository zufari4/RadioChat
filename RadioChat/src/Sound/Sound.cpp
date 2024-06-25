#include "Sound.h"
#include "../Logger/Logger.h"
#include <Arduino.h>

Sound::Sound()
{}

Sound::~Sound()
{}

void Sound::init(const SoundSettings& settings)
{
    LOG_INF("-- Initialize sound --");
    LOG_INF("io pin: %u", settings.pins.io);
    LOG_INF("tempo: %u", settings.tempo);
    settings_ = settings;
    pinMode(settings_.pins.io, OUTPUT);
    wholenote_ = (60000 * 4) / settings_.tempo;
}

void Sound::play(Melody::Name melodyName)
{
    if (!settings_.enable || playThread_.joinable()) {
        return;
    }
    playThread_ = std::thread(&Sound::playThreadFn, this, melodyName);
}

void Sound::playThreadFn(Melody::Name melodyName)
{
    const Melody::Info& melody = Melody::getMelody(melodyName);
    
    int noteDuration;

    for (const Melody::Step& step: melody.steps) {
        // calculates the duration of each note
        if (step.duration > 0) {
            // regular note, just proceed
            noteDuration = (wholenote_) / step.duration;
        }
        else if (step.duration < 0) {
            // dotted notes are represented with negative durations!!
            noteDuration = (wholenote_) / abs(step.duration);
            noteDuration *= 1.5; // increases the duration in half for dotted notes
        }
        // we only play the note for 90% of the duration, leaving 10% as a pause
        myTone(step.frequency, noteDuration*0.9);
        // Wait for the specief duration before playing the next note.
        delay(noteDuration);
        // stop the waveform generation before the next note.
        myNoTone();
    }
}

void Sound::myTone(unsigned int frequency, unsigned long duration)
{
    tone(settings_.pins.io, frequency, duration);
}

void Sound::myNoTone()
{
    noTone(settings_.pins.io);
}