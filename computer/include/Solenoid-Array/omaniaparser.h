#ifndef OMANIAPARSER_H
#define OMANIAPARSER_H
#include "beatmap.h"
#include "osuenv.h"
#include <unordered_map>
#include <fstream>
#include <sstream>


class OManiaParser {
private:
    OsuEnv*  env = nullptr;
    void     setStartDelay(HC& hitChart, int keyCount);
    HC       hitObjectParse(std::ifstream& osuFile, int keyCount);
    BeatMap* construct(std::vector<std::vector<std::string>>& allLines,
                       std::ifstream& osuFile);

public:
    OManiaParser(const std::string& osuRoot);
    BeatMap* from_osu(const std::string& osuPath);
};
#endif