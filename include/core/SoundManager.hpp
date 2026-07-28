#pragma once

#include <SFML/Audio.hpp>
#include <deque>
#include <string>
#include "core/ResourceManager.hpp"
#include "core/ResourceLoader.hpp"

// New in Phase 2.
//
// SoundManager centralizes all audio for the game:
//   - Background music (sf::Music, streamed) for Menu/Settings only.
//   - Short one-shot sound effects (jump / shoot / lose), backed by the
//     same generic ResourceManager<T> template from Phase 1, now
//     instantiated for sf::SoundBuffer exactly as the assignment asks.
//
// A small pool of sf::Sound instances is reused for overlapping effects
// (e.g. rapid-fire shooting) instead of growing without bound, since
// several bullets can be in flight and each needs its own "shoot" sound.

class SoundManager
{
public:
    SoundManager();

    void setVolume(float volumePercent);
    float getVolume() const;

    void playMusic();
    void stopMusic();

    void playJumpSound();
    void playShootSound();
    void playLoseSound();

private:
    void playEffect(sf::SoundBuffer& buffer);

    ResourceManager<sf::SoundBuffer> soundBuffers;

    sf::Music music;
    float volumePercent;

    // Small fixed-size pool of sf::Sound instances recycled round-robin so
    // overlapping effects (e.g. two bullets firing close together) can
    // play at once without unbounded allocation.
    static constexpr std::size_t EffectPoolSize = 8;
    std::deque<sf::Sound> effectPool;
};
