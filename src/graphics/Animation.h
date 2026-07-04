#pragma once

#include <SFML/Graphics.hpp>

// Represents ONE animation sequence on a sprite sheet — a fixed
// set of equally-sized frames on a given row, played back at a
// fixed rate, either looping or playing once.
//
// Animation does not know about Player, Enemy, or anything else.
// It only knows about frames and timing. Entities that use it own
// one Animation per state (idle, walk, etc.) and decide which one
// is "current" themselves — that decision belongs to the entity,
// not to this class.
class Animation
{
public:
    // frameSize: pixel size of a single frame in the sheet.
    // frameCount: how many frames this animation has.
    // row: which row of the sheet this animation lives on
    //      (many sheets put idle on row 0, walk on row 1, etc).
    // frameDuration: how long each frame is shown.
    // loop: whether it repeats or stops on the last frame.
    Animation(sf::Vector2i frameSize, int frameCount, int row,
        sf::Time frameDuration, bool loop = true);

    // Advances the animation's internal timer. Call once per frame
    // while this animation is the active one for its owner.
    void update(sf::Time deltaTime);

    // Call this whenever an entity switches INTO this animation,
    // so it starts from frame 0 instead of resuming wherever it
    // left off last time it was active.
    void reset();

    // The texture rect for whichever frame is currently showing.
    // Apply via sprite.setTextureRect(animation.getCurrentFrameRect()).
    sf::IntRect getCurrentFrameRect() const;

    // Only meaningful for non-looping animations.
    bool isFinished() const;

private:
    sf::Vector2i m_frameSize;
    int m_frameCount;
    int m_row;
    sf::Time m_frameDuration;
    bool m_loop;

    int m_currentFrame;
    sf::Time m_elapsedTime;
    bool m_finished;
};