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
    // layout: one string per row, '#' = wall, anything else = floor.
    // tileSize: how big each tile is in pixels.
    TileMap(const std::vector<std::string>& layout, float tileSize);

    void draw(sf::RenderWindow& window);

    TileType getTileType(int column, int row) const;

    // Total size of the map in pixels — this is what defines
    // how big the world actually is.
    sf::Vector2f getWorldSize() const;

private:
    std::vector<std::vector<TileType>> m_tiles;
    float m_tileSize;
    int m_columns;
    int m_rows;
};