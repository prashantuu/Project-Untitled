#include "core/Game.h"

const float Game::PLAYER_SPEED = 200.f;

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "TopDownShooter")
    , m_player(20.f)
{
    m_window.setFramerateLimit(60);

    m_player.setFillColor(sf::Color::Green);
    m_player.setPosition({400.f, 300.f});
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
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        movement.y -= PLAYER_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        movement.y += PLAYER_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        movement.x -= PLAYER_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        movement.x += PLAYER_SPEED;

    m_player.move(movement * deltaTime.asSeconds());
}

void Game::render()
{
    m_window.clear(sf::Color::Black);
    m_window.draw(m_player);
    m_window.display();
}