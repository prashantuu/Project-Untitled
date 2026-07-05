#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

// Generic reusable text-list menu — used for both the Main Menu
// and Pause Menu. It only knows about text options and a selected
// index; it has no idea what "Play" or "Restart" actually do.
// The owning screen (Game) decides what selecting an option means.
class Menu
{
public:
    Menu(sf::Font& font, const std::vector<std::string>& options, sf::Vector2f centerPosition);

    void moveUp();
    void moveDown();
    int getSelectedIndex() const;

    void draw(sf::RenderWindow& window);

private:
    std::vector<sf::Text> m_optionTexts;
    int m_selectedIndex;
};