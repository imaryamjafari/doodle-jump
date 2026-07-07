#pragma once
#include <SFML/Graphics.hpp>
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
