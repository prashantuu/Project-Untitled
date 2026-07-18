#pragma once

#include <SFML/Audio.hpp>
#include<iostream>
#include<string>

#include <map>

using namespace sf;
using namespace std;


class Whisper
{

private:

		Music bgMusic;


		Sound playershoot;
		Sound droidded;


		map<string, SoundBuffer> buffers;





public:
	Whisper();
	

	void startbuffers();
	void playplayershoot();
	void playdroidded();

	void playbgmusic();
	void stopbgmusic();




	virtual ~Whisper();
};

