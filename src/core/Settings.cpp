#include "core/Settings.hpp"
#include "core/Constants.hpp"
#include <fstream>

Settings::Settings()
    : volumePercent(GameConfig::DefaultVolumePercent)
    , difficulty(GameConfig::DefaultDifficulty)
{
    load();
}

float Settings::getVolume() const
{
    return volumePercent;
}

void Settings::setVolumeLive(float newVolumePercent)
{
    volumePercent = newVolumePercent;
}

void Settings::setVolume(float newVolumePercent)
{
    volumePercent = newVolumePercent;
    save();
}

Difficulty Settings::getDifficulty() const
{
    return difficulty;
}

void Settings::setDifficulty(Difficulty newDifficulty)
{
    difficulty = newDifficulty;
    save();
}

void Settings::load()
{
    std::ifstream file(GameConfig::SettingsFile);
    if (!file.is_open())
    {
        return; // first run: keep the defaults
    }

    float storedVolume = volumePercent;
    int storedDifficulty = static_cast<int>(difficulty);

    if (file >> storedVolume >> storedDifficulty)
    {
        volumePercent = storedVolume;

        if (storedDifficulty >= static_cast<int>(Difficulty::Easy) &&
            storedDifficulty <= static_cast<int>(Difficulty::Hard))
        {
            difficulty = static_cast<Difficulty>(storedDifficulty);
        }
    }
}

void Settings::save() const
{
    std::ofstream file(GameConfig::SettingsFile, std::ios::trunc);
    if (file.is_open())
    {
        file << volumePercent << " " << static_cast<int>(difficulty);
    }
}
