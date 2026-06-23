#include "Pickup.h"

Pickup::Pickup(sf::Vector2f position, PickupType type)
{
    this->type = type;

    body.setSize(sf::Vector2f(20.f, 20.f));
    body.setPosition(position);

    if (type == PickupType::Health)
        body.setFillColor(sf::Color::Green);

    if (type == PickupType::Ammo)
        body.setFillColor(sf::Color::Yellow);
}
void Pickup::render(sf::RenderWindow& window)
{
    window.draw(body);
}
sf::FloatRect Pickup::getBounds() const
{
    return body.getGlobalBounds();
}
PickupType Pickup::getType() const
{
    return type;
}