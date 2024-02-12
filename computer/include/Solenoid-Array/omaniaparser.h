#ifndef OMANIAPARSER_H
#define OMANIAPARSER_H
#include "beatmap.h" // Resultant container.
#include "osuenv.h"  // Link enviroment object.
#include <unordered_map>
#include <fstream>
#include <sstream>


class OManiaParser
{
private:
    OsuEnv&  env;
    void     setStartDelay(HC& hitChart, int keyCount);
    HC       hitObjectParse(std::ifstream& osuFile, int keyCount);
    BeatMap* construct(std::vector<std::vector<std::string>>& allLines,
                       std::ifstream& osuFile);

public:
    OManiaParser(OsuEnv& ref) : env(ref) {}
    BeatMap* from_osu(const std::string& osuPath);
};
#endif