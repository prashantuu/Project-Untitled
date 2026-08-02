#pragma once
#include <SFML/Graphics.hpp>

class LaserBeam
{
public:
    enum class HitResult { None, HitPlayer, ReachedBottom };

    LaserBeam();

    // Fires from (startX, startY) straight down. maxLength = distance the FULL beam should visually reach.
    void fire(const sf::Texture& texture, int frameCount, float frameDuration,
        float startX, float startY, float maxLength, float thicknessScale = 0.3f);

    // Call every frame while active. Returns HitPlayer/ReachedBottom the instant either happens.
    HitResult update(float dt, const sf::FloatRect& playerBounds);
    void startShrinking();

    void render(sf::RenderWindow& target) const;
    bool isFinished() const;
    sf::FloatRect getBounds() const;

private:
    enum class State { Idle, Growing, Shrinking };
    State state = State::Idle;

    const sf::Texture* texture = nullptr;
    sf::Sprite sprite;

    int frameWidth = 0, frameHeight = 0, frameCount = 0, currentFrame = 0;
    float frameDuration = 0.f, elapsed = 0.f;

    float posX = 0.f, posY = 0.f, maxLength = 0.f, beamWidth = 0.f;
};