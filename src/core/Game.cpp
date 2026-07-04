#include "core/Game.h"

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "TopDownShooter")
    , m_camera({800.f, 600.f})
    , m_gameOver(false)
    , m_gameOverText(m_font)
{
    m_window.setFramerateLimit(60);

    m_font.openFromFile("assets/fonts/arial.ttf");

    m_gameOverText.setString("GAME OVER");
    m_gameOverText.setCharacterSize(60);
    m_gameOverText.setFillColor(sf::Color::White);

    sf::FloatRect bounds = m_gameOverText.getLocalBounds();
    m_gameOverText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
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
    m_camera.follow(m_player.getPosition());

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
        // Everything drawn after this uses the camera's view —
        // world positions, not fixed screen positions.
        m_camera.apply(m_window);

        m_enemyManager.draw(m_window);
        m_player.draw(m_window);
    }
    else
    {
        // Switch back to the window's default view so the
        // Game Over text is fixed on screen, not affected by
        // wherever the camera happened to be looking.
        m_window.setView(m_window.getDefaultView());

        m_gameOverText.setPosition(
            { m_window.getSize().x / 2.f, m_window.getSize().y / 2.f });

        m_window.draw(m_gameOverText);
    }

    m_window.display();
}