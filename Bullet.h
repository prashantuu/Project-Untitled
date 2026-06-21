#pragma once
#define BULLET_H

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet
{
private:

	Sprite shape;
	

	Vector2f direction;
	float movementspeed;

public:
	Bullet();
	Bullet(Texture* texture, float pos_x, float pos_y , float dir_X,float dir_Y, float movement_speed);
	virtual ~Bullet();

	//Accessor of Coordinates

	const FloatRect getBounds() const;

	void update();
	void render(RenderTarget* target);
};

