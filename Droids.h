#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace sf;

class Droids
{
private:

	Sprite sprite;

	unsigned pointcounts;

	int type;
	float speed;
	int hp;
	int hpMax;
	int damage;
	int points;

	//void startsprite();
	//void starttexture();
	void startVariables();


public:


	//Accessors
	const FloatRect getBounds() const;
	const int& getPoints() const;


	void update();
	void render(RenderTarget* target);  // RenderTarget an abstract base class that defines the interface for drawing 2D entities

	Droids(Texture* texture,float pos_x, float pos_y);
	virtual ~Droids();
};

