#include "core/Game.h"

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "TopDownShooter")
{
    m_window.setFramerateLimit(60);
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

    m_enemyManager.update(deltaTime, m_player.getPosition());
    m_enemyManager.checkCollisions(m_player);
    m_enemyManager.checkPlayerCollision(m_player);
}

void Game::render()
{
    m_window.clear(sf::Color::Black);

    m_enemyManager.draw(m_window);
    m_player.draw(m_window);

    m_window.display();
}