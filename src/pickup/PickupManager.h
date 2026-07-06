#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "pickup/Pickup.h"
#include "player/Player.h"
#include "resources/ResourceManager.h"

class PickupManager
{
public:
    PickupManager(sf::Vector2f worldSize, ResourceManager& resources);

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);
    void checkCollisions(Player& player);

    void reset(); // used by Milestone 28's restart

private:
    sf::Vector2f getRandomPosition();
    void spawnRandomPickup();

private:
    sf::Texture& m_healthTexture;
    sf::Texture& m_ammoTexture;
    sf::Texture& m_coinTexture;

    std::vector<Pickup> m_pickups;
    sf::Vector2f m_worldSize;

    sf::Time m_spawnTimer;
    static const sf::Time SPAWN_INTERVAL;
    static const std::size_t MAX_PICKUPS;

    std::mt19937 m_randomEngine;
};