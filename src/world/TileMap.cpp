#include "world/TileMap.h"

const float TileMap::SOURCE_TILE_SIZE = 32.f;

TileMap::TileMap(const std::vector<std::string>& layout, float tileSize, ResourceManager& resources)
    : m_tileSize(tileSize)
    , m_columns(layout.empty() ? 0 : static_cast<int>(layout[0].size()))
    , m_rows(static_cast<int>(layout.size()))
    , m_tilesetTexture(resources.getTexture("assets/sprites/tiles/tileset.png"))
    , m_tileSprite(m_tilesetTexture)
{
    float scale = m_tileSize / SOURCE_TILE_SIZE;
    m_tileSprite.setScale({scale, scale});

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
    int sourceSize = static_cast<int>(SOURCE_TILE_SIZE);

    for (int row = 0; row < m_rows; ++row)
    {
        for (int column = 0; column < m_columns; ++column)
        {
            TileType type = m_tiles[row][column];

            int tileIndex = (type == TileType::Wall) ? 1 : 0;
            m_tileSprite.setTextureRect(
                sf::IntRect({ tileIndex * sourceSize, 0 }, { sourceSize, sourceSize }));

            m_tileSprite.setPosition({ column * m_tileSize, row * m_tileSize });

            window.draw(m_tileSprite);
        }
    }
}

TileType TileMap::getTileType(int column, int row) const
{
    if (row < 0 || row >= m_rows || column < 0 || column >= m_columns)
    {
        return TileType::Wall;
    }

    return m_tiles[row][column];
}

sf::Vector2f TileMap::getWorldSize() const
{
    return { m_columns * m_tileSize, m_rows * m_tileSize };
}