
#include "Whisper.h"


Whisper::Whisper()
{
	this->startbuffers();
}

void Whisper::startbuffers()
{

	// Loading From File
	this->buffers["PLAYER"].loadFromFile("Audio/player.wav");   //Playter Shooting
	this->buffers["DROIDDED"].loadFromFile("Audio/droidded.wav"); //Droid getting Destroid
	this->buffers["CHARGE"].loadFromFile("Audio/bosslasercharge.wav");//Laser Charging
	this->buffers["BOSSLASER"].loadFromFile("Audio/bosslaser.wav");//Boss laser Shoot
		


	//Setting Buffers
	this->playershoot.setBuffer(this->buffers["PLAYER"]); //Player Shooting
	this->droidded.setBuffer(this->buffers["DROIDDED"]); //Droid getting destroid
	this->chargesound.setBuffer(this->buffers["CHARGE"]);//Boss Laser Charging
	this->bosslaser.setBuffer(this->buffers["BOSSLASER"]);//Boss Laser Shoot



	// Configuration Stuffs
	this->playershoot.setVolume(20.f);
	this->droidded.setVolume(20.f);
	this->chargesound.setVolume(20.f);



}

void Whisper::playbosscharge()
{
	this->chargesound.play();
}

void Whisper::playbossshoot()
{
	this->bosslaser.play();
}

float Whisper::getChargeSoundDuration()
{
	return this->buffers["CHARGE"].getDuration().asSeconds();
}

void Whisper::playplayershoot()
{
	this->playershoot.play();
	
}

void Whisper::playdroidded()
{
	this->droidded.play();
}

void Whisper::playbossmusic()
{
	this->bgMusic.openFromFile("Audio/bossmusic.mp3");
	this->bgMusic.setVolume(25.f);
	this->bgMusic.play();
}

void Whisper::playbgmusic()
{
	this->bgMusic.openFromFile("Audio/bgmusic.mp3");
	this->bgMusic.setVolume(10.f);
	this->bgMusic.play();
}

void Whisper::stopbgmusic()
{
	this->bgMusic.stop();
}



Whisper::~Whisper()
{
}


