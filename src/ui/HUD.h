#pragma once

#include <SFML/Graphics.hpp>
#include <cstddef>
#include "resource/ResourceManager.h"

class HUD
{
public:
    HUD(ResourceManager& resources);

    void draw(sf::RenderWindow& window, int health, int maxHealth, std::size_t enemyCount);

private:
    sf::Texture& m_heartTexture;
    sf::Sprite m_heartIcon;

    sf::Text m_healthText;
    sf::Text m_enemyCountText;
};