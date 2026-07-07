#pragma once
#include <map>
#include <memory>
#include <string>
#include <stdexcept>

// ResourceLoader<T>
// in SFML 3, sf::Texture uses loadFromFile and sf::Font uses openFromFile
template <typename T>
struct ResourceLoader;

template <typename T>
class ResourceManager
{
public:
    ResourceManager() = default;

    // Non-copyable
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    T& get(const std::string& filePath)
    {
        auto it = resources.find(filePath);
        if (it != resources.end())
            return *(it->second);

        auto resource = std::make_unique<T>();
        if (!ResourceLoader<T>::load(*resource, filePath))
            throw std::runtime_error("ResourceManager: failed to load file: " + filePath);

        T& reference = *resource;
        resources.emplace(filePath, std::move(resource));
        return reference;
    }

private:
    std::map<std::string, std::unique_ptr<T>> resources;
};

#include <core/ResourceLoader.hpp>