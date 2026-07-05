#include "core/Game.h"

namespace
{
    std::vector<std::string> createLevelLayout()
    {
        return {
            "##############################",
            "#............................#",
            "#............................#",
            "#............................#",
            "#............................#",
            "#....######...#..............#",
            "#.............#..............#",
            "#.............#..............#",
            "#.............#.....######...#",
            "#.............#..............#",
            "#.............#..............#",
            "#............................#",
            "#.......#....................#",
            "#.......#....................#",
            "#.......#....................#",
            "#.......#....................#",
            "#.......#.........######.....#",
            "#.......#....................#",
            "#............................#",
            "#............................#",
            "#............................#",
            "#............................#",
            "#............................#",
            "##############################",
        };
    }
}

Game::Game()
    : m_window(sf::VideoMode({800, 600}), "TopDownShooter")
    , m_tileMap(createLevelLayout(), 80.f, m_resources)
    , m_worldSize(m_tileMap.getWorldSize())
    , m_player(m_resources)
    , m_enemyManager(m_worldSize, m_resources)
    , m_waveManager(m_enemyManager)
    , m_pickupManager(m_worldSize, m_resources)
    , m_camera({800.f, 600.f})
    , m_particleSystem()
    , m_audioManager(m_resources)
    , m_state(GameState::MainMenu)
    , m_wasTriggerHeldLastFrame(false)
    , m_font(m_resources.getFont("assets/fonts/game_font.ttf"))
    , m_gameOverText(m_font)
    , m_hud(m_resources)
    , m_mainMenu(m_font, {"Play", "Settings", "Exit"}, {400.f, 300.f})
    , m_pauseMenu(m_font, {"Resume", "Restart", "Quit"}, {400.f, 300.f})
{
    m_window.setFramerateLimit(60);

    m_camera.setWorldBounds(m_worldSize);
    m_player.setPosition(m_worldSize / 2.f);

    m_gameOverText.setString("GAME OVER");
    m_gameOverText.setCharacterSize(60);
    m_gameOverText.setFillColor(sf::Color(220, 60, 60));

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

void Game::restartGame()
{
    m_player.reset(m_worldSize / 2.f);
    m_enemyManager.reset();
    m_waveManager.reset();
    m_pickupManager.reset();
    m_state = GameState::Playing;
    m_audioManager.playMusic();
}

void Game::processEvents()
{
    while (const std::optional<sf::Event> event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            switch (m_state)
            {
                case GameState::MainMenu:
                    if (keyPressed->code == sf::Keyboard::Key::Up)
                        m_mainMenu.moveUp();
                    else if (keyPressed->code == sf::Keyboard::Key::Down)
                        m_mainMenu.moveDown();
                    else if (keyPressed->code == sf::Keyboard::Key::Enter)
                    {
                        int selected = m_mainMenu.getSelectedIndex();
                        if (selected == 0) // Play
                        {
                            m_state = GameState::Playing;
                            m_audioManager.playMusic();
                        }
                        else if (selected == 1) // Settings — placeholder, nothing configurable yet
                        {
                            // Intentionally left as a no-op for now.
                        }
                        else if (selected == 2) // Exit
                        {
                            m_window.close();
                        }
                    }
                    break;

                case GameState::Playing:
                    if (keyPressed->code == sf::Keyboard::Key::R)
                        m_player.reload();
                    else if (keyPressed->code == sf::Keyboard::Key::Escape)
                        m_state = GameState::Paused;
                    else if (keyPressed->code == sf::Keyboard::Key::F5)
                        SaveManager::save(m_player, m_waveManager);
                    else if (keyPressed->code == sf::Keyboard::Key::F9)
                        SaveManager::load(m_player, m_waveManager);
                    break;

                case GameState::Paused:
                    if (keyPressed->code == sf::Keyboard::Key::Up)
                        m_pauseMenu.moveUp();
                    else if (keyPressed->code == sf::Keyboard::Key::Down)
                        m_pauseMenu.moveDown();
                    else if (keyPressed->code == sf::Keyboard::Key::Escape)
                        m_state = GameState::Playing; // Esc also resumes
                    else if (keyPressed->code == sf::Keyboard::Key::Enter)
                    {
                        int selected = m_pauseMenu.getSelectedIndex();
                        if (selected == 0)      m_state = GameState::Playing;  // Resume
                        else if (selected == 1) restartGame();                 // Restart
                        else if (selected == 2) m_window.close();              // Quit
                    }
                    break;

                case GameState::GameOver:
                    if (keyPressed->code == sf::Keyboard::Key::Enter)
                        m_state = GameState::MainMenu;
                    break;
            }
        }
    }
}

void Game::update(sf::Time deltaTime)
{
    m_audioManager.update();

    if (m_state != GameState::Playing)
        return;

    m_player.update(deltaTime, m_window);
    m_player.constrainToWorld(m_worldSize);

    bool triggerHeld = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    bool justPressed = triggerHeld && !m_wasTriggerHeldLastFrame;

    if (triggerHeld && (m_player.isWeaponAutomatic() || justPressed))
    {
        if (m_player.shoot())
        {
            m_audioManager.playGunshot();
            m_particleSystem.emit(ParticleType::MuzzleFlash, m_player.getPosition(), 4);
        }
    }
    m_wasTriggerHeldLastFrame = triggerHeld;

    m_camera.follow(m_player.getPosition(), deltaTime);

    m_enemyManager.update(deltaTime, m_player.getPosition());
    m_waveManager.update(deltaTime);

    m_pickupManager.update(deltaTime);
    m_pickupManager.checkCollisions(m_player);

    std::vector<sf::Vector2f> deathPositions = m_enemyManager.checkCollisions(m_player);
    for (const auto& position : deathPositions)
    {
        m_audioManager.playEnemyDeath();
        m_particleSystem.emit(ParticleType::Blood, position, 10);
    }

    m_particleSystem.update(deltaTime);

    if (m_enemyManager.checkPlayerCollision(m_player))
    {
        m_audioManager.playPlayerHit();
        m_camera.shake(6.f, sf::seconds(0.2f));
    }

    if (m_enemyManager.checkEnemyBulletHits(m_player))
    {
        m_audioManager.playPlayerHit();
        m_camera.shake(6.f, sf::seconds(0.2f));
    }

    // Slight zoom-out during boss fights for a bit more drama
    // and to give the player more visibility of the arena.
    m_camera.setZoom(m_enemyManager.hasBoss() ? 1.2f : 1.f);

    if (!m_player.isAlive())
    {
        m_state = GameState::GameOver;
        m_audioManager.playGameOver();
        m_audioManager.stopMusic();
    }
}

void Game::render()
{
    m_window.clear(sf::Color(15, 15, 20));

    switch (m_state)
    {
        case GameState::MainMenu:
        {
            m_window.setView(m_window.getDefaultView());
            m_mainMenu.draw(m_window);
            break;
        }

        case GameState::Playing:
        {
            m_camera.apply(m_window);

            m_tileMap.draw(m_window);
            m_pickupManager.draw(m_window);
            m_enemyManager.draw(m_window);
            m_player.draw(m_window);
            m_particleSystem.draw(m_window);

            m_window.setView(m_window.getDefaultView());
            m_hud.draw(m_window, m_player.getHealth(), m_player.getMaxHealth(),
                m_enemyManager.getEnemyCount(), m_player.getAmmo(), m_player.getMagazineSize(),
                m_player.isReloading(), m_player.getScore());
            break;
        }

        case GameState::Paused:
        {
            // Draw the frozen world underneath, then the pause menu
            // on top of it — world stays visible but static.
            m_camera.apply(m_window);
            m_tileMap.draw(m_window);
            m_pickupManager.draw(m_window);
            m_enemyManager.draw(m_window);
            m_player.draw(m_window);

            m_window.setView(m_window.getDefaultView());

            sf::RectangleShape overlay({800.f, 600.f});
            overlay.setFillColor(sf::Color(0, 0, 0, 150));
            m_window.draw(overlay);

            m_pauseMenu.draw(m_window);
            break;
        }

        case GameState::GameOver:
        {
            m_window.setView(m_window.getDefaultView());

            m_gameOverText.setPosition(
                { m_window.getSize().x / 2.f, m_window.getSize().y / 2.f });
            m_window.draw(m_gameOverText);
            break;
        }
    }

    m_window.display();
}