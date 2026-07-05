#include "ui/Menu.h"

Menu::Menu(sf::Font& font, const std::vector<std::string>& options, sf::Vector2f centerPosition)
    : m_selectedIndex(0)
{
    float spacing = 50.f;
    float startY = centerPosition.y - (options.size() - 1) * spacing / 2.f;

    for (std::size_t i = 0; i < options.size(); ++i)
    {
        sf::Text text(font);
        text.setString(options[i]);
        text.setCharacterSize(32);
        text.setFillColor(sf::Color::White);

        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        text.setPosition({centerPosition.x, startY + i * spacing});

        m_optionTexts.push_back(text);
    }
}

void Menu::moveUp()
{
    m_selectedIndex = (m_selectedIndex - 1 + static_cast<int>(m_optionTexts.size())) % static_cast<int>(m_optionTexts.size());
}

void Menu::moveDown()
{
    m_selectedIndex = (m_selectedIndex + 1) % static_cast<int>(m_optionTexts.size());
}

int Menu::getSelectedIndex() const
{
    return m_selectedIndex;
}

void Menu::draw(sf::RenderWindow& window)
{
    for (std::size_t i = 0; i < m_optionTexts.size(); ++i)
    {
        bool isSelected = (static_cast<int>(i) == m_selectedIndex);
        m_optionTexts[i].setFillColor(isSelected ? sf::Color(255, 220, 80) : sf::Color::White);
        m_optionTexts[i].setCharacterSize(isSelected ? 36 : 32);

        window.draw(m_optionTexts[i]);
    }
}