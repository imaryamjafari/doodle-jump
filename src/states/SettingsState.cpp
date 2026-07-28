#include "states/SettingsState.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include <sstream>
#include <algorithm>

namespace
{
    // Layout constants local to this file only (not general game balance,
    // so kept out of GameConfig, matching how MenuState/GameOverState keep
    // their own purely-local layout numbers inline).
    constexpr float TrackWidth = 300.f;
    constexpr float TrackHeight = 6.f;
    constexpr float HandleRadius = 12.f;
    constexpr float DifficultyButtonWidth = 140.f;
    constexpr float DifficultyButtonHeight = 50.f;
    constexpr float DifficultyButtonSpacing = 20.f;
}

SettingsState::SettingsState(Game& game_)
    : GameState(game_)
    , titleText(game.getFonts().get(std::string(GameConfig::FontsFolder) + GameConfig::FontFile))
    , volumeLabel(game.getFonts().get(std::string(GameConfig::FontsFolder) + GameConfig::FontFile))
    , draggingVolumeHandle(false)
    , difficultyLabels{
          sf::Text(game.getFonts().get(std::string(GameConfig::FontsFolder) + GameConfig::FontFile)),
          sf::Text(game.getFonts().get(std::string(GameConfig::FontsFolder) + GameConfig::FontFile)),
          sf::Text(game.getFonts().get(std::string(GameConfig::FontsFolder) + GameConfig::FontFile))}
    , backButtonSprite(game.getTextures().get(std::string(GameConfig::AssetsFolder) + GameConfig::BackButtonTexture))
{
    titleText.setString("Settings");
    titleText.setCharacterSize(GameConfig::TitleFontSize);
    titleText.setFillColor(sf::Color(60, 60, 60));
    const sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin({titleBounds.size.x / 2.f, titleBounds.size.y / 2.f});
    titleText.setPosition({GameConfig::WindowWidth / 2.f, GameConfig::WindowHeight * GameConfig::TitleVerticalPositionRatio});

    // Volume slider track, centered horizontally.
    volumeTrack.setSize({TrackWidth, TrackHeight});
    volumeTrack.setFillColor(sf::Color(200, 195, 185));
    volumeTrack.setOrigin({0.f, TrackHeight / 2.f});
    volumeTrack.setPosition({GameConfig::WindowWidth / 2.f - TrackWidth / 2.f, GameConfig::WindowHeight / 2.f});

    volumeHandle.setRadius(HandleRadius);
    volumeHandle.setOrigin({HandleRadius, HandleRadius});
    volumeHandle.setFillColor(sf::Color(40, 90, 190));

    volumeLabel.setCharacterSize(GameConfig::ScoreFontSize);
    volumeLabel.setFillColor(sf::Color(60, 60, 60));
    volumeLabel.setStyle(sf::Text::Bold);

    // Position the handle according to the currently persisted volume.
    const float initialVolume = game.getSettings().getVolume();
    const float handleX = volumeTrack.getPosition().x + (initialVolume / 100.f) * TrackWidth;
    volumeHandle.setPosition({handleX, volumeTrack.getPosition().y});

    // Three difficulty buttons, laid out horizontally and centered.
    const float totalWidth = DifficultyButtonWidth * 3.f + DifficultyButtonSpacing * 2.f;
    const float startX = GameConfig::WindowWidth / 2.f - totalWidth / 2.f;
    const float buttonsY = GameConfig::WindowHeight / 2.f + 100.f;

    const char* labels[3] = {"Easy", "Medium", "Hard"};
    for (int i = 0; i < 3; ++i)
    {
        difficultyButtons[i].setSize({DifficultyButtonWidth, DifficultyButtonHeight});
        difficultyButtons[i].setPosition({startX + static_cast<float>(i) * (DifficultyButtonWidth + DifficultyButtonSpacing), buttonsY});

        difficultyLabels[i].setString(labels[i]);
        difficultyLabels[i].setCharacterSize(GameConfig::ScoreFontSize);
        difficultyLabels[i].setStyle(sf::Text::Bold);
        difficultyLabels[i].setFillColor(sf::Color::White);
        const sf::FloatRect labelBounds = difficultyLabels[i].getLocalBounds();
        difficultyLabels[i].setOrigin({labelBounds.size.x / 2.f, labelBounds.size.y / 2.f + labelBounds.position.y});
        difficultyLabels[i].setPosition({
            difficultyButtons[i].getPosition().x + DifficultyButtonWidth / 2.f,
            difficultyButtons[i].getPosition().y + DifficultyButtonHeight / 2.f});
    }
    refreshDifficultyButtonColors();

    const sf::Vector2u backTextureSize = backButtonSprite.getTexture().getSize();
    backButtonSprite.setScale({
        static_cast<float>(GameConfig::ButtonWidth) / static_cast<float>(backTextureSize.x),
        static_cast<float>(GameConfig::ButtonHeight) / static_cast<float>(backTextureSize.y)});
    backButtonSprite.setPosition({
        GameConfig::WindowWidth / 2.f - GameConfig::ButtonWidth / 2.f,
        buttonsY + DifficultyButtonHeight + GameConfig::ButtonSpacing});

    // Background music continues playing in Settings, exactly as in Menu.
    game.getSoundManager().playMusic();
}

sf::FloatRect SettingsState::difficultyButtonBounds(int index) const
{
    return difficultyButtons[index].getGlobalBounds();
}

void SettingsState::refreshDifficultyButtonColors()
{
    const Difficulty current = game.getSettings().getDifficulty();
    for (int i = 0; i < 3; ++i)
    {
        const bool isSelected = (static_cast<int>(current) == i);
        difficultyButtons[i].setFillColor(isSelected ? sf::Color(60, 150, 90) : sf::Color(150, 150, 150));
    }
}

void SettingsState::applyVolumeFromMouseX(float mouseX)
{
    const float trackLeft = volumeTrack.getPosition().x;
    const float clampedX = std::clamp(mouseX, trackLeft, trackLeft + TrackWidth);
    const float ratio = (clampedX - trackLeft) / TrackWidth;

    volumeHandle.setPosition({clampedX, volumeTrack.getPosition().y});

    const float newVolume = ratio * 100.f;
    game.getSettings().setVolumeLive(newVolume); // no disk write yet, see handleEvent's MouseButtonReleased
    game.getSoundManager().setVolume(newVolume);
}

void SettingsState::handleEvent(const sf::Event& event)
{
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left)
        {
            const sf::Vector2f clickPosition(static_cast<float>(mouseButtonPressed->position.x),
                                              static_cast<float>(mouseButtonPressed->position.y));

            if (volumeHandle.getGlobalBounds().contains(clickPosition) ||
                volumeTrack.getGlobalBounds().contains(clickPosition))
            {
                draggingVolumeHandle = true;
                applyVolumeFromMouseX(clickPosition.x);
            }
            else if (difficultyButtonBounds(0).contains(clickPosition))
            {
                game.getSettings().setDifficulty(Difficulty::Easy);
                refreshDifficultyButtonColors();
            }
            else if (difficultyButtonBounds(1).contains(clickPosition))
            {
                game.getSettings().setDifficulty(Difficulty::Medium);
                refreshDifficultyButtonColors();
            }
            else if (difficultyButtonBounds(2).contains(clickPosition))
            {
                game.getSettings().setDifficulty(Difficulty::Hard);
                refreshDifficultyButtonColors();
            }
            else if (backButtonSprite.getGlobalBounds().contains(clickPosition))
            {
                game.requestStateChange(StateID::Menu);
            }
        }
    }
    else if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>())
    {
        if (mouseButtonReleased->button == sf::Mouse::Button::Left)
        {
            if (draggingVolumeHandle)
            {
                game.getSettings().save(); // persist the final dragged-to volume once, not on every move
            }
            draggingVolumeHandle = false;
        }
    }
    else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>())
    {
        if (draggingVolumeHandle)
        {
            applyVolumeFromMouseX(static_cast<float>(mouseMoved->position.x));
        }
    }
}

void SettingsState::update(float /*deltaTime*/)
{
    std::ostringstream stream;
    stream << "Volume: " << static_cast<int>(game.getSettings().getVolume()) << "%";
    volumeLabel.setString(stream.str());
    const sf::FloatRect bounds = volumeLabel.getLocalBounds();
    volumeLabel.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    volumeLabel.setPosition({GameConfig::WindowWidth / 2.f, volumeTrack.getPosition().y - 40.f});
}

void SettingsState::render(sf::RenderWindow& window)
{
    window.setView(window.getDefaultView());
    window.clear(sf::Color(245, 240, 230));

    window.draw(titleText);

    window.draw(volumeLabel);
    window.draw(volumeTrack);
    window.draw(volumeHandle);

    for (int i = 0; i < 3; ++i)
    {
        window.draw(difficultyButtons[i]);
        window.draw(difficultyLabels[i]);
    }

    window.draw(backButtonSprite);
}
