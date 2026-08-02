#pragma once
#define BULLET_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

class Bullet
{
private:

	Sprite shape;

	int damage;

	Vector2f direction;
	float movementspeed;

public:
	Bullet();
	Bullet(Texture* texture, float pos_x, float pos_y, float dir_X, float dir_Y, float movement_speed, int damage = 1);
	int getDamage() const;
	virtual ~Bullet();

	//Accessor of Coordinates

	const FloatRect getBounds() const;

	void update();
	void render(RenderTarget* target);
};

