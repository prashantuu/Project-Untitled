#pragma once
#include <SFML/Graphics.hpp>

class Animation
{
public:
    Animation();
    Animation(const sf::Texture& texture, int frameCount, float frameDuration,
        float x, float y, float scaleX = 1.f, float scaleY = 1.f, bool loop = false);

    void setPosition(float x, float y);
    void setScale(float sx, float sy);

    void update(float dt);
    void render(sf::RenderWindow& target) const;

    bool isFinished() const;

private:
    const sf::Texture* texture = nullptr;
    sf::Sprite sprite;

    int frameWidth = 0;
    int frameHeight = 0;
    int frameCount = 0;
    int currentFrame = 0;

    float frameDuration = 0.f;
    float elapsed = 0.f;

    bool loop = false;
    bool finished = false;
};