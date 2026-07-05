#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include "Pickup.h"

class ResourceManager;
class Player;

// PickupManager reports FACTS about what was collected this frame -
// it never decides what a "score" is or plays a sound itself. That
// decision belongs to Game, same pattern as EnemyManager reporting
// kills/hits without knowing about audio.
struct PickupCollectionReport
{
    int  coinsCollected   = 0;
    bool healthCollected  = false;
    bool ammoCollected    = false;
};

class PickupManager
{
public:
    PickupManager(ResourceManager& resources, sf::Vector2f worldSize);

    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);

    void spawnPickup(PickupType type, sf::Vector2f position);
    void spawnRandomPickup();

    // Checks pickups against the player, removes any that were collected,
    // and applies Health/Ammo directly through Player's own interface
    // (player.heal / player.addAmmo) - PickupManager never touches
    // Player's internals, only calls the methods Player already exposes.
    PickupCollectionReport checkCollisions(Player& player);

private:
    std::vector<Pickup> m_pickups;
    sf::Vector2f         m_worldSize;

    sf::Texture& m_healthTexture;
    sf::Texture& m_ammoTexture;
    sf::Texture& m_coinTexture;

    std::mt19937 m_rng;

    float m_spawnTimer;
    static constexpr float       SPAWN_INTERVAL = 8.f; // seconds between automatic spawns
    static constexpr std::size_t MAX_PICKUPS    = 15;

    sf::Texture& getTextureForType(PickupType type);
};