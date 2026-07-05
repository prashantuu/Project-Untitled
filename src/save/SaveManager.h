#pragma once

#include <string>
#include "player/Player.h"
#include "gameplay/WaveManager.h"

// Plain-text key=value save format. Simple, human-readable, easy
// to debug by opening the file — not encrypted or compressed,
// which is fine for a single-player student project save file.
class SaveManager
{
public:
    static bool save(const Player& player, const WaveManager& waveManager,
        const std::string& path = "save.txt");

    static bool load(Player& player, WaveManager& waveManager,
        const std::string& path = "save.txt");
};