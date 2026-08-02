#include "LaserBeam.h"

LaserBeam::LaserBeam() {}

void LaserBeam::fire(const sf::Texture& tex, int fc, float fd,
    float x, float y, float maxLen, float thicknessScale)
{
    texture = &tex;
    frameCount = fc;
    frameDuration = fd;
    posX = x; posY = y;
    maxLength = maxLen;
    currentFrame = 0;
    elapsed = 0.f;
    state = State::Growing;

    frameWidth = texture->getSize().x / frameCount;
    frameHeight = texture->getSize().y;

    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setOrigin(frameWidth / 2.f, 0.f);   // pinned at top (cannon point)
    sprite.setPosition(posX, posY);

    // Scale so the LAST frame (full length in the sheet) reaches exactly maxLength
    float scaleY = maxLength / static_cast<float>(frameHeight);
    sprite.setScale(thicknessScale, scaleY);

    beamWidth = frameWidth * thicknessScale;
}

LaserBeam::HitResult LaserBeam::update(float dt, const sf::FloatRect& playerBounds)
{
    if (state == State::Idle)
        return HitResult::None;

    elapsed += dt;
    if (elapsed >= frameDuration)
    {
        elapsed -= frameDuration;

        if (state == State::Growing)
        {
            currentFrame++;
            if (currentFrame >= frameCount - 1)
            {
                currentFrame = frameCount - 1;
                sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
                return HitResult::ReachedBottom;   // fully extended
            }
            sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
        }
        else if (state == State::Shrinking)
        {
            currentFrame--;
            if (currentFrame < 0)
            {
                currentFrame = 0;
                state = State::Idle;
            }
            else
            {
                sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
            }
        }
    }

    // Check collision every frame (not just on frame-swap ticks) so a fast-moving player
    // can't slip through between animation-frame updates
    if (state == State::Growing && getBounds().intersects(playerBounds))
        return HitResult::HitPlayer;

    return HitResult::None;
}

void LaserBeam::startShrinking() { state = State::Shrinking; }

void LaserBeam::render(sf::RenderWindow& target) const
{
    if (state != State::Idle)
        target.draw(sprite);
}

bool LaserBeam::isFinished() const { return state == State::Idle; }

sf::FloatRect LaserBeam::getBounds() const
{
    float currentLength = maxLength * (static_cast<float>(currentFrame + 1) / frameCount);
    return sf::FloatRect(posX - beamWidth / 2.f, posY, beamWidth, currentLength);
}