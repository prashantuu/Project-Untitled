#include "core/Game.h"
#include <algorithm>

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "TopDownShooter")
{
    m_window.setFramerateLimit(60);

    // A couple of enemies to test collision against.
    m_enemies.emplace_back(sf::Vector2f{600.f, 200.f});
    m_enemies.emplace_back(sf::Vector2f{600.f, 400.f});
}

void Game::run()
{
    sf::Clock clock;

    while (m_window.isOpen())
    {
        sf::Time deltaTime = clock.restart();

        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents()
{
    while (const std::optional<sf::Event> event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }

        if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButton->button == sf::Mouse::Button::Left)
            {
                m_player.shoot();
            }
        }
    }
}

void Game::update(sf::Time deltaTime)
{
    m_player.update(deltaTime, m_window);

    for (auto& enemy : m_enemies)
        enemy.update(deltaTime);

    // Ask the player "did any of your bullets hit this enemy?"
    // Game doesn't know how that check works internally —
    // it just removes whichever enemies got hit.
    m_enemies.erase(
        std::remove_if(m_enemies.begin(), m_enemies.end(),
            [this](Enemy& enemy) { return m_player.checkHit(enemy.getBounds()); }),
        m_enemies.end());
}

void Game::render()
{
    m_window.clear(sf::Color::Black);

    for (auto& enemy : m_enemies)
        enemy.draw(m_window);

    m_player.draw(m_window);

    m_window.display();
}