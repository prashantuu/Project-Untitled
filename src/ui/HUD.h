#pragma once

#include <SFML/Graphics.hpp>
#include <cstddef>

class HUD
{
public:
    HUD(sf::Font& font);

    void draw(sf::RenderWindow& window, int health, int maxHealth, std::size_t enemyCount);

private:
    sf::Texture m_heartTexture;
    sf::Sprite m_heartIcon;

    sf::Text m_healthText;
    sf::Text m_enemyCountText;
};