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
    , m_camera({800.f, 600.f})
    , m_audioManager(m_resources)
    , m_pickupManager(m_resources, m_worldSize)
    , m_state(GameState::MainMenu)
    , m_wasTriggerHeldLastFrame(false)
    , m_score(0)
    , m_font(m_resources.getFont("assets/fonts/game_font.ttf"))
    , m_gameOverText(m_font)
    , m_hud(m_resources)
    , m_mainMenu(m_resources)
{
    m_window.setFramerateLimit(60);

    m_camera.setWorldBounds(m_worldSize);
    m_player.setPosition(m_worldSize / 2.f);

    m_gameOverText.setString("GAME OVER");
    m_gameOverText.setCharacterSize(60);
    m_gameOverText.setFillColor(sf::Color(220, 60, 60));

    sf::FloatRect bounds = m_gameOverText.getLocalBounds();
    m_gameOverText.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    m_audioManager.playMusic();
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

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (m_state == GameState::MainMenu)
            {
                if (keyPressed->code == sf::Keyboard::Key::W || keyPressed->code == sf::Keyboard::Key::Up)
                {
                    m_mainMenu.moveSelectionUp();
                }
                else if (keyPressed->code == sf::Keyboard::Key::S || keyPressed->code == sf::Keyboard::Key::Down)
                {
                    m_mainMenu.moveSelectionDown();
                }
                else if (keyPressed->code == sf::Keyboard::Key::Enter)
                {
                    if (m_mainMenu.getSelectedOption() == MenuOption::StartGame)
                    {
                        m_state = GameState::Playing;
                    }
                    else
                    {
                        m_window.close();
                    }
                }
            }
            else if (m_state == GameState::Playing && keyPressed->code == sf::Keyboard::Key::R)
            {
                m_player.reload();
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

    // Semi-auto weapons (Pistol, Shotgun) fire only on the frame the
    // trigger transitions from not-held to held. Automatic weapons
    // (Rifle, SMG) fire every frame it's held, naturally rate-limited
    // by Weapon::shoot()'s own fire-rate check.
    bool triggerHeld = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    bool justPressed = triggerHeld && !m_wasTriggerHeldLastFrame;

    if (triggerHeld && (m_player.isWeaponAutomatic() || justPressed))
    {
        if (m_player.shoot())
        {
            m_audioManager.playGunshot();
        }
    }

    m_wasTriggerHeldLastFrame = triggerHeld;

    m_camera.follow(m_player.getPosition(), deltaTime);

    m_enemyManager.update(deltaTime, m_player.getPosition());
    m_waveManager.update(deltaTime);

    std::size_t enemiesDestroyed = m_enemyManager.checkCollisions(m_player);
    for (std::size_t i = 0; i < enemiesDestroyed; ++i)
    {
        m_audioManager.playEnemyDeath();
    }

    if (m_enemyManager.checkPlayerCollision(m_player))
    {
        m_audioManager.playPlayerHit();
    }

    if (m_enemyManager.checkEnemyBulletHits(m_player))
    {
        m_audioManager.playPlayerHit();
    }

    m_pickupManager.update(deltaTime);
    PickupCollectionReport pickupReport = m_pickupManager.checkCollisions(m_player);

    if (pickupReport.coinsCollected > 0)
    {
        m_score += pickupReport.coinsCollected;
    }
    // Health/ammo pickups already applied themselves via player.heal()/
    // addAmmo() inside PickupManager::checkCollisions() — Game only
    // reacts to the parts it actually owns (score, sound).
    if (pickupReport.healthCollected || pickupReport.ammoCollected)
    {
        m_audioManager.playGunshot(); // TODO: replace with a dedicated playPickup() once you add that SFX
    }

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

    if (m_state == GameState::Playing)
    {
        m_camera.apply(m_window);

        m_tileMap.draw(m_window);
        m_pickupManager.draw(m_window);
        m_enemyManager.draw(m_window);
        m_player.draw(m_window);

        m_window.setView(m_window.getDefaultView());
        m_hud.draw(m_window, m_player.getHealth(), m_player.getMaxHealth(),
            m_enemyManager.getEnemyCount(), m_player.getAmmo(), m_player.getMagazineSize(),
            m_player.isReloading());
    }
    else if (m_state == GameState::MainMenu)
    {
        m_window.setView(m_window.getDefaultView());
        m_mainMenu.draw(m_window);
    }
    else // GameState::GameOver
    {
        m_window.setView(m_window.getDefaultView());

        m_gameOverText.setPosition(
            { m_window.getSize().x / 2.f, m_window.getSize().y / 2.f });

        m_window.draw(m_gameOverText);
    }

    m_window.display();
}