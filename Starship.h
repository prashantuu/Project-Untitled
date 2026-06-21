#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace sf;

class Starship
{
private:
	Sprite sprite;
	Texture texture;

	float movementspeed;

	void starttexture();
	void startsprite();

public:
	Starship();
	virtual ~Starship();

	//Accessor
	const Vector2f& getPos() const;

	//Functions
	void move(const float dirX, const float dirY);


	void update();
	void render(RenderTarget& target);  // RenderTarget an abstract base class that defines the interface for drawing 2D entities
};

