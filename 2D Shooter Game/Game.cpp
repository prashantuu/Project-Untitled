#include "Game.h"

Game::Game()
    : window(sf::VideoMode(1280, 720), "2D Shooter",Style::Default)
{
    window.setFramerateLimit(60);
    enemies.emplace_back(sf::Vector2f(600.f, 200.f));
    enemies.emplace_back(sf::Vector2f(900.f, 500.f));
    pickups.emplace_back(
        sf::Vector2f(400.f, 200.f),
        PickupType::Health
    );

    pickups.emplace_back(
        sf::Vector2f(700.f, 400.f),
        PickupType::Ammo
    );
}

void Game::run()
{
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();//how much time each frame took i.e slower pc higher time taken//

        processEvents();

        update(deltaTime);

        render();
    }
}

void Game::processEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        // SHOOTING
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f playerPos = player.getPosition();
                sf::Vector2f mousePos = window.mapPixelToCoords(
                    sf::Mouse::getPosition(window)
                );

                if (player.getAmmo() > 0)
                {
                    bullets.emplace_back(playerPos, mousePos);

                    player.useAmmo(1);
                }
            }
        }
    }
}

void Game::update(float deltaTime)
{
    player.update(deltaTime);

    for (auto& bullet : bullets)
    {
        bullet.update(deltaTime);
    }
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); )
    {
        bool bulletRemoved = false;

        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); )
        {
            if (bulletIt->getBounds().intersects(enemyIt->getBounds()))
            {
                enemyIt->takeDamage(1);

                bulletIt = bullets.erase(bulletIt);
                bulletRemoved = true;

                if (enemyIt->isDead())
                {
                    enemyIt = enemies.erase(enemyIt);
                }
                else
                {
                    ++enemyIt;
                }

                break;
            }
            else
            {
                ++enemyIt;
            }
        }
        if (!bulletRemoved)
            ++bulletIt;
    }
    for (auto& enemy : enemies)
    {
        enemy.update(deltaTime, player.getPosition());
        if (enemy.getBounds().intersects(player.getBounds()))
        {
            player.takeDamage(1);
        }
    }
    for (auto pickupIt = pickups.begin();
        pickupIt != pickups.end(); )
    {
        if (pickupIt->getBounds().intersects(
            player.getBounds()))
        {
            if (pickupIt->getType()
                == PickupType::Health)
            {
                player.addHealth(25);
            }

            if (pickupIt->getType()
                == PickupType::Ammo)
            {
                player.addAmmo(15);
            }

            pickupIt = pickups.erase(pickupIt);
        }
        else
        {
            ++pickupIt;
        }
    }
}

void Game::render()
{
    window.clear();

    player.render(window);

    for (auto& bullet : bullets)
    {
        bullet.render(window);
    }
    for (auto& enemy : enemies)
    {
        enemy.render(window);
    }
    for (auto& pickup : pickups)
    {
        pickup.render(window);
    }
    window.display();
    //don't draw anything after window.display()
}