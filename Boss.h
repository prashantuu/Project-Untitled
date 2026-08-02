#pragma once
#include "LaserBeam.h"
#include "Animation.h"
#include "Whisper.h"
#include <SFML/Graphics.hpp>

class Boss
{
public:
    Boss(const sf::Texture& bodyTexture, const sf::Texture& chargeTexture, const sf::Texture& laserTexture,
        float windowWidth, float windowHeight, float spawnDelaySeconds = 15.f);

    float chargeSoundDuration = 0.72f;
    void setChargeSoundDuration(float seconds);
    void update(const sf::Vector2f& playerPos, const sf::FloatRect& playerBounds);
    void render(sf::RenderWindow& target);


    bool consumeChargeStartedEvent();


    void triggerSpawn(); // call this externally when it's time to spawn

    sf::FloatRect getBounds() const;
    void takeDamage(int dmg);
    int  getHP() const;
    bool isDead() const;
    bool isActive() const;      // false while still waiting to spawn

    // Returns true exactly once, the frame the laser connects with the player
    
    bool consumeDeathEvent();
    bool consumeHitEvent();
    bool consumeSpawnEvent();
    int  getLaserDamage() const;

private:
    enum class State { Waiting, Entering, Tracking, Charging, Firing, Retracting, Cooldown, Dead };
    State state = State::Waiting;

    sf::Sprite bodySprite;
    Animation chargeAnim;
    LaserBeam laser;

    Whisper whisper;

    // HP bar (fixed at top-middle of screen)
    sf::RectangleShape hpBarBack;
    sf::RectangleShape hpBarFill;
    float hpBarMaxWidth = 150.f;
    float hpBarHeight = 10.f;


    bool deathEventPending = false;
    bool spawnEventPending = false;
    bool deathHandled = false;
    bool chargeStartedPending = false;

    const sf::Texture* chargeTexture;
    const sf::Texture* laserTexture;

    sf::Clock m_clock;

    float windowWidth, windowHeight;

    
    //float spawnDelay = 15.f;
    //float spawnElapsed = 0.f;
    

    float entrySpeed = 60.f;
    float targetY = 130.f;

    float trackSpeed = 150.f;

    float cooldownTimer = 0.f;
    float cooldownMax = 2.5f;

    int hp = 250, maxHp = 250;
    int laserDamage = 100;

    bool hitEventPending = false;

    void trackTowardsPlayer(float playerX, float dt);
};