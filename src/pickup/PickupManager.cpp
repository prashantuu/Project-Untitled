#include "PickupManager.h"
#include "../resources/ResourceManager.h"
#include "../player/Player.h"
#include <chrono>

PickupManager::PickupManager(ResourceManager& resources, sf::Vector2f worldSize)
    : m_worldSize(worldSize)
    , m_healthTexture(resources.getTexture("assets/sprites/pickups/health.png"))
    , m_ammoTexture(resources.getTexture("assets/sprites/pickups/ammo.png"))
    , m_coinTexture(resources.getTexture("assets/sprites/pickups/coin.png"))
    , m_rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()))
    , m_spawnTimer(0.f)
{
}

sf::Texture& PickupManager::getTextureForType(PickupType type)
{
    switch (type)
    {
        case PickupType::Health: return m_healthTexture;
        case PickupType::Ammo:   return m_ammoTexture;
        case PickupType::Coin:   return m_coinTexture;
    }
    return m_coinTexture;
}

void PickupManager::spawnPickup(PickupType type, sf::Vector2f position)
{
    m_pickups.emplace_back(type, position, getTextureForType(type));
}

void PickupManager::spawnRandomPickup()
{
    if (m_pickups.size() >= MAX_PICKUPS)
        return;

    // Kept 50px inside the world edges, same margin used for enemy spawning,
    // so pickups never land straddling the boundary wall.
    std::uniform_real_distribution<float> distX(50.f, m_worldSize.x - 50.f);
    std::uniform_real_distribution<float> distY(50.f, m_worldSize.y - 50.f);
    std::uniform_int_distribution<int>    distType(0, 2);

    sf::Vector2f position(distX(m_rng), distY(m_rng));
    PickupType   type = static_cast<PickupType>(distType(m_rng));

    spawnPickup(type, position);
}

void PickupManager::update(sf::Time deltaTime)
{
    for (auto& pickup : m_pickups)
        pickup.update(deltaTime);

    m_spawnTimer += deltaTime.asSeconds();
    if (m_spawnTimer >= SPAWN_INTERVAL)
    {
        m_spawnTimer = 0.f;
        spawnRandomPickup();
    }
}

void PickupManager::draw(sf::RenderWindow& window)
{
    for (auto& pickup : m_pickups)
        pickup.draw(window);
}

PickupCollectionReport PickupManager::checkCollisions(Player& player)
{
    PickupCollectionReport report;

    for (auto it = m_pickups.begin(); it != m_pickups.end(); )
    {
        // SFML 3's findIntersection() returns std::optional<FloatRect>,
        // which converts to bool - same pattern used in every other
        // collision check in this project (bullets vs enemies, etc.)
        if (it->getBounds().findIntersection(player.getBounds()))
        {
            switch (it->getType())
            {
                case PickupType::Health:
                    player.heal(it->getValue());
                    report.healthCollected = true;
                    break;

                case PickupType::Ammo:
                    player.addAmmo(it->getValue());
                    report.ammoCollected = true;
                    break;

                case PickupType::Coin:
                    // Coins have no meaning inside Player (no "score" member
                    // there) - this is a Game-level concept, so it's only
                    // reported upward, never applied here.
                    report.coinsCollected += it->getValue();
                    break;
            }

            it = m_pickups.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return report;
}