#include "Starship.h"



void Starship::startvariable()
{
	this->movementspeed = 5.f;

	this->hpmax = 10;
	this->hp = this->hpmax;
	//this->attackcooldownmax = 10.f;
	//this->attackcooldown = this->attackcooldownmax;
	

}


const FloatRect Starship::getBounds() const
{
	return this->sprite.getGlobalBounds();
}

const int& Starship::getHP() const
{
	return this->hp;
}

const int& Starship::getHPmax() const
{
	return this->hpmax;
}

void Starship::setPosition(const Vector2f pos)
{
	this->sprite.setPosition(pos);

}

void Starship::setPosition(const float x, const float y)
{
	this->sprite.setPosition(x, y);
}



void Starship::setHP(const int hp)
{
	this->hp = hp;
}

void Starship::loseHp(const int value)
{
	this->hp -= value;
	if (this->hp < 0)
		this->hp = 0;
}

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
	this->startvariable();
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

//const bool Starship::canattack()
//{
//	return false;
//}

//const bool Starship::canattack()
//{
//	if (this->attackcooldown >= this->attackcooldownmax)
//	{
//		this->attackcooldown = 0.5f;
//		return true;
//	}
//	return false;
//}

//void Starship::updateAttackcooldown()
//{
//	if(this->attackcooldown < this->attackcooldownmax)
//		this->attackcooldown += 1.0f;
//}

void Starship::update() {

	/*this->updateAttackcooldown();*/
}

void Starship::render(RenderTarget& target) {

	target.draw(this->sprite);

}

