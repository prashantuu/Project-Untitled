#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <memory>
#include <string>

// Centralizes asset loading so the same file is never loaded twice.
// Every class that needs a texture/font/sound/music gets a reference
// to one shared instance instead of loading its own copy.
//
// Resources are heap-allocated (via unique_ptr) specifically so their
// memory address never changes once created — this matters because
// Enemy/Bullet sprites hold references to shared textures, and those
// references must stay valid even as more resources get loaded later.
//
// This is NOT a singleton — Game owns one instance and passes a
// reference to whatever classes need it (dependency injection).
class ResourceManager
{
public:
    sf::Texture& getTexture(const std::string& path);
    sf::Font& getFont(const std::string& path);
    sf::SoundBuffer& getSoundBuffer(const std::string& path);
    sf::Music& getMusic(const std::string& path);

private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_textures;
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> m_fonts;
    std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> m_soundBuffers;
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> m_music;
};