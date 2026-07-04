#include "core/Game.h"

namespace
{
    std::vector<std::string> createLevelLayout()
    {
        return {
            "#########################",
            "#.......................#",
            "#.......................#",
            "#.......................#",
            "#.......................#",
            "#....#####..#...........#",
            "#...........#...........#",
            "#...........#...........#",
            "#...........#...........#",
            "#...........#...........#",
            "#......#................#",
            "#......#................#",
            "#......#................#",
            "#......#................#",
            "#......#.......#####....#",
            "#.......................#",
            "#.......................#",
            "#.......................#",
            "#.......................#",
            "#########################",
        };
    }
}

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "TopDownShooter")
    , m_tileMap(createLevelLayout(), 80.f)
    , m_worldSize(m_tileMap.getWorldSize())
    , m_enemyManager(m_worldSize)
    , m_camera({800.f, 600.f})
    , m_gameOver(false)
    , m_gameOverText(m_font)
{
    m_window.setFramerateLimit(60);

    // World size now comes from the actual map, not a hardcoded
    // constant — one source of truth for both camera and player.
    m_camera.setWorldBounds(m_worldSize);

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
    m_player.constrainToWorld(m_worldSize);

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
        m_camera.apply(m_window);

        m_tileMap.draw(m_window);
        m_enemyManager.draw(m_window);
        m_player.draw(m_window);
    }
    else
    {
        m_window.setView(m_window.getDefaultView());

        m_gameOverText.setPosition(
            { m_window.getSize().x / 2.f, m_window.getSize().y / 2.f });

        m_window.draw(m_gameOverText);
    }

    m_window.display();
}