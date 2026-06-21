#include "Starship.h"



void Starship::starttexture()
{
	//Load a texture from file
	if (!(this->texture.loadFromFile("Textures/starship.png"))) {
		std::cout << "ERROR : STARSHIP::STARTTEXTURE::COULDNT LOAD TEXTURE FILE" << std::endl;
	}
}

void Starship::startsprite()
{
	//set the texture to sprite
	this->sprite.setTexture(this->texture);

	//Reseize the sprite
	this->sprite.scale(0.25f, 0.25f);
}

Starship::Starship()
{
	this->movementspeed = 5.f;
	this->starttexture();
	this->startsprite();
}

Starship::~Starship()
{
}

const Vector2f& Starship::getPos() const
{
	return this->sprite.getPosition();
}

void Starship::move(const float dirX, const float dirY)
{
	this->sprite.move(this->movementspeed * dirX, this->movementspeed * dirY);
}

void Starship::update() {


}

void Starship::render(RenderTarget& target) {

	target.draw(this->sprite);

}

