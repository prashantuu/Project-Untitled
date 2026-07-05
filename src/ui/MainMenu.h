#pragma once

#include <SFML/Graphics.hpp>

class ResourceManager;

// The two things a player can pick on the title screen. Game reads
// this once Enter is pressed and decides what it means (start
// playing, or close the window) — MainMenu itself has no idea
// those actions exist, it only tracks which option is highlighted.
enum class MenuOption
{
    StartGame,
    Exit
};

class MainMenu
{
public:
    MainMenu(ResourceManager& resources);

    void draw(sf::RenderWindow& window);

    // Only two options right now, so "up" and "down" both just
    // toggle between them — this naturally extends to a real list
    // later (e.g. adding Settings) without changing the interface.
    void moveSelectionUp();
    void moveSelectionDown();

    MenuOption getSelectedOption() const;

private:
    void centerText(sf::Text& text, sf::Vector2f position);
    void refreshSelectionColors();

private:
    sf::Text m_titleText;
    sf::Text m_startText;
    sf::Text m_exitText;

    MenuOption m_selected;
};