#ifndef OSUENV_H
#define OSUENV_H
#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
namespace fs = std::filesystem;


class OsuEnv {
private:
    std::vector<int> virtual4Keys;
    std::vector<int> virtual7Keys;
    fs::path osuRoot;
    fs::path osuSong;

public:
    OsuEnv(const std::string& rootDir);
    std::vector<int> getVirtualKeys(const int keyCount);
    void             setKeys(std::string& k4, std::string& k7);
};
#endif