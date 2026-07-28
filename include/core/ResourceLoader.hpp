#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

template <>
struct ResourceLoader<sf::Texture>
{
    static bool load(sf::Texture& texture, const std::string& filePath)
    {
        return texture.loadFromFile(filePath);
    }
};

template <>
struct ResourceLoader<sf::Font>
{
    static bool load(sf::Font& font, const std::string& filePath)
    {
        return font.openFromFile(filePath);
    }
};

// New in Phase 2: reuses the exact same generic ResourceManager<T> to cache
// sf::SoundBuffer instances (short sound effects), one load per file, cached
// for reuse by every sf::Sound that plays that effect.
template <>
struct ResourceLoader<sf::SoundBuffer>
{
    static bool load(sf::SoundBuffer& buffer, const std::string& filePath)
    {
        return buffer.loadFromFile(filePath);
    }
};
