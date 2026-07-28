#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

// New in Phase 2.
//
// A single shared overlap-check function, as explicitly required: instead
// of writing a separate overlap test for every pair of object types
// (platform-platform, platform-monster, monster-hole, ...), every spawner
// funnels its candidate placement through this one function, which checks
// it against every currently occupied rectangle.
//
// GameConfig::MinClearanceBetweenObjects is used to inflate the candidate
// rectangle so objects keep a small gap rather than merely not overlapping
// pixel-for-pixel.
namespace WorldSpaceOverlap
{
    bool overlapsAny(const sf::FloatRect& candidate, const std::vector<sf::FloatRect>& occupiedRects);
}
