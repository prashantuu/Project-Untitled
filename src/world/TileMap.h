#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum class TileType
{
    Floor,
    Wall
};

class TileMap
{
public:
    TileMap(const std::vector<std::string>& layout, float tileSize);

    void draw(sf::RenderWindow& window);

    TileType getTileType(int column, int row) const;
    sf::Vector2f getWorldSize() const;

private:
    std::vector<std::vector<TileType>> m_tiles;
    float m_tileSize;
    int m_columns;
    int m_rows;

    // Only one TileMap ever exists (owned directly by Game, never
    // stored in a container), so it's safe for it to own its own
    // texture — no reallocation risk like Enemy/Bullet have.
    sf::Texture m_tilesetTexture;
    sf::Sprite m_tileSprite;

    // Pixel size of a single tile *in the source tileset image*,
    // before scaling up to m_tileSize. Adjust to match whatever
    // tileset you download.
    static const float SOURCE_TILE_SIZE;
};