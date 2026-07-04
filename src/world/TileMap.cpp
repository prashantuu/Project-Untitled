#include "world/TileMap.h"

TileMap::TileMap(const std::vector<std::string>& layout, float tileSize)
    : m_tileSize(tileSize)
    , m_columns(layout.empty() ? 0 : static_cast<int>(layout[0].size()))
    , m_rows(static_cast<int>(layout.size()))
{
    m_tiles.resize(m_rows, std::vector<TileType>(m_columns, TileType::Floor));

    for (int row = 0; row < m_rows; ++row)
    {
        for (int column = 0; column < m_columns; ++column)
        {
            char symbol = layout[row][column];
            m_tiles[row][column] = (symbol == '#') ? TileType::Wall : TileType::Floor;
        }
    }
}

void TileMap::draw(sf::RenderWindow& window)
{
    // One shape, repositioned and recolored per tile — cheap for now.
    // We'll switch to a proper tile atlas / vertex array once textures
    // come in, since this becomes slow with big maps.
    sf::RectangleShape tileShape({m_tileSize, m_tileSize});

    for (int row = 0; row < m_rows; ++row)
    {
        for (int column = 0; column < m_columns; ++column)
        {
            TileType type = m_tiles[row][column];

            tileShape.setFillColor(
                type == TileType::Wall ? sf::Color(70, 70, 70) : sf::Color(30, 90, 40));

            tileShape.setPosition({column * m_tileSize, row * m_tileSize});

            window.draw(tileShape);
        }
    }
}

TileType TileMap::getTileType(int column, int row) const
{
    if (row < 0 || row >= m_rows || column < 0 || column >= m_columns)
    {
        // Treat anything outside the map as solid — useful once
        // wall collision is implemented next milestone.
        return TileType::Wall;
    }

    return m_tiles[row][column];
}

sf::Vector2f TileMap::getWorldSize() const
{
    return { m_columns * m_tileSize, m_rows * m_tileSize };
}