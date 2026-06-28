#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <algorithm>
#include <optional>
#include <random>
#include <string>
struct Bullet
{
    sf::Sprite shape;
    sf::Vector2f direction;
    float speed = 500.f;
    bool alive = true;

    Bullet(const sf::Texture& texture)
        : shape(texture)
    {
        shape.setScale({0.08f, 0.08f});

        sf::FloatRect bounds = shape.getLocalBounds();
        shape.setOrigin({
            bounds.size.x / 2.f,
            bounds.size.y / 2.f
        });
    }
};

struct EnemyBullet
{
    sf::RectangleShape shape;
    sf::Vector2f direction;
    float speed = 250.f;
    bool alive = true;
};

struct Enemy
{
    sf::RectangleShape shape;
    int hp = 2;
    bool alive = true;
    float shootTimer = 0.f;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({900, 600}), "Tower Defense Shooter");
    window.setFramerateLimit(60);

    sf::Music music;
    if (!music.openFromFile("background.ogg.mp3"))
        return -1;

    music.setLooping(true);
    music.setVolume(30.f);
    music.play();

    // PLAYER (SPRITE FIXED)
    sf::Texture playerTexture;

if (!playerTexture.loadFromFile("player.png"))
    return -1;
sf::Sprite player(playerTexture);

player.setScale({0.5f, 0.5f});

sf::Vector2u texSize = playerTexture.getSize();

player.setOrigin({
    texSize.x / 2.f,
    texSize.y / 2.f
});

player.setPosition({100.f, 100.f});
    // enemy sprite
    sf::Texture enemyTexture;

if (!enemyTexture.loadFromFile("enemy.png"))
    return -1;
    // bullet sprite
sf::Texture bulletTexture;

if (!bulletTexture.loadFromFile("bullet.png"))
    return -1;
    // TOWER SPRITE
sf::Texture towerTexture;

if (!towerTexture.loadFromFile("tower.png"))
    return -1;
// background
sf::Texture backgroundTexture;

if (!backgroundTexture.loadFromFile("backgrounduse.png"))
    return -1;

sf::Sprite background(backgroundTexture);

// Scale to fit the 900x600 window
background.setScale({
    900.f / backgroundTexture.getSize().x,
    600.f / backgroundTexture.getSize().y
});
    // TOWER
sf::RectangleShape tower({100.f, 100.f});
tower.setTexture(&towerTexture);
tower.setPosition({120.f, 250.f});

    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<EnemyBullet> enemyBullets;

    int playerHP = 5;
    int towerHP = 5;
    int kills = 0;

    bool gameOver = false;
    bool gameWon = false;

    const float playerSpeed = 300.f;
    const float enemySpeed = 120.f;
float playerAngle = 0.f;
    sf::Clock clock;
    sf::Clock spawnClock;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(0.f, 900.f);
    std::uniform_real_distribution<float> yDist(0.f, 600.f);
    std::uniform_real_distribution<float> shootRandom(1.f, 3.f);

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouse->button == sf::Mouse::Button::Left)
                {
             Bullet b(bulletTexture);
// Muzzle position relative to player's center
float muzzleForward = 95.f;   // forward along the gun
float muzzleSide = -6.f;       // sideways adjustment

float rad = playerAngle * 3.14159265f / 180.f;

sf::Vector2f center = player.getPosition();

center.x += std::cos(rad) * muzzleForward - std::sin(rad) * muzzleSide;
center.y += std::sin(rad) * muzzleForward + std::cos(rad) * muzzleSide;
                    b.shape.setPosition(center);

                    sf::Vector2f mousePos =
                        window.mapPixelToCoords(sf::Mouse::getPosition(window));

                    sf::Vector2f dir = mousePos - center;

                    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                    if (len != 0)
                        dir /= len;

                    b.direction = dir;
                    float angle = atan2(dir.y, dir.x) * 180.f / 3.14159265f;
b.shape.setRotation(sf::degrees(angle));
                    bullets.push_back(b);
                }
            }
        }

        if (!gameOver && !gameWon)
        {
            // PLAYER MOVE
            sf::Vector2f movement(0.f, 0.f);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                movement.y -= playerSpeed * deltaTime;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                movement.y += playerSpeed * deltaTime;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                movement.x -= playerSpeed * deltaTime;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                movement.x += playerSpeed * deltaTime;

            player.move(movement);
            // Rotate player toward mouse
sf::Vector2f mousePos =
    window.mapPixelToCoords(sf::Mouse::getPosition(window));

playerAngle = atan2(
    mousePos.y - player.getPosition().y,
    mousePos.x - player.getPosition().x
) * 180.f / 3.14159265f;

player.setRotation(sf::degrees(playerAngle));

            // SPAWN ENEMIES
            if (spawnClock.getElapsedTime().asSeconds() > 2.f)
            {
                Enemy e;
                e.shape.setSize({80.f, 80.f});
e.shape.setTexture(&enemyTexture);

                int side = rand() % 4;

                if (side == 0) e.shape.setPosition({0.f, yDist(gen)});
                if (side == 1) e.shape.setPosition({900.f, yDist(gen)});
                if (side == 2) e.shape.setPosition({xDist(gen), 0.f});
                if (side == 3) e.shape.setPosition({xDist(gen), 600.f});

                e.shootTimer = shootRandom(gen);
                enemies.push_back(e);

                spawnClock.restart();
            }

            // ENEMY AI
            for (auto& e : enemies)
            {
                if (!e.alive) continue;

                sf::Vector2f dir = tower.getPosition() - e.shape.getPosition();
                float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

                if (len != 0)
                {
                    dir /= len;
                    e.shape.move(dir * enemySpeed * deltaTime);
                }

                e.shootTimer -= deltaTime;

                if (e.shootTimer <= 0.f)
                {
                    e.shootTimer = shootRandom(gen);

                    EnemyBullet b;
                    b.shape.setSize({8.f, 8.f});
                    b.shape.setFillColor(sf::Color::Red);

                    sf::Vector2f start = e.shape.getPosition();
                    b.shape.setPosition(start);

                    sf::Vector2f target = player.getPosition() - start;
                    float l = std::sqrt(target.x * target.x + target.y * target.y);

                    if (l != 0)
                        target /= l;

                    b.direction = target;
                    enemyBullets.push_back(b);
                }
            }
// PLAYER BULLETS
for (auto& b : bullets)
{
    if (!b.alive) continue;

    b.shape.move(b.direction * b.speed * deltaTime);

    sf::Vector2f bp = b.shape.getPosition();
    sf::FloatRect bs = b.shape.getGlobalBounds();
    for (auto& e : enemies)
    {
        if (!e.alive) continue;

        sf::Vector2f ep = e.shape.getPosition();
        sf::Vector2f es = e.shape.getSize();

        bool collision =
            bp.x < ep.x + es.x &&
            bp.x + bs.size.x > ep.x &&
            bp.y < ep.y + es.y &&
            bp.y + bs.size.y > ep.y;

        if (collision)
        {
            b.alive = false;
            e.hp--;

            if (e.hp <= 0)
            {
                e.alive = false;
                e.shape.setPosition({-1000.f, -1000.f});
                kills++;
            }
        }
    }
}

            // ENEMY BULLETS (FIXED PLAYER HIT BUG)
            for (auto& b : enemyBullets)
            {
                if (!b.alive) continue;

                b.shape.move(b.direction * b.speed * deltaTime);

                sf::Vector2f bp = b.shape.getPosition();
                sf::Vector2f bs = b.shape.getSize();

                sf::FloatRect ps = player.getGlobalBounds();
                sf::Vector2f pp = player.getPosition();

                if (bp.x < pp.x + ps.size.x &&
                    bp.x + bs.x > pp.x &&
                    bp.y < pp.y + ps.size.y &&
                    bp.y + bs.y > pp.y)
                {
                    b.alive = false;
                    playerHP--;
                }

                sf::Vector2f tp = tower.getPosition();
                sf::Vector2f ts = tower.getSize();

                if (bp.x < tp.x + ts.x &&
                    bp.x + bs.x > tp.x &&
                    bp.y < tp.y + ts.y &&
                    bp.y + bs.y > tp.y)
                {
                    b.alive = false;
                    towerHP--;
                }
            }

            bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                [](Bullet& b) { return !b.alive; }), bullets.end());

            enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(),
                [](EnemyBullet& b) { return !b.alive; }), enemyBullets.end());

            if (kills >= 20)
                gameWon = true;

            if (playerHP <= 0 || towerHP <= 0)
                gameOver = true;
        }

        window.setTitle(
            "Kills: " + std::to_string(kills) + "/20 | " +
            "Player HP: " + std::to_string(playerHP) + "/5 | " +
            "Tower HP: " + std::to_string(towerHP) + "/5"
        );

    window.clear();

window.draw(background);   // Draw background first

window.draw(player);
window.draw(tower);
        for (auto& e : enemies)
            if (e.alive)
                window.draw(e.shape);

        for (auto& b : bullets)
            window.draw(b.shape);

        for (auto& b : enemyBullets)
            window.draw(b.shape);

        window.display();
    }

    return 0;
}