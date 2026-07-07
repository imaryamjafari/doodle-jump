#pragma once

namespace GameConfig
{
    // Window
    constexpr unsigned int WindowWidth  = 750;
    constexpr unsigned int WindowHeight =900;
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

    constexpr float MovingPlatformSpeed = 100.f;       // px / s

    constexpr int NormalPlatformWeight    = 65;
    constexpr int MovingPlatformWeight    = 20;
    constexpr int BreakablePlatformWeight = 15;

    constexpr float BreakableFallSpeed = 300.f;        // px / s

    // Springs
    constexpr float SpringWidthRatio = 3.f / 7.f;      // spring width relative to platform width
    constexpr float SpringHeight = 25.f;
    constexpr int   SpringSpawnChancePercent = 15;

    // spring_sprite.png contains two frames stacked vertically: the top
    // portion is the compressed (resting) spring, the bottom portion is the
    // extended (just-launched) spring. These rects select each frame via
    // sf::Sprite::setTextureRect.
    constexpr int SpringSheetWidth = 25;
    constexpr int SpringCompressedTop = 0;
    constexpr int SpringCompressedHeight = 21;
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
}
