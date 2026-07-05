#include "graphics/Camera.h"
#include <algorithm>
#include <ctime>

Camera::Camera(sf::Vector2f viewSize)
    : m_view(sf::FloatRect({0.f, 0.f}, viewSize))
    , m_baseViewSize(viewSize)
    , m_viewSize(viewSize)
    , m_worldSize(viewSize)
    , m_shakeIntensity(0.f)
    , m_shakeDuration(sf::Time::Zero)
    , m_shakeTimer(sf::Time::Zero)
    , m_randomEngine(static_cast<unsigned int>(std::time(nullptr)))
{
}

void Camera::setWorldBounds(sf::Vector2f worldSize)
{
    m_worldSize = worldSize;
}

void Camera::follow(sf::Vector2f target, sf::Time deltaTime)
{
    sf::Vector2f currentCenter = m_view.getCenter();

    const float SMOOTHING = 6.f;
    float lerpFactor = std::min(SMOOTHING * deltaTime.asSeconds(), 1.f);
    sf::Vector2f smoothedCenter = currentCenter + (target - currentCenter) * lerpFactor;

    float halfWidth = m_viewSize.x / 2.f;
    float halfHeight = m_viewSize.y / 2.f;

    float clampedX = std::clamp(smoothedCenter.x, halfWidth, m_worldSize.x - halfWidth);
    float clampedY = std::clamp(smoothedCenter.y, halfHeight, m_worldSize.y - halfHeight);

    sf::Vector2f finalCenter = {clampedX, clampedY};

    // Apply screen shake on top of the clamped position, decaying
    // linearly to zero over m_shakeDuration.
    if (m_shakeTimer < m_shakeDuration)
    {
        m_shakeTimer += deltaTime;

        float remainingFraction = 1.f - (m_shakeTimer.asSeconds() / m_shakeDuration.asSeconds());
        float currentIntensity = m_shakeIntensity * std::max(0.f, remainingFraction);

        std::uniform_real_distribution<float> dist(-currentIntensity, currentIntensity);
        finalCenter.x += dist(m_randomEngine);
        finalCenter.y += dist(m_randomEngine);
    }

    m_view.setCenter(finalCenter);
}

void Camera::apply(sf::RenderWindow& window)
{
    window.setView(m_view);
}

void Camera::shake(float intensity, sf::Time duration)
{
    m_shakeIntensity = intensity;
    m_shakeDuration = duration;
    m_shakeTimer = sf::Time::Zero;
}

void Camera::setZoom(float zoomFactor)
{
    m_viewSize = m_baseViewSize * zoomFactor;
    m_view.setSize(m_viewSize);
}