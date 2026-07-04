#include "Droids.h"


//void Droids::starttexture()
//{
//	//Load a texture from file
//	if (!(this->texture.loadFromFile("Textures/droid.png"))) {
//		std::cout << "ERROR : DROIDS::STARTTEXTURE::COULDNT LOAD TEXTURE FILE" << std::endl;
//	}
//
//}


//void Droids::startsprite() {
//	//set the texture to sprite
//	this->sprite.setTexture(this->texture);
//
//	//Reseize the sprite
//	this->sprite.scale(0.025f, 0.025f); //Change as per need
//
//}


void Droids::startVariables()
{
	this->pointcounts = 5;/*rand() % 8 + 3;*/ // min value is 3 and Max is  10 as rand(0 to 7)
	this->type = 0;
	this->hp  = this->hpMax ;
	this->speed = 2.f;
	this->hpMax = 5;
	this->damage = 1;
	this->points = 5;
}	

void Droids::update() {
	this->sprite.move(0.f, this->speed);
}

void Droids::render(RenderTarget* target) {

	target->draw(this->sprite);

}

Droids::Droids(Texture* texture,float pos_x,float pos_y)
{
	this->startVariables();
	
	this->sprite.setTexture(*texture);
	this->sprite.setScale(0.025f, 0.025f);
	this->sprite.setPosition(pos_x, pos_y);
	/*this->starttexture();
	this->startsprite();*/
}


//Accessor
const FloatRect Droids::getBounds() const {
	return this->sprite.getGlobalBounds();      //it returns actual bounds of the sprite and not of any reference which might later cause any bugs
}
const int& Droids::getPoints() const
{
	return this->points;
}
Droids::~Droids()
{
}
