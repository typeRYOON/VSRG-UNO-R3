#include "osuenv.hpp"
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>


std::string OsuEnv::toLowercase(const std::string& str) {
    std::string lowercaseStr = str;
    std::transform(
        lowercaseStr.begin(),
        lowercaseStr.end(),
        lowercaseStr.begin(),
        ::tolower);
    return lowercaseStr;
}


void OsuEnv::writeConfigs()
{
    std::ofstream configs("VSRG-UNO-R3.cfg");
    if (!configs.is_open()) {
        throw std::invalid_argument("Unable to write to config file.\n\n");
    }
    configs << osuRoot.string() << '\n' << modulesInUse;
    configs.close();
}


void OsuEnv::getModulesInUse(bool& AU, bool& SA, bool& KS, bool& KPS)
{
    AU = modulesInUse & 1;
    SA = modulesInUse & 2;
    KS = modulesInUse & 4;
    KPS = modulesInUse & 8;

    if (AU && SA) {
        throw std::invalid_argument
            ("Auto Player and Solenoid Array can't both be set to on.");
    }
    else if (!(AU || SA || KS || KPS)) {
        throw std::invalid_argument
            ("Must have at least one module set to on.");
    }

    if (AU)  { onModules += "AU " ; }
    if (SA)  { onModules += "SA " ; }
    if (KS)  { onModules += "KS " ; }
    if (KPS) { onModules += "KPS "; }
}


void OsuEnv::setModulesInUse(std::string& arg)
{
    std::unordered_map<std::string, short> args = 
        {{"au", 1}, {"sa", 2}, {"ks", 4}, {"kps", 8}};
    std::istringstream ss(arg);
    std::string argument;
    int old = modulesInUse;
    modulesInUse = 0;

    while (ss >> argument) {
        if (args.count(toLowercase(argument))) {
            modulesInUse |= args[toLowercase(argument)];
        }
        else if (toLowercase(argument) == "old") {
            modulesInUse = old;
        }
    }
}


std::vector<int> OsuEnv::getVirtualKeys(const int keyCount)
{
    if (keyCount == 4) { return virtual4Keys; }
    if (keyCount == 7) { return virtual7Keys; }
    throw std::invalid_argument("Please only play 4K or 7K maps.");
}


void OsuEnv::setKeys(std::string& k4, std::string& k7)
{
    std::unordered_map<std::string, int> keyMappings;
    std::istringstream ss4(k4);
    std::istringstream ss7(k7);
    full4K = k4, full7K = k7;
    std::string key;
    int val = 0x48;

    for (char c = 'A'; c <= 'Z'; ++c) {
        std::string character(1, c);
        keyMappings[character] = static_cast<int>(c);
    }
    for (char c = '0'; c <= '9'; ++c) {
        std::string character(1, c);
        keyMappings[character] = val++;
    }
    keyMappings["OemCloseBrackets"] = 0xDD;
    keyMappings["OemOpenBrackets"] = 0xDB;
    keyMappings["OemSemicolon"] = 0xBA;
    keyMappings["OemQuestion"] = 0xBF;
    keyMappings["OemQuotes"] = 0xDE;
    keyMappings["OemPeriod"] = 0xBE;
    keyMappings["OemComma"] = 0xBC;
    keyMappings["OemMinus"] = 0X6D;
    keyMappings["OemPipe"] = 0xDC;
    keyMappings["OemPlus"] = 0x6B;
    keyMappings["Space"] = 0x20;

    while (ss4 >> key) { virtual4Keys.push_back(keyMappings[key]); }
    while (ss7 >> key) { virtual7Keys.push_back(keyMappings[key]); }

    if (virtual4Keys.size() != 4) { 
        throw std::invalid_argument("Please set your 4K keys in-game.");
    }
    if (virtual7Keys.size() != 7) {
        throw std::invalid_argument("Please set your 7K keys in-game.");
    }
}


OsuEnv::OsuEnv(const std::string& osuDir, const int& modulesInUse)
{
    std::string fileName, line, k4, k7;
    this->modulesInUse = modulesInUse;
    fs::path rootDir(osuDir);
    size_t idx;

    // Check if the osu game folder exists :: - -                                      - -
    if (fs::exists(rootDir))
    {
        fs::path songsDir = rootDir / "Songs";
        if (fs::is_directory(songsDir)) {
            osuRoot = rootDir;
            osuSong = songsDir;
        }
        else { throw std::invalid_argument("Your songs folder doesn't exist."); }
    }
    else { throw std::invalid_argument("Your Osu root game folder doesn't exist."); }

    // Find osu configuration file in osu directory and parse it :: - -                - -
    for (const auto& entry : fs::directory_iterator(rootDir))
    {
        if (!entry.is_regular_file()) { continue; }

        fs::path filePath = entry.path();
        fileName = filePath.filename().string();
        if (fileName.find("osu!.") || fileName.rfind(".cfg") != fileName.length() - 4) {
            continue;
        }
        if (fileName == "osu!.cfg") { continue; }

        std::ifstream configs(filePath);
        if (!configs.is_open()) {
            throw std::invalid_argument("Unable to open cfg file.");
        }

        while (std::getline(configs, line)) {
            idx = line.find("ManiaLayouts4K");
            if (idx != std::string::npos && idx == 0) { k4 = line.substr(idx + 17); }

            idx = line.find("ManiaLayouts7K");
            if (idx != std::string::npos && idx == 0) { k7 = line.substr(idx + 17); }
        }
    }
    setKeys(k4, k7);
}