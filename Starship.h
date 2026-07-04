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


	int hp;
	int hpmax;

	/*float attackcooldown;
	float attackcooldownmax;*/

	void startvariable();
	void starttexture();
	void startsprite();
	

public:
	Starship();
	virtual ~Starship();

	//Accessor
	const Vector2f& getPos() const;
	const FloatRect getBounds() const;
	const int& getHP() const;
	const int& getHPmax() const;


	//Modifiers
	void setPosition(const Vector2f pos);
	void setPosition(const float x, const float y);
	void setHP(const int hp);
	void loseHp(const int value);

	//Functions
	void move(const float dirX, const float dirY);
	/*const bool canattack();*/

	/*void updateAttackcooldown();*/
	void update();
	void render(RenderTarget& target);  // RenderTarget an abstract base class that defines the interface for drawing 2D entities
};

