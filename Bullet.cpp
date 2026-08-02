#include "Bullet.h"



Bullet::Bullet() {

}

Bullet::Bullet(Texture* texture, float pos_x, float pos_y, float dir_X, float dir_Y, float movement_speed, int damage)
    : damage(damage)
{
    this->shape.setTexture(*texture);
    this->movementspeed = movement_speed;
    this->shape.setPosition(pos_x, pos_y);
    this->direction.x = dir_X;
    this->direction.y = dir_Y;
}

int Bullet::getDamage() const { return this->damage; }

Bullet::~Bullet()
{
}

const FloatRect Bullet::getBounds() const
{
	return this->shape.getGlobalBounds();
}

void Bullet::update() {
	//Movements
	this->shape.move(this->movementspeed * this->direction);
}

void Bullet::render(RenderTarget* target) {
	target->draw(this->shape);
}
