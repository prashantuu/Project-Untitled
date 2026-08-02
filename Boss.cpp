#include "Boss.h"
#include <cmath>

Boss::Boss(const sf::Texture& bodyTexture, const sf::Texture& chargeTexture_, const sf::Texture& laserTexture_,
    float windowWidth_, float windowHeight_, float spawnDelaySeconds)
    : chargeTexture(&chargeTexture_)
    , laserTexture(&laserTexture_)
    , windowWidth(windowWidth_)
    , windowHeight(windowHeight_)
{
    bodySprite.setTexture(bodyTexture);
    sf::FloatRect b = bodySprite.getLocalBounds();
    bodySprite.setOrigin(b.width / 2.f, b.height / 2.f);
    bodySprite.setScale(0.25f, 0.25f);   // tune to taste
    bodySprite.setPosition(windowWidth / 2.f, -b.height * 0.35f);   // parked above the screen

    // HP bar — centered horizontally, pinned near the top of the screen
    hpBarBack.setSize(sf::Vector2f(hpBarMaxWidth, hpBarHeight));
    hpBarBack.setFillColor(sf::Color(25, 25, 25, 200));
    hpBarBack.setPosition(windowWidth / 2.f - hpBarMaxWidth +100.f/ 2.f, 50.f);

    hpBarFill.setSize(sf::Vector2f(hpBarMaxWidth, hpBarHeight));
    hpBarFill.setFillColor(sf::Color::Red);
    hpBarFill.setPosition(hpBarBack.getPosition());




}

void Boss::trackTowardsPlayer(float playerX, float dt)
{
    sf::Vector2f pos = bodySprite.getPosition();
    float dx = playerX - pos.x;
    float step = trackSpeed * dt;
    if (std::abs(dx) <= step) pos.x = playerX;
    else                      pos.x += (dx > 0.f ? step : -step);
    bodySprite.setPosition(pos);
}

void Boss::setChargeSoundDuration(float seconds)
{
    this->chargeSoundDuration = seconds;
}

void Boss::update(const sf::Vector2f& playerPos, const sf::FloatRect& playerBounds)
{

    if (isDead())
    {
        state = State::Dead;
        return;   // frozen — no more charging/firing/tracking once dead
    }

    float dt = m_clock.restart().asSeconds();

    hitEventPending = false;

    switch (state)
    {
    case State::Waiting:
    {
        
        //if (spawnElapsed >= spawnDelay)
        //    state = State::Entering;
        break;
    }

    case State::Entering:
    {
        sf::Vector2f pos = bodySprite.getPosition();
        pos.y += entrySpeed * dt;
        if (pos.y >= targetY) { pos.y = targetY; state = State::Tracking; }
        bodySprite.setPosition(pos);
        break;
    }

    case State::Tracking:
    {
        trackTowardsPlayer(playerPos.x, dt);
        float frameDur = chargeSoundDuration / 8.f;
        // Begin charging immediately — tracking continues INTO the charge phase
        chargeAnim = Animation(*chargeTexture, 8, frameDur,
            bodySprite.getPosition().x, bodySprite.getPosition().y + 60.f,
            0.6f, 0.6f, false);
        chargeStartedPending = true;
        state = State::Charging;
        break;
    }

    case State::Charging:
    {
        // Keep tracking the player WHILE powering up — this is the player's dodge window
        trackTowardsPlayer(playerPos.x, dt);

        sf::Vector2f pos = bodySprite.getPosition();
        chargeAnim.setPosition(pos.x, pos.y + 55.f);
        chargeAnim.update(dt);

        if (chargeAnim.isFinished())
        {
            // Position LOCKS here — the shot fires straight down from wherever the boss now is
            float fireX = pos.x;
            float fireY = pos.y + 52.f;
            float maxLen = windowHeight - fireY;
            laser.fire(*laserTexture, 8, 0.035f, fireX, fireY, maxLen);
            this->whisper.playbossshoot();
            state = State::Firing;
        }
        break;
    }

    case State::Firing:
    {
        LaserBeam::HitResult result = laser.update(dt, playerBounds);
        if (result == LaserBeam::HitResult::HitPlayer)
        {
            hitEventPending = true;
            laser.startShrinking();
            state = State::Retracting;
        }
        else if (result == LaserBeam::HitResult::ReachedBottom)
        {
            laser.startShrinking();
            state = State::Retracting;
        }
        break;
    }

    case State::Retracting:
    {
        laser.update(dt, playerBounds);
        if (laser.isFinished())
        {
            cooldownTimer = 0.f;
            state = State::Cooldown;
        }
        break;
    }

    case State::Cooldown:
    {
        cooldownTimer += dt;
        if (cooldownTimer >= cooldownMax)
            state = State::Tracking;
        break;
    }
    }
}

void Boss::render(sf::RenderWindow& target)
{
    if (state == State::Waiting || state == State::Dead) return;

    if (state == State::Waiting) return;   // not spawned yet, draw nothing

    target.draw(bodySprite);

    if (state == State::Charging)
        chargeAnim.render(target);

    if (state == State::Firing || state == State::Retracting)
        laser.render(target);

    // Boss HP bar, fixed at top-middle of screen
    target.draw(hpBarBack);
    target.draw(hpBarFill);
}

bool Boss::consumeChargeStartedEvent()
{
    bool v = chargeStartedPending;
    chargeStartedPending = false;
    return v;
}

void Boss::triggerSpawn()
{
    if (state == State::Waiting)
    {
        state = State::Entering;
        spawnEventPending = true;
    }
}

sf::FloatRect Boss::getBounds() const { return bodySprite.getGlobalBounds(); }
void Boss::takeDamage(int dmg)
{
    hp -= dmg;
    if (hp < 0) hp = 0;

    float hpPercent = static_cast<float>(hp) / static_cast<float>(maxHp);
    hpBarFill.setSize(sf::Vector2f(hpBarMaxWidth * hpPercent, hpBarHeight));

    if (hp <= 0 && !deathHandled)
    {
        deathHandled = true;
        deathEventPending = true;
    }
}
int  Boss::getHP() const { return hp; }
bool Boss::isDead() const { return hp <= 0; }
bool Boss::isActive() const { return state != State::Waiting; }
int  Boss::getLaserDamage() const { return laserDamage; }

bool Boss::consumeDeathEvent()
{
    bool v = deathEventPending;
    deathEventPending = false;
    return v;
}

bool Boss::consumeHitEvent()
{
    bool v = hitEventPending;
    hitEventPending = false;
    return v;
}

bool Boss::consumeSpawnEvent()
{
    bool v = spawnEventPending;
    spawnEventPending = false;
    return v;
}
