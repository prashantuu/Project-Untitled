#pragma once
#include "Animation.h"
#include <vector>
#include <algorithm>
#include <SFML/System.hpp>

class AnimationManager
{
public:
    // Call this whenever you want to trigger an animation (explosion, muzzle flash, etc.)
    void spawn(const sf::Texture& texture, float x, float y,
        int frameCount, float frameDuration,
        float scaleX = 1.f, float scaleY = 1.f, bool loop = false)
    {
        animations.emplace_back(texture, frameCount, frameDuration, x, y, scaleX, scaleY, loop);
    }

    void updateAll()
    {
        float dt = m_clock.restart().asSeconds();

        for (auto& anim : animations)
            anim.update(dt);

        animations.erase(
            std::remove_if(animations.begin(), animations.end(),
                [](const Animation& a) { return a.isFinished(); }),
            animations.end());
    }

    void renderAll(sf::RenderWindow& window) const
    {
        for (auto& anim : animations)
            anim.render(window);
    }

private:
    std::vector<Animation> animations;
    sf::Clock m_clock;
};