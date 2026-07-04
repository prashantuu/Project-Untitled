#include "core/Game.h"

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "TopDownShooter")
    , m_gameOver(false)
    , m_gameOverText(m_font)
{
    m_window.setFramerateLimit(60);

    // NOTE: point this at an actual .ttf file in your project,
    // e.g. assets/fonts/arial.ttf. If it fails to load, the text
    // just won't render — the rest of the game still works fine.
    m_font.openFromFile("assets/fonts/arial.ttf");

    m_gameOverText.setString("GAME OVER");
    m_gameOverText.setCharacterSize(60);
    m_gameOverText.setFillColor(sf::Color::White);

    // Center the text's origin so setPosition() centers it on screen,
    // not its top-left corner.
    sf::FloatRect bounds = m_gameOverText.getLocalBounds();
    m_gameOverText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    m_gameOverText.setPosition({400.f, 300.f});
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
            if (!m_gameOver && mouseButton->button == sf::Mouse::Button::Left)
            {
                m_player.shoot();
            }
        }
    }
}

void Game::update(sf::Time deltaTime)
{
    if (m_gameOver)
        return;

    m_player.update(deltaTime, m_window);

    m_enemyManager.update(deltaTime, m_player.getPosition());
    m_enemyManager.checkCollisions(m_player);
    m_enemyManager.checkPlayerCollision(m_player);

    if (!m_player.isAlive())
    {
        m_gameOver = true;
    }
}

void Game::render()
{
    m_window.clear(sf::Color::Black);

    if (!m_gameOver)
    {
        m_enemyManager.draw(m_window);
        m_player.draw(m_window);
    }
    else
    {
        m_window.draw(m_gameOverText);
    }

    m_window.display();
}