#include "Animation.h"

Animation::Animation()
{
}

Animation::Animation(const sf::Texture& tex, int frameCount, float frameDuration,
    float x, float y, float scaleX, float scaleY, bool loop)
    : texture(&tex)
    , frameCount(frameCount)
    , frameDuration(frameDuration)
    , loop(loop)
{
    frameWidth = texture->getSize().x / frameCount;
    frameHeight = texture->getSize().y;

    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setOrigin(frameWidth / 2.f, frameHeight / 2.f);

    setPosition(x, y);
    setScale(scaleX, scaleY);
}

void Animation::setPosition(float x, float y) { sprite.setPosition(x, y); }
void Animation::setScale(float sx, float sy) { sprite.setScale(sx, sy); }

void Animation::update(float dt)
{
    if (finished) return;

    elapsed += dt;
    if (elapsed >= frameDuration)
    {
        elapsed -= frameDuration;
        currentFrame++;

        if (currentFrame >= frameCount)
        {
            if (loop)
            {
                currentFrame = 0;
            }
            else
            {
                currentFrame = frameCount - 1;
                finished = true;
                return;
            }
        }

        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
    }
}

void Animation::render(sf::RenderWindow& target) const { target.draw(sprite); }
bool Animation::isFinished() const { return finished; }