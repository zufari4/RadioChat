#include "Sound.h"
#include "../Logger/Logger.h"
#include "../Settings/Settings.h"
#include <Arduino.h>

Sound::Sound()
{}

Sound::~Sound()
{}

void Sound::init(Settings& settings)
{
    LOG_INF("-- Initialize sound --");
    loadSettings(settings);
    pinMode(settings_.pins.io, OUTPUT);
    wholenote_ = (60000 * 4) / settings_.tempo;
}

void Sound::play(Melody::Name melodyName)
{
    if (!settings_.enable || isPlaying_) {
        return;
    }
    isPlaying_ = true;
    currentMelody_ = melodyName;
    xTaskCreatePinnedToCore(this->playThread, "PlaySound", 2*1024, this, (configMAX_PRIORITIES - 1) / 2, NULL, ARDUINO_RUNNING_CORE);
}

void Sound::playThread(void* thisPtr)
{
    Sound* self = static_cast<Sound*>(thisPtr);
    self->playImpl();
    vTaskDelete(NULL);
}

void Sound::playImpl()
{
    const Melody::Info& melody = Melody::getMelody(currentMelody_);
    LOG_DBG("Play melody: %s", melody.nameStr.data());
    int noteDuration;

    for (uint8_t i = 0; i < melody.steps.size(); ++i) {
        const Melody::Step& step = melody.steps[i];
        LOG_DBG("Play note %u: %u %d", i, step.frequency, step.duration);
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
        else {
            noteDuration = 0;
        }
        // we only play the note for 90% of the duration, leaving 10% as a pause
        myTone(step.frequency, noteDuration*0.9);
        // Wait for the specified duration before playing the next note.
        delay(noteDuration);
        // stop the waveform generation before the next note.
        myNoTone();
    }

    isPlaying_ = false;
}

void Sound::myTone(unsigned int frequency, unsigned long duration)
{
    tone(settings_.pins.io, frequency, duration);
}

void Sound::myNoTone()
{
    noTone(settings_.pins.io);
}

void Sound::loadSettings(Settings& settings)
{
    auto props = settings.sound();
    settings_.enable = Settings::get_b(eSoundEnable, props);
    settings_.pins.io = Settings::get_i(eSoundPinIO, props);
    settings_.tempo = Settings::get_i(eSoundTempo, props);
    settings_.enablePlayOnPowerOn = Settings::get_b(eSoundEnablePlayOnPowerOn, props);
}

const SoundSettings& Sound::getSettings() const
{
    return settings_;
}
