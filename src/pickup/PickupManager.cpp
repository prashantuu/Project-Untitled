#include "pickup/PickupManager.h"
#include <ctime>

const sf::Time PickupManager::SPAWN_INTERVAL = sf::seconds(5.f);
const std::size_t PickupManager::MAX_PICKUPS = 6;

PickupManager::PickupManager(sf::Vector2f worldSize, ResourceManager& resources)
    : m_healthTexture(resources.getTexture("assets/sprites/ui/health_pickup.png"))
    , m_ammoTexture(resources.getTexture("assets/sprites/ui/ammo_pickup.png"))
    , m_coinTexture(resources.getTexture("assets/sprites/ui/coin_pickup.png"))
    , m_worldSize(worldSize)
    , m_spawnTimer(sf::Time::Zero)
    , m_randomEngine(static_cast<unsigned int>(std::time(nullptr)))
{
}

void PickupManager::update(sf::Time deltaTime)
{
    for (auto& pickup : m_pickups)
        pickup.update(deltaTime);

    m_spawnTimer += deltaTime;
    if (m_spawnTimer >= SPAWN_INTERVAL)
    {
        m_spawnTimer = sf::Time::Zero;
        if (m_pickups.size() < MAX_PICKUPS)
        {
            spawnRandomPickup();
        }
    }
}

sf::Vector2f PickupManager::getRandomPosition()
{
    std::uniform_real_distribution<float> xDist(50.f, m_worldSize.x - 50.f);
    std::uniform_real_distribution<float> yDist(50.f, m_worldSize.y - 50.f);
    return { xDist(m_randomEngine), yDist(m_randomEngine) };
}

void PickupManager::spawnRandomPickup()
{
    std::uniform_int_distribution<int> typeDist(0, 2);
    int roll = typeDist(m_randomEngine);

    PickupType type;
    sf::Texture* texture;
    int value;

    switch (roll)
    {
        case 0:  type = PickupType::Health; texture = &m_healthTexture; value = 30; break;
        case 1:  type = PickupType::Ammo;   texture = &m_ammoTexture;   value = 0;  break;
        default: type = PickupType::Coin;   texture = &m_coinTexture;   value = 10; break;
    }

    m_pickups.emplace_back(*texture, getRandomPosition(), type, value);
}

void PickupManager::draw(sf::RenderWindow& window)
{
    for (auto& pickup : m_pickups)
        pickup.draw(window);
}

void PickupManager::checkCollisions(Player& player)
{
    for (auto it = m_pickups.begin(); it != m_pickups.end();)
    {
        if (it->getBounds().findIntersection(player.getBounds()))
        {
            switch (it->getType())
            {
                case PickupType::Health: player.heal(it->getValue()); break;
                case PickupType::Ammo:   player.refillAmmo(); break;
                case PickupType::Coin:   player.addScore(it->getValue()); break;
            }
            it = m_pickups.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void PickupManager::reset()
{
    m_pickups.clear();
    m_spawnTimer = sf::Time::Zero;
}