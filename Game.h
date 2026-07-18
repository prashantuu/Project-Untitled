#pragma once
#include <map>
#include <string>
#include <sstream>

#include "Starship.h"
#include "Bullet.h"
#include "Droids.h"
#include "Whisper.h"
#include "AnimationManager.h"


#include <string.h>
#include <iostream>



using namespace sf;

class Game
{
private:

	
	//Window
	RenderWindow* window; //lets us easily pass the window object around and good for memory management

	//Player
	Starship* starship;

	//Audio
	Whisper whisper;


	//World
	Texture worldbackgroundtex;
	Sprite worldbackground;


	//Ship GUI
	RectangleShape playerhpbar;
	RectangleShape playerhpbarback;


	//Animation Manager
	AnimationManager animationManager;

	//systems
	unsigned points;

	



	//GUI
	Font font;
	Text pointText;

	Text GameOVertext;


	//Droids
	float spawnTimer;
	float spawnTimerMax;

	std::vector<Droids*>droids;


	//Resources
	std::map<std::string, Texture*> textures; //map is like dictionary of python


	
	std::vector<Bullet*> bullets;

	//Window Functions
	void startwindow();
	void startbackground();
	void startsystems();
	void starttextures();
	
	void startstarship();

	void startdroids();

	void startGUI();



	




public :
	Game();
	virtual ~Game();

	//Functions to run the game
	void run();

	void updatePollEvents();
	void updateInput();
	void updatebullets();
	void updateworld();
	void updatecollision();
	void updatedroids();
	void updateCombat();
	void updateGUI();
	void update();

	void renderworld();
	void renderGUI();
	void render();
};

