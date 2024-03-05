#pragma once
#include <filesystem>
#include <iostream>
#include <vector>
namespace fs = std::filesystem;


class OsuEnv {
private:
    std::vector<int> virtual4Keys;
    std::vector<int> virtual7Keys;
    fs::path osuRoot;
    fs::path osuSong;
    int modulesInUse;

public:
    std::string full4K, full7K, onModules = "";
    OsuEnv(const std::string&, const int&);
    std::string      toLowercase(const std::string&);
    std::vector<int> getVirtualKeys(const int);
    void             getModulesInUse(bool&, bool&, bool&, bool&);
    void             setKeys(std::string&, std::string&);
    void             setModulesInUse(std::string&);
    void             writeConfigs();
    
};