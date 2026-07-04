#pragma once

#include <SFML/Graphics.hpp>
#include <cstddef>

class HUD
{
public:
    HUD(sf::Font& font);

    // HUD doesn't own any of this data — Player and EnemyManager
    // own it. HUD is just told the current values each frame and
    // displays them. It never reaches into other classes itself.
    void draw(sf::RenderWindow& window, int health, int maxHealth, std::size_t enemyCount);

private:
    sf::Text m_healthText;
    sf::Text m_enemyCountText;
};