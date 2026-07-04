#include "ui/HUD.h"
#include <string>

HUD::HUD(sf::Font& font)
    : m_healthText(font)
    , m_enemyCountText(font)
{
    m_healthText.setCharacterSize(24);
    m_healthText.setFillColor(sf::Color::White);
    m_healthText.setPosition({20.f, 20.f});

    m_enemyCountText.setCharacterSize(24);
    m_enemyCountText.setFillColor(sf::Color::White);
    m_enemyCountText.setPosition({20.f, 50.f});
}

void HUD::draw(sf::RenderWindow& window, int health, int maxHealth, std::size_t enemyCount)
{
    m_healthText.setString("Health: " + std::to_string(health) + " / " + std::to_string(maxHealth));
    m_enemyCountText.setString("Enemies: " + std::to_string(enemyCount));

    window.draw(m_healthText);
    window.draw(m_enemyCountText);

    // Ammo will go here once weapons have limited ammo — for now
    // there's nothing to display, so it's intentionally left out
    // rather than showing a fake placeholder number.
}