#include "core/SoundManager.hpp"
#include "core/Constants.hpp"

SoundManager::SoundManager()
    : volumePercent(GameConfig::DefaultVolumePercent)
{
    // openFromFile is [[nodiscard]] in SFML 3; if the music file is
    // missing for some reason we still want the game to run silently
    // rather than crash, so the failure is deliberately just ignored here
    // (there is no reasonable in-game recovery for a missing asset).
    const bool musicLoaded = music.openFromFile(std::string(GameConfig::SoundsFolder) + GameConfig::MainMenuMusicFile);
    (void)musicLoaded;

    music.setLooping(true);
    music.setVolume(volumePercent);

    // Pre-fill the effect pool with real sf::Sound instances. SFML 3's
    // sf::Sound has no default constructor, so each pooled instance is
    // constructed with the jump-sound buffer as a placeholder; setBuffer()
    // is called right before every actual playback to pick the correct clip.
    sf::SoundBuffer& placeholderBuffer = soundBuffers.get(std::string(GameConfig::SoundsFolder) + GameConfig::JumpSoundFile);
    for (std::size_t i = 0; i < EffectPoolSize; ++i)
    {
        effectPool.emplace_back(placeholderBuffer);
    }
}

void SoundManager::setVolume(float newVolumePercent)
{
    volumePercent = newVolumePercent;
    music.setVolume(volumePercent);

    // Also retroactively apply to whatever is currently playing in the pool.
    for (sf::Sound& sound : effectPool)
    {
        sound.setVolume(volumePercent);
    }
}

float SoundManager::getVolume() const
{
    return volumePercent;
}

void SoundManager::playMusic()
{
    if (music.getStatus() != sf::Music::Status::Playing)
    {
        music.play();
    }
}

void SoundManager::stopMusic()
{
    music.stop();
}

void SoundManager::playEffect(sf::SoundBuffer& buffer)
{
    effectPool.push_back(effectPool.front());
    effectPool.pop_front();

    sf::Sound& sound = effectPool.back();
    sound.setBuffer(buffer);
    sound.setVolume(volumePercent);
    sound.play();
}

void SoundManager::playJumpSound()
{
    playEffect(soundBuffers.get(std::string(GameConfig::SoundsFolder) + GameConfig::JumpSoundFile));
}

void SoundManager::playShootSound()
{
    playEffect(soundBuffers.get(std::string(GameConfig::SoundsFolder) + GameConfig::ShootSoundFile));
}

void SoundManager::playLoseSound()
{
    playEffect(soundBuffers.get(std::string(GameConfig::SoundsFolder) + GameConfig::LoseSoundFile));
}
