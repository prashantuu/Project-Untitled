#pragma once
#include<SFML/Graphics.hpp>
class Bullet
{
private:
	sf::CircleShape shape;
	sf::Vector2f direction;
	float speed;
public:
	Bullet(sf::Vector2f position, sf::Vector2f target);
	void update(float deltatime);
	void render(sf::RenderWindow& window);
	sf::FloatRect getBounds() const;//for collision//
};

