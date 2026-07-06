#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "resources/ResourceManager.h"

enum class TileType
{
    Floor,
    Wall
};

class TileMap
{
public:
    TileMap(const std::vector<std::string>& layout, float tileSize, ResourceManager& resources);

    void draw(sf::RenderWindow& window);

    TileType getTileType(int column, int row) const;
    sf::Vector2f getWorldSize() const;

private:
    std::vector<std::vector<TileType>> m_tiles;
    float m_tileSize;
    int m_columns;
    int m_rows;

    sf::Texture& m_tilesetTexture;
    sf::Sprite m_tileSprite;

    static const float SOURCE_TILE_SIZE;
};