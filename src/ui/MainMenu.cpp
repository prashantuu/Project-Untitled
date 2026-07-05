#include "ui/MainMenu.h"
#include "resources/ResourceManager.h"

namespace
{
    const sf::Color SELECTED_COLOR(230, 230, 80);
    const sf::Color UNSELECTED_COLOR(200, 200, 200);
}

MainMenu::MainMenu(ResourceManager& resources)
    : m_titleText(resources.getFont("assets/fonts/game_font.ttf"))
    , m_startText(resources.getFont("assets/fonts/game_font.ttf"))
    , m_exitText(resources.getFont("assets/fonts/game_font.ttf"))
    , m_selected(MenuOption::StartGame)
{
    // Positions are fixed against the 800x600 window Game constructs —
    // same assumption Game already makes for the Camera's initial view
    // size, so nothing new is being hardcoded here that wasn't already.
    m_titleText.setString("TOP DOWN SHOOTER");
    m_titleText.setCharacterSize(48);
    m_titleText.setFillColor(sf::Color::White);
    centerText(m_titleText, {400.f, 180.f});

    m_startText.setString("Start Game");
    m_startText.setCharacterSize(32);
    centerText(m_startText, {400.f, 340.f});

    m_exitText.setString("Exit");
    m_exitText.setCharacterSize(32);
    centerText(m_exitText, {400.f, 400.f});

    refreshSelectionColors();
}

void MainMenu::centerText(sf::Text& text, sf::Vector2f position)
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    text.setPosition(position);
}

void MainMenu::refreshSelectionColors()
{
    m_startText.setFillColor(m_selected == MenuOption::StartGame ? SELECTED_COLOR : UNSELECTED_COLOR);
    m_exitText.setFillColor(m_selected == MenuOption::Exit ? SELECTED_COLOR : UNSELECTED_COLOR);
}

void MainMenu::moveSelectionUp()
{
    m_selected = MenuOption::StartGame;
    refreshSelectionColors();
}

void MainMenu::moveSelectionDown()
{
    m_selected = MenuOption::Exit;
    refreshSelectionColors();
}

MenuOption MainMenu::getSelectedOption() const
{
    return m_selected;
}

void MainMenu::draw(sf::RenderWindow& window)
{
    window.draw(m_titleText);
    window.draw(m_startText);
    window.draw(m_exitText);
}