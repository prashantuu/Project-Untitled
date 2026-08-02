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
		Music bossmusic;

		Sound playershoot;
		Sound droidded;
		Sound chargesound;
		Sound bosslaser;


		map<string, SoundBuffer> buffers;





public:
	Whisper();
	

	void startbuffers();

	void playbosscharge();
	void playbossshoot();
	float getChargeSoundDuration();
	void playplayershoot();
	void playdroidded();

	void playbossmusic();


	void playbgmusic();
	void stopbgmusic();




	virtual ~Whisper();
};

