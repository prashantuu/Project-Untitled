#include "graphics/Animation.h"

Animation::Animation(sf::Vector2i frameSize, int frameCount, int row,
    sf::Time frameDuration, bool loop)
    : m_frameSize(frameSize)
    , m_frameCount(frameCount)
    , m_row(row)
    , m_frameDuration(frameDuration)
    , m_loop(loop)
    , m_currentFrame(0)
    , m_elapsedTime(sf::Time::Zero)
    , m_finished(false)
{
}

void Animation::update(sf::Time deltaTime)
{
    if (m_finished)
        return;

    m_elapsedTime += deltaTime;

    // A while-loop (not if) in case deltaTime is large enough to
    // skip more than one frame in a single update (e.g. a lag spike).
    while (m_elapsedTime >= m_frameDuration)
    {
        m_elapsedTime -= m_frameDuration;
        m_currentFrame++;

        if (m_currentFrame >= m_frameCount)
        {
            if (m_loop)
            {
                m_currentFrame = 0;
            }
            else
            {
                m_currentFrame = m_frameCount - 1;
                m_finished = true;
                break;
            }
        }
    }
}

void Animation::reset()
{
    m_currentFrame = 0;
    m_elapsedTime = sf::Time::Zero;
    m_finished = false;
}

sf::IntRect Animation::getCurrentFrameRect() const
{
    return sf::IntRect(
        { m_currentFrame * m_frameSize.x, m_row * m_frameSize.y },
        { m_frameSize.x, m_frameSize.y });
}

bool Animation::isFinished() const
{
    return m_finished;
}