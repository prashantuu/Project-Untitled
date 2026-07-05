#include "save/SaveManager.h"
#include <fstream>
#include <sstream>
#include <map>

bool SaveManager::save(const Player& player, const WaveManager& waveManager, const std::string& path)
{
    std::ofstream file(path);
    if (!file.is_open())
        return false;

    file << "health=" << player.getHealth() << "\n";
    file << "score=" << player.getScore() << "\n";
    file << "wave=" << waveManager.getCurrentWave() << "\n";
    file << "weapon=" << static_cast<int>(player.getWeaponType()) << "\n";

    return true;
}

bool SaveManager::load(Player& player, WaveManager& waveManager, const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return false;

    std::map<std::string, int> values;
    std::string line;

    while (std::getline(file, line))
    {
        std::size_t equalsPos = line.find('=');
        if (equalsPos == std::string::npos)
            continue;

        std::string key = line.substr(0, equalsPos);
        int value = std::stoi(line.substr(equalsPos + 1));
        values[key] = value;
    }

    if (values.count("health")) player.setHealth(values["health"]);
    if (values.count("score"))  player.setScore(values["score"]);
    if (values.count("wave"))   waveManager.setWave(values["wave"]);
    if (values.count("weapon")) player.setWeaponType(static_cast<WeaponType>(values["weapon"]));

    return true;
}