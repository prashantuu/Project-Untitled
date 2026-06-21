#pragma once
#include <map>
#include "Starship.h"
#include <string.h>
#include <iostream>
#include "Bullet.h"


using namespace sf;

class Game
{
private:

	//Window
	RenderWindow* window; //lets us easily pass the window object around and good for memory management

	//Player
	Starship* starship;

	//Resources
	std::map<std::string, Texture*> textures; //map is like dictionary of python
	std::vector<Bullet*> bullets;

	//Window Functions
	void startwindow();
	void starttextures();

	void startstarship();



	




public :
	Game();
	virtual ~Game();

	//Functions to run the game
	void run();

	void updatePollEvents();
	void updateInput();
	void updatebullets();
	void update();
	void render();
};

