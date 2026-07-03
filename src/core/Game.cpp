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
    }
}

void Game::update(sf::Time deltaTime)
{
    // Game still doesn't know HOW the player moves or rotates.
    // It just passes along what Player needs: time and window (for mouse).
    m_player.update(deltaTime, m_window);
}

void Game::render()
{
    m_window.clear(sf::Color::Black);
    m_player.draw(m_window);
    m_window.display();
}