#include "world/Camera.hpp"
#include "core/Constants.hpp"

Camera::Camera(float screenWidth_, float screenHeight_)
    : screenWidth(screenWidth_)
    , screenHeight(screenHeight_)
    , view(sf::FloatRect({0.f, 0.f}, {screenWidth_, screenHeight_}))
{
    view.setCenter({screenWidth / 2.f, screenHeight / 2.f});
}

void Camera::reset()
{
    view.setSize({screenWidth, screenHeight});
    view.setCenter({screenWidth / 2.f, screenHeight / 2.f});
}

float Camera::update(const Player& player)
{
    const float visibleTopY = view.getCenter().y - screenHeight / 2.f;
    const float halfwayY = visibleTopY + screenHeight * GameConfig::CameraFollowThresholdRatio;

    const float playerTopY = player.getBounds().position.y;

    if (playerTopY < halfwayY)
    {
        const float desiredCenterY = playerTopY + screenHeight / 2.f - screenHeight * GameConfig::CameraFollowThresholdRatio;

        if (desiredCenterY < view.getCenter().y)
        {
            const float climbed = view.getCenter().y - desiredCenterY;
            view.setCenter({view.getCenter().x, desiredCenterY});
            return climbed;
        }
    }

    return 0.f;
}

const sf::View& Camera::getView() const
{
    return view;
}

bool Camera::hasReachedBottomEdge(const Player& player) const
{
    const sf::FloatRect bounds = player.getBounds();
    const float feetY = bounds.position.y + bounds.size.y;
    const float visibleBottomY = view.getCenter().y + screenHeight / 2.f;
    return feetY >= visibleBottomY;
}
