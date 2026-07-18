
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



	//Setting Buffers
	this->playershoot.setBuffer(this->buffers["PLAYER"]); //Player Shooting
	this->droidded.setBuffer(this->buffers["DROIDDED"]); //Droid getting destroid



	// Configuration Stuffs
	this->playershoot.setVolume(20.f);
	this->droidded.setVolume(20.f);



}

void Whisper::playplayershoot()
{
	this->playershoot.play();
	
}

void Whisper::playdroidded()
{
	this->droidded.play();
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


