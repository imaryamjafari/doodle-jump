#pragma once

#include "core/Difficulty.hpp"

// New in Phase 2.
//
// Settings owns the two values configurable from the Settings state
// (volume and difficulty) and persists them to GameConfig::SettingsFile so
// they survive between runs of the program.
class Settings
{
public:
    Settings();

    float getVolume() const;

    // Updates the in-memory volume only (no disk write). Used while the
    // volume slider is actively being dragged, so a fast drag doesn't
    // write to disk on every single mouse-move event.
    void setVolumeLive(float volumePercent);

    // Updates the in-memory volume AND persists it to disk immediately.
    // Used for one-off changes (or call save() explicitly once a drag
    // ends, after a series of setVolumeLive() calls).
    void setVolume(float volumePercent);

    Difficulty getDifficulty() const;
    void setDifficulty(Difficulty difficulty);

    void load();
    void save() const;

private:
    float volumePercent;
    Difficulty difficulty;
};
