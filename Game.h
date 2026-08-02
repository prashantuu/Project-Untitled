#pragma once
#include <map>
#include <string>
#include <sstream>

#include "Starship.h"
#include "Bullet.h"
#include "Droids.h"
#include "Whisper.h"
#include "AnimationManager.h"
#include "Boss.h"


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

	//BOSS
	Boss* boss;



	//Animation Manager
	AnimationManager animationManager;

	//systems
	unsigned points;

	



	//GUI
	Font font;
	Text pointText;
	Text levelFinishedText;
	
	Text GameOVertext;

	//Gamefinished 
	bool levelFinished = false;



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


	//Boss Related
	void startBoss();
	

	// Waves
	enum class WaveState { Spawning, WaveCooldown, AllWavesCleared };
	WaveState waveState = WaveState::Spawning;

	int currentWave = 1;
	static const int totalWaves = 3;
	
	int waveDroidCounts[totalWaves] = { 25, 25, 50 };// increasing per wave
	
	float waveSpawnIntervals[totalWaves] = { 0.8, 0.5f, 0.5f };  // faster each wave

	int droidsSpawnedThisWave = 0;
	int droidsAliveThisWave = 0;

	float waveSpawnTimer = 0.f;
	float waveCooldownTimer = 0.f;
	float waveCooldownMax = 4.f;   // gap between waves

	sf::Clock waveClock;   // for real dt




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

