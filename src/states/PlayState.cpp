#include "states/PlayState.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include <sstream>

PlayState::PlayState(Game& game_)
    : GameState(game_)
    , backgroundSprite(game.getTextures().get(std::string(GameConfig::AssetsFolder) + GameConfig::BackgroundTexture))
    , scoreText(game.getFonts().get(std::string(GameConfig::FontsFolder) + GameConfig::FontFile))
    , moveLeftPressed(false)
    , moveRightPressed(false)
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

    player = std::make_unique<Player>(leftTexture, rightTexture, leftTuckedTexture, rightTuckedTexture, sf::Vector2f(0.f, 0.f));
    world = std::make_unique<World>(game.getTextures(),
                                     static_cast<float>(GameConfig::WindowWidth),
                                     static_cast<float>(GameConfig::WindowHeight));

    world->reset(*player);
}

void PlayState::handleEvent(const sf::Event& event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->code == sf::Keyboard::Key::Left)  moveLeftPressed = true;
        if (keyPressed->code == sf::Keyboard::Key::Right) moveRightPressed = true;
    }
    else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
    {
        if (keyReleased->code == sf::Keyboard::Key::Left)  moveLeftPressed = false;
        if (keyReleased->code == sf::Keyboard::Key::Right) moveRightPressed = false;
    }
}

void PlayState::update(float deltaTime)
{
    if (world->checkFellBelowScreen(*player))
    {
        game.setLastScore(player->getScore());
        game.updateHighScoreIfNeeded(player->getScore());
        game.requestStateChange(StateID::GameOver);
        return;
    }

    player->handleInput(moveLeftPressed, moveRightPressed);
    world->update(deltaTime, *player);

    std::ostringstream stream;
    stream << "Score: " << static_cast<int>(player->getScore());
    scoreText.setString(stream.str());
}

void PlayState::render(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
    window.clear(sf::Color::White);
    window.draw(backgroundSprite);

    window.setView(world->getView());
    world->render(window);
    player->render(window);

    window.setView(window.getDefaultView());
    scoreText.setPosition({GameConfig::HudMargin, GameConfig::HudMargin});
    window.draw(scoreText);
}
