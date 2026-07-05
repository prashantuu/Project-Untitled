#include "ui/HUD.h"
#include <string>

HUD::HUD(ResourceManager& resources)
    : m_heartTexture(resources.getTexture("assets/sprites/ui/heart.png"))
    , m_heartIcon(m_heartTexture)
    , m_healthText(resources.getFont("assets/fonts/game_font.ttf"))
    , m_enemyCountText(resources.getFont("assets/fonts/game_font.ttf"))
    , m_ammoText(resources.getFont("assets/fonts/game_font.ttf"))
{
    sf::Vector2u size = m_heartTexture.getSize();
    if (size.x > 0)
    {
        float scale = 32.f / static_cast<float>(size.x);
        m_heartIcon.setScale({scale, scale});
    }
    m_heartIcon.setPosition({20.f, 20.f});

    m_healthText.setCharacterSize(26);
    m_healthText.setFillColor(sf::Color::White);
    m_healthText.setPosition({60.f, 18.f});

    m_enemyCountText.setCharacterSize(22);
    m_enemyCountText.setFillColor(sf::Color(210, 210, 210));
    m_enemyCountText.setPosition({20.f, 60.f});

    m_ammoText.setCharacterSize(22);
    m_ammoText.setFillColor(sf::Color(230, 230, 150));
    m_ammoText.setPosition({20.f, 90.f});
}

void HUD::draw(sf::RenderWindow& window, int health, int maxHealth, std::size_t enemyCount,
    int currentAmmo, int magazineSize, bool isReloading)
{
    m_healthText.setString(std::to_string(health) + " / " + std::to_string(maxHealth));
    m_enemyCountText.setString("Enemies remaining: " + std::to_string(enemyCount));

    if (isReloading)
    {
        m_ammoText.setString("Reloading...");
    }
    else
    {
        m_ammoText.setString("Ammo: " + std::to_string(currentAmmo) + " / " + std::to_string(magazineSize));
    }

    window.draw(m_heartIcon);
    window.draw(m_healthText);
    window.draw(m_enemyCountText);
    window.draw(m_ammoText);
}