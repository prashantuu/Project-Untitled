#include "resources/ResourceManager.h"

sf::Texture& ResourceManager::getTexture(const std::string& path)
{
    auto it = m_textures.find(path);
    if (it != m_textures.end())
    {
        return *it->second;
    }

    auto texture = std::make_unique<sf::Texture>();
    texture->loadFromFile(path);

    sf::Texture& reference = *texture;
    m_textures.emplace(path, std::move(texture));
    return reference;
}

sf::Font& ResourceManager::getFont(const std::string& path)
{
    auto it = m_fonts.find(path);
    if (it != m_fonts.end())
    {
        return *it->second;
    }

    auto font = std::make_unique<sf::Font>();
    font->openFromFile(path);

    sf::Font& reference = *font;
    m_fonts.emplace(path, std::move(font));
    return reference;
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& path)
{
    auto it = m_soundBuffers.find(path);
    if (it != m_soundBuffers.end())
    {
        return *it->second;
    }

    auto buffer = std::make_unique<sf::SoundBuffer>();
    buffer->loadFromFile(path);

    sf::SoundBuffer& reference = *buffer;
    m_soundBuffers.emplace(path, std::move(buffer));
    return reference;
}

sf::Music& ResourceManager::getMusic(const std::string& path)
{
    auto it = m_music.find(path);
    if (it != m_music.end())
    {
        return *it->second;
    }

    auto music = std::make_unique<sf::Music>();
    music->openFromFile(path);

    sf::Music& reference = *music;
    m_music.emplace(path, std::move(music));
    return reference;
}