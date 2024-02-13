#include "osuenv.h"


std::vector<int> OsuEnv::getVirtualKeys(const int keyCount)
{
    if (keyCount == 4) { return virtual4Keys; }
    if (keyCount == 7) { return virtual7Keys; }
    std::cerr << "Please only play 4K or 7K maps." << std::endl;
    return {};  // FIX THIS, IF NO KEY COUNTS ARE FOUND IN osu!.USER.cfg, THROW ERROR!
}


void OsuEnv::setKeys(std::string& k4, std::string& k7)
{
    std::unordered_map<std::string, int> keyMappings;
    std::istringstream ss4(k4);
    std::istringstream ss7(k7);
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


OsuEnv::OsuEnv(const std::string& rd)
{
    size_t idx;
    fs::path rootDir(rd);
    std::string fileName, line, k4, k7;

    if (fs::exists(rootDir)) {
        fs::path songsDir = rootDir / "Songs";

        if (fs::is_directory(songsDir)) {
            osuRoot = rootDir;
            osuSong = songsDir;
        }
        else { throw std::invalid_argument("Your songs folder doesn't exist."); }
    }
    else { throw std::invalid_argument("Your Osu root game folder doesn't exist."); }

    for (const auto& entry : fs::directory_iterator(rootDir)) {
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