#pragma once
#include "core/Difficulty.hpp"

namespace GameConfig
{
    // Window
    constexpr unsigned int WindowWidth  = 750;
    constexpr unsigned int WindowHeight = 900;
    constexpr const char*  WindowTitle  = "Doodle Jump";
    constexpr unsigned int FrameRateLimit = 60;

    // Paths
    constexpr const char* AssetsFolder = "assets/";
    constexpr const char* FontsFolder  = "fonts/";
    constexpr const char* FontFile     = "ariblk.ttf";
    constexpr const char* HighScoreFile = "highscore.txt";

    constexpr const char* BackgroundTexture        = "background.png";
    constexpr const char* NormalPlatformTexture    = "normal_platform.png";
    constexpr const char* MovingPlatformTexture    = "moving_platform.png";
    constexpr const char* BrokenPlatformTexture    = "broken_platform.png";
    constexpr const char* SpringTexture            = "spring_sprite.png";
    constexpr const char* PlayerLeftTexture        = "left_doodle.png";
    constexpr const char* PlayerRightTexture       = "right_doodle.png";
    constexpr const char* PlayerLeftTuckedTexture  = "left_tucked_doodle.png";
    constexpr const char* PlayerRightTuckedTexture = "right_tucked_doodle.png";
    constexpr const char* StartButtonTexture       = "start_button.png";
    constexpr const char* RestartButtonTexture     = "restart_button.png";
    constexpr const char* MenuButtonTexture        = "menu_button.png";

    // Physics
    constexpr float Gravity = 900.f;                   // px / s^2, downward acceleration
    constexpr float PlayerHorizontalSpeed = 600.f;     // px / s
    constexpr float PlayerJumpSpeed = 620.f;           // px / s, upward (negative Y), normal bounce
    constexpr float SpringJumpSpeed = 1300.f;          // px / s, upward (negative Y), spring bounce
    constexpr float MaxFallSpeed = 1400.f;             // px / s

    // Player
    constexpr float PlayerWidth  = 90.f;
    constexpr float PlayerHeight = 90.f;
    constexpr float PlayerSpawnOffsetY = 150.f; 
    constexpr float LegsTuckedDurationSeconds = 0.25f;

    // Platforms
    constexpr float PlatformWidth  = PlayerWidth * 1.5f;
    constexpr float PlatformHeight = 22.f;
    constexpr float MinVerticalGapBetweenPlatforms = 40.f;

    constexpr float MaxJumpHeight = (PlayerJumpSpeed * PlayerJumpSpeed) / (2.f * Gravity);
    constexpr float MaxVerticalGapBetweenPlatforms = MaxJumpHeight - (PlayerHeight + PlatformHeight + 5.);

    constexpr float MovingPlatformSpeedEasy = 100.f;       // px / s
    constexpr float MovingPlatformSpeedMedium = 150.f;       // px / s
    constexpr float MovingPlatformSpeedHard = 150.f;       // px / s

    constexpr int NormalPlatformWeight    = 65;
    constexpr int MovingPlatformWeight    = 20;
    constexpr int BreakablePlatformWeight = 15;

    constexpr float BreakableFallSpeed = 300.f;        // px / s

    // Springs
    constexpr float SpringWidthRatio = 1.f / 2.f;      // spring width relative to platform width
    constexpr float SpringHeight = 25.f;
    constexpr int   SpringSpawnChancePercent = 15;

    constexpr int SpringSheetWidth = 25;
    constexpr int SpringCompressedTop = 0;
    constexpr int SpringCompressedHeight = 17;
    constexpr int SpringExtendedTop = 21;
    constexpr int SpringExtendedHeight = 31;

    // How long the spring is shown "extended" after launching the player
    // before it resets back to its compressed resting frame.
    constexpr float SpringExtendedDurationSeconds = 0.25f;

    // Scoring
    constexpr float ScorePerPixelClimbed = 0.1f;

    // Camera
    constexpr float CameraFollowThresholdRatio = 0.5f;  // upper half of the screen triggers scroll

    // UI
    constexpr unsigned int TitleFontSize = 60;
    constexpr unsigned int ScoreFontSize = 30;
    constexpr unsigned int MessageFontSize = 50;
    constexpr unsigned int ButtonWidth = 200;
    constexpr unsigned int ButtonHeight = 70;

    constexpr float TitleVerticalPositionRatio = 1.f / 3.f;   // fraction of window height
    constexpr float SubTextSpacingFromTitle = 60.f;           // px below the title/message text
    constexpr float SecondSubTextSpacingFromTitle = 100.f;     // px below the title/message text
    constexpr float ButtonSpacing = 25.f;                     // px gap between stacked buttons
    constexpr float FirstPlatformBottomMargin = 40.f;         // px above the window's bottom edge
    constexpr float BreakablePlatformDespawnMultiplier = 3.f; // x window height, safely below any camera position
    constexpr int   PercentRollMax = 100;                     // upper bound for a percent-chance dice roll
    constexpr float HudMargin = 10.f;                         // px from the window edge for on-screen score text

    // New assets
    constexpr const char* BlueMonsterTexture      = "BlueMonster.png";
    constexpr const char* GreenMonsterTexture     = "green_monster.png";
    constexpr const char* HoleTexture             = "hole.png";
    constexpr const char* NoseTexture             = "Nose.png";
    constexpr const char* ShootingPoseTexture     = "shooting@pose.png";
    constexpr const char* ShootingPoseTuckedTexture = "shooting@pose_tucked.png";
    constexpr const char* SettingsButtonTexture   = "Settings_button.png";
    constexpr const char* BackButtonTexture       = "back_button.png";

    // Sounds
    constexpr const char* SoundsFolder          = "sounds/";
    constexpr const char* MainMenuMusicFile     = "MainMenu_Song.flac";
    constexpr const char* JumpSoundFile         = "Jumping_Sound.wav";
    constexpr const char* ShootSoundFile        = "Shooting_Sound.wav";
    constexpr const char* LoseSoundFile         = "Loosing_Sound.wav";
    constexpr const char* SettingsFile          = "settings.txt";

    constexpr float DefaultVolumePercent = 100.f; // 0-100
    constexpr Difficulty DefaultDifficulty = Difficulty::Easy;

    // Monsters
    constexpr float MonsterWidth  = 80.f;
    constexpr float MonsterHeight = 51.f;
    constexpr float GreenMonsterHeight = 28.f; // green_monster.png has a flatter native aspect ratio than BlueMonster.png
    constexpr float MonsterSpeed = 100.f;              // px / s, horizontal patrol speed
    constexpr int   MonsterSpawnChancePercent = 12;    // % chance a newly generated platform gets a monster nearby

    // Monster health scales with difficulty: harder levels need more bullets.
    constexpr int MonsterHealthEasy   = 1;
    constexpr int MonsterHealthMedium = 2;
    constexpr int MonsterHealthHard   = 2;

    // Monsters/holes must never be generated before the player has had a moment to get moving
    constexpr float MonsterHoleSpawnDelaySeconds = 3.f;
    constexpr int AttemptOverlapCheck = 5;

    // applied when the player lands on a monster's back.
    constexpr float MonsterBounceJumpSpeed = 900.f;

    // Bullets
    constexpr float NoseWidth  = 15.f;
    constexpr float NoseHeight = 22.f;

    constexpr float noseOffsetY = 26.f;
    constexpr float noseOffsetTuckedY = 21.f;

    constexpr float BulletRadius = 6.f;
    constexpr float BulletSpeed = 1800.f;               // px / s, upward

    constexpr float FireCooldownEasy   = 0.18f;
    constexpr float FireCooldownMedium = 0.32f;
    constexpr float FireCooldownHard   = 0.32f;

    // Holes
    constexpr float HoleSmallSize = 74.f;
    constexpr float HoleLargeSize = 120.f;
    constexpr int   HoleSpawnChancePercent = 5; // % chance a newly generated platform gets a hole nearby (Hard only)

    constexpr float HoleSuckDurationSeconds = 1.f; // time for the player to shrink to nothing once caught

    // Shared spawn
    // Minimum clearance kept between any two world objects (platform,
    // monster, hole) so nothing overlaps or touches, used by the single
    // shared overlap-check function all spawners call into.
    constexpr float MinClearanceBetweenObjects = 12.f;

    // Scoring
    constexpr const char* HighScoreFileHeader = "EASY MEDIUM HARD";
}

