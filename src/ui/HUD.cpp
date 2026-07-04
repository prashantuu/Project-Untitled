#include "ui/HUD.h"
#include <string>

HUD::HUD(sf::Font& font)
    : m_heartIcon(m_heartTexture)
    , m_healthText(font)
    , m_enemyCountText(font)
{
    // NOTE: replace with your actual downloaded heart/health icon.
    m_heartTexture.loadFromFile("assets/sprites/ui/heart.png");

    sf::Vector2u size = m_heartTexture.getSize();
    if (size.x > 0)
    {
        // Normalize whatever icon size you downloaded down to a
        // consistent 32px HUD icon size.
        float scale = 32.f / static_cast<float>(size.x);
        m_heartIcon.setScale({scale, scale});
    }
    m_heartIcon.setPosition({20.f, 20.f});

    m_healthText.setCharacterSize(26);
    m_healthText.setFillColor(sf::Color::White);
    m_healthText.setPosition({60.f, 18.f}); // sits beside the heart icon

    m_enemyCountText.setCharacterSize(22);
    m_enemyCountText.setFillColor(sf::Color(210, 210, 210));
    m_enemyCountText.setPosition({20.f, 60.f});
}

void HUD::draw(sf::RenderWindow& window, int health, int maxHealth, std::size_t enemyCount)
{
    m_healthText.setString(std::to_string(health) + " / " + std::to_string(maxHealth));
    m_enemyCountText.setString("Enemies remaining: " + std::to_string(enemyCount));

    window.draw(m_heartIcon);
    window.draw(m_healthText);
    window.draw(m_enemyCountText);
}