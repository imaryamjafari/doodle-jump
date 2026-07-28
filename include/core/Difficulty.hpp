#pragma once

// New in Phase 2: the three difficulty levels selectable from the Settings
// state. Affects monster health/speed, platform speed, fire rate, and
// whether holes are active (Hard only). Kept as its own small header since
// it is used by many unrelated classes (Game, Settings, MonsterSpawner,
// HoleSpawner, CollisionSystem) that shouldn't need to include each other
// just to know which difficulty is selected.
enum class Difficulty
{
    Easy,
    Medium,
    Hard
};
