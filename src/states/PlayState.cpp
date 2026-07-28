#include "states/PlayState.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include <sstream>
#include <cmath>
#include <algorithm>

PlayState::PlayState(Game& game_)
    : GameState(game_)
    , backgroundSprite(game.getTextures().get(std::string(GameConfig::AssetsFolder) + GameConfig::BackgroundTexture))
    , scoreText(game.getFonts().get(std::string(GameConfig::FontsFolder) + GameConfig::FontFile))
    , moveLeftPressed(false)
    , moveRightPressed(false)
    , spaceHeld(false)                      // New in Phase 2
    , fireCooldownRemaining(0.f)             // New in Phase 2
    , noseSprite(game.getTextures().get(std::string(GameConfig::AssetsFolder) + GameConfig::NoseTexture)) // New in Phase 2
    , isBeingSuckedIntoHole(false)           // New in Phase 2
    , holeSuckTimeRemaining(0.f)             // New in Phase 2
    , holeSuckTargetCenter(0.f, 0.f)         // New in Phase 2
    , holeSuckStartPosition(0.f, 0.f)        // New in Phase 2
{
    const sf::Vector2u backgroundTextureSize = backgroundSprite.getTexture().getSize();
    backgroundSprite.setScale({
        static_cast<float>(GameConfig::WindowWidth) / static_cast<float>(backgroundTextureSize.x),
        static_cast<float>(GameConfig::WindowHeight) / static_cast<float>(backgroundTextureSize.y)});

    scoreText.setCharacterSize(GameConfig::ScoreFontSize);
    scoreText.setFillColor(sf::Color(50, 50, 50));
    scoreText.setStyle(sf::Text::Bold);

    sf::Texture& leftTexture = game.getTextures().get(std::string(GameConfig::AssetsFolder) + GameConfig::PlayerLeftTexture);
    sf::Texture& rightTexture = game.getTextures().get(std::string(GameConfig::AssetsFolder) + GameConfig::PlayerRightTexture);
    sf::Texture& leftTuckedTexture = game.getTextures().get(std::string(GameConfig::AssetsFolder) + GameConfig::PlayerLeftTuckedTexture);
    sf::Texture& rightTuckedTexture = game.getTextures().get(std::string(GameConfig::AssetsFolder) + GameConfig::PlayerRightTuckedTexture);

    // New in Phase 2: shooting-pose textures, wired through the new
    // constructor overload added to Player.
    sf::Texture& shootingTexture = game.getTextures().get(std::string(GameConfig::AssetsFolder) + GameConfig::ShootingPoseTexture);
    sf::Texture& shootingTuckedTexture = game.getTextures().get(std::string(GameConfig::AssetsFolder) + GameConfig::ShootingPoseTuckedTexture);

    player = std::make_unique<Player>(leftTexture, rightTexture, leftTuckedTexture, rightTuckedTexture,
                                       shootingTexture, shootingTuckedTexture, sf::Vector2f(0.f, 0.f));
    world = std::make_unique<World>(game.getTextures(),
                                     static_cast<float>(GameConfig::WindowWidth),
                                     static_cast<float>(GameConfig::WindowHeight));

    // New in Phase 2: apply the currently selected difficulty to the world
    // (monster health, hole eligibility, platform/monster speed already
    // baked into GameConfig are difficulty-independent constants, but
    // health/holes are not).
    world->setDifficulty(game.getSettings().getDifficulty());

    world->reset(*player);

    // New in Phase 2: the nose is drawn small and upright, glued to the
    // player's top-center only while shooting (see updateNoseAttachment).
    // The bullet itself is a separate, plain round shape (see Bullet.hpp),
    // not another sprite, so a fired shot never looks like a copy of the
    // nose flying off.
    const sf::Vector2u noseTextureSize = noseSprite.getTexture().getSize();
    if (noseTextureSize.x > 0 && noseTextureSize.y > 0)
    {
        noseSprite.setScale({GameConfig::NoseWidth / static_cast<float>(noseTextureSize.x),
                             GameConfig::NoseHeight / static_cast<float>(noseTextureSize.y)});
    }

    // New in Phase 2: background music plays only in Menu/Settings.
    game.getSoundManager().stopMusic();
}

void PlayState::handleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->code == sf::Keyboard::Key::Left)  moveLeftPressed = true;
        if (keyPressed->code == sf::Keyboard::Key::Right) moveRightPressed = true;
        if (keyPressed->code == sf::Keyboard::Key::Space) spaceHeld = true; // New in Phase 2
    }
    else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
    {
        if (keyReleased->code == sf::Keyboard::Key::Left)  moveLeftPressed = false;
        if (keyReleased->code == sf::Keyboard::Key::Right) moveRightPressed = false;
        if (keyReleased->code == sf::Keyboard::Key::Space) spaceHeld = false; // New in Phase 2
    }
}

void PlayState::update(float deltaTime)
{
    // New in Phase 2: once the hole-suck sequence has started, ordinary
    // gameplay is completely frozen (no world update, no falling check) —
    // only the shrink-toward-the-hole animation runs until it ends the
    // game itself.
    if (isBeingSuckedIntoHole)
    {
        updateHoleSuckSequence(deltaTime);
        return;
    }

    if (world->checkFellBelowScreen(*player))
    {
        endGameAndTransition();
        return;
    }

    player->handleInput(moveLeftPressed, moveRightPressed);

    // New in Phase 2: shooting is completely independent of movement/jump
    // state — held Space fires on a fixed cooldown that only depends on
    // difficulty.
    player->setShooting(spaceHeld);
    fireCooldownRemaining -= deltaTime;

    if (spaceHeld)
    {
        // Keep the nose glued to the player every frame it's visible, not
        // only on the frame a shot is actually fired.
        const sf::Vector2f noseTipPosition = updateNoseAttachment();

        if (fireCooldownRemaining <= 0.f)
        {
            // Bullet centers itself on whatever position it's given (see
            // Bullet's constructor / setOrigin), so noseTipPosition can be
            // passed directly — the bullet spawns exactly where the nose
            // ends, not above or offset from it.
            world->fireBullet(noseTipPosition);
            game.getSoundManager().playShootSound();

            switch (game.getSettings().getDifficulty())
            {
                case Difficulty::Easy:   fireCooldownRemaining = GameConfig::FireCooldownEasy;   break;
                case Difficulty::Medium: fireCooldownRemaining = GameConfig::FireCooldownMedium; break;
                case Difficulty::Hard:   fireCooldownRemaining = GameConfig::FireCooldownHard;   break;
            }
        }
    }

    const bool wasMovingDownwardBeforeUpdate = player->isMovingDownward();

    world->update(deltaTime, *player);

    // New in Phase 2: react to whatever CollisionSystem/World reported
    // this frame.
    const WorldUpdateOutcome outcome = world->getLastOutcome();
    if (outcome == WorldUpdateOutcome::CaughtByHole)
    {
        isBeingSuckedIntoHole = true;
        holeSuckTimeRemaining = GameConfig::HoleSuckDurationSeconds;
        holeSuckTargetCenter = world->getLastCaughtHoleCenter();
        holeSuckStartPosition = player->getPosition();
        return;
    }
    if (outcome == WorldUpdateOutcome::KilledByMonster)
    {
        endGameAndTransition();
        return;
    }

    // New in Phase 2: a jump sound plays every time the player bounces off
    // a platform/spring/monster. That is exactly the frame where velocity
    // flips from "moving downward" to "moving upward", so this is
    // detected without CollisionSystem needing to know about sound at all.
    if (wasMovingDownwardBeforeUpdate && !player->isMovingDownward())
    {
        game.getSoundManager().playJumpSound();
    }

    std::ostringstream stream;
    stream << "Score: " << static_cast<int>(player->getScore());
    scoreText.setString(stream.str());
}

// New in Phase 2.
void PlayState::updateHoleSuckSequence(float deltaTime)
{
    holeSuckTimeRemaining -= deltaTime;

    const float progress = std::clamp(
        1.f - (holeSuckTimeRemaining / GameConfig::HoleSuckDurationSeconds), 0.f, 1.f);

    // Lerp the player's position toward the hole's center and shrink them
    // toward nothing as progress approaches 1.
    const sf::Vector2f interpolatedPosition(
        holeSuckStartPosition.x + (holeSuckTargetCenter.x - holeSuckStartPosition.x) * progress,
        holeSuckStartPosition.y + (holeSuckTargetCenter.y - holeSuckStartPosition.y) * progress);
    player->setPosition(interpolatedPosition);
    player->setShrinkFactor(1.f - progress);

    if (holeSuckTimeRemaining <= 0.f)
    {
        endGameAndTransition();
    }
}

// New in Phase 2: shared by both lose paths (falling off-screen, killed by
// monster/hole) so GameOver transition + scoring + sound stay in one place.
void PlayState::endGameAndTransition()
{
    game.setLastScore(player->getScore());
    game.updateHighScoreIfNeeded(player->getScore());
    game.getSoundManager().playLoseSound();
    game.requestStateChange(StateID::GameOver);
}

// Glues noseSprite to the player's top-center and returns the exact
// world-space point where the nose ends (its own top edge) — bullets
// spawn from precisely there, not above or below it.

sf::Vector2f PlayState::updateNoseAttachment()
{
    const sf::FloatRect playerBounds = player->getBounds();
    const float noseCenterX = playerBounds.position.x + playerBounds.size.x / 2.f;
    
    float noseOffsetY = 0.f;
    if (player->isShooting())
        noseOffsetY = player->isLegsTucked() ? GameConfig::noseOffsetTuckedY : GameConfig::noseOffsetY;
    
    const float noseTopY = playerBounds.position.y - GameConfig::NoseHeight + noseOffsetY;
    
    noseSprite.setPosition({noseCenterX - GameConfig::NoseWidth / 2.f, noseTopY});
    
    return sf::Vector2f(noseCenterX, noseTopY);
}

void PlayState::render(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
    window.clear(sf::Color::White);
    window.draw(backgroundSprite);

    window.setView(world->getView());
    world->render(window);
    player->render(window);

    // New in Phase 2: the nose is only glued to the player while actively
    // shooting (matches the requirement that it should visually attach
    // during shooting, not sit there permanently).
    if (spaceHeld && !isBeingSuckedIntoHole)
    {
        window.draw(noseSprite);
    }

    window.setView(window.getDefaultView());
    scoreText.setPosition({GameConfig::HudMargin, GameConfig::HudMargin});
    window.draw(scoreText);
}
