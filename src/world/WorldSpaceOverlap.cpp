#include "world/WorldSpaceOverlap.hpp"
#include "core/Constants.hpp"

namespace WorldSpaceOverlap
{
    bool overlapsAny(const sf::FloatRect& candidate, const std::vector<sf::FloatRect>& occupiedRects)
    {
        // Inflate the candidate by the required clearance on every side so
        // objects keep a small gap rather than merely avoiding pixel-exact
        // overlap.
        const float margin = GameConfig::MinClearanceBetweenObjects;
        const sf::FloatRect inflatedCandidate(
            {candidate.position.x - margin, candidate.position.y - margin},
            {candidate.size.x + margin * 2.f, candidate.size.y + margin * 2.f});

        for (const sf::FloatRect& occupied : occupiedRects)
            if (inflatedCandidate.findIntersection(occupied).has_value())
                return true;

        return false;
    }
}
