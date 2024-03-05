#include "omaniaparser.hpp"


void OManiaParser::setStartDelay(HC& hitChart, const int keyCount)
{
    unsigned int earliest = 1 << 31;
    for (int i = 0; i < keyCount; ++i) {
        if (hitChart[i][0].first < earliest) { earliest = hitChart[i][0].first; }
    }

    for (int i = 0; i < keyCount; ++i) {
        hitChart[hitChart.size() - 1].push_back(
            std::make_pair(hitChart[i][0].first - earliest, 0)
        );
    }
}

HC OManiaParser::hitObjectParse(std::ifstream& osuFile, int keyCount)
{
    HC hitChart(keyCount + 1);
    unsigned int holdTime, delayTime;
    unsigned int column, time, n;
    std::string line, slice;

    while (std::getline(osuFile, line)) {
        std::istringstream lineStream(line);

        // X/Column calculation:
        std::getline(lineStream, slice, ',');
        column = std::stoi(slice) * keyCount / 0x200;

        // Dummy Y:
        std::getline(lineStream, slice, ',');

        // Time:
        std::getline(lineStream, slice, ',');
        time = std::stoi(slice);

        // Dummies:
        std::getline(lineStream, slice, ',');
        std::getline(lineStream, slice, ',');

        // Endtime:
        std::getline(lineStream, slice, ',');
        hitChart[column].push_back(std::make_pair(time, std::stoi(slice)));
    }
    osuFile.close();
    setStartDelay(hitChart, keyCount);

    for (int i = 0; i < keyCount; ++i) {
        n = hitChart[i].size() - 1;

        for (unsigned int j = 0; j < n; ++j) {
            holdTime  = hitChart[i][j].second == 0 ? 15 : hitChart[i][j].second - hitChart[i][j].first;
            delayTime = hitChart[i][j + 1].first - hitChart[i][j].first - holdTime;
            if (hitChart[i][j + 1].first < (hitChart[i][j].first + holdTime)) {
                std::cerr << "Overlapping note on column: " << i;
                std::cerr << ", at time(ms): " << hitChart[i][j + 1].first << std::endl;
                return {};
            }
            hitChart[i][j].first = holdTime;
            hitChart[i][j].second = delayTime;
        }
        holdTime = hitChart[i][n].second == 0 ? 15 : hitChart[i][n].second - hitChart[i][n].first;
        hitChart[i][n].first = holdTime;
        hitChart[i][n].second = 0;
    }
    return hitChart;
}

BeatMap* OManiaParser::construct(std::vector<std::vector<std::string>>& allLines,
                                 std::ifstream& osuFile)
{
    std::vector<std::string> metadata(4);
    int keyCount = -1, mode = -1;
    size_t idx;

    // [General] std::string vector loop:
    for (auto& line : allLines[0]) {
        idx = line.find("Mode:");
        if (idx != std::string::npos && idx == 0)
            mode = std::stoi(line.substr(idx + 5));
    }

    // Osu mania mode 3 required:
    if (mode != 3) {
        std::cerr << "Wrong mode passed (not 3): " << mode << std::endl;
        return nullptr;
    }

    // [Metadata] std::string vector loop:
    for (auto& line : allLines[2]) {
        idx = line.find("TitleUnicode:");
        if (idx != std::string::npos && idx == 0) metadata[0] = line.substr(idx + 13);

        idx = line.find("Version:");
        if (idx != std::string::npos && idx == 0) metadata[1] = line.substr(idx + 8);

        idx = line.find("ArtistUnicode:");
        if (idx != std::string::npos && idx == 0) metadata[2] = line.substr(idx + 14);

        idx = line.find("Creator:");
        if (idx != std::string::npos && idx == 0) metadata[3] = line.substr(idx + 8);
    }

    // [Difficulty] std::string vector loop:
    for (auto& line : allLines[3]) {
        idx = line.find("CircleSize:");
        if (idx != std::string::npos && idx == 0) {
            keyCount = std::stoi(line.substr(idx + 11));
            break;
        }
    }
    BeatMap* ret = new BeatMap(metadata, keyCount, hitObjectParse(osuFile, keyCount));
    if (ret->hitChartIsEmpty()) {
        delete ret;
        return nullptr;
    }
    ret->virtualKeys = env.getVirtualKeys(keyCount);
    return ret;
}


BeatMap* OManiaParser::from_osu(const std::string& osuPath)
{
    std::ifstream osuFile(osuPath);

    if (!osuFile.is_open()) {
        throw std::invalid_argument("Unable to open the file.");
    }

    std::vector<std::vector<std::string>> allLines(6);
    std::unordered_map<std::string, int> mapping;
    mapping["[TimingPoints]"] = 5;
    mapping["[HitObjects]"] = 6;
    mapping["[Difficulty]"] = 3;
    mapping["[Metadata]"] = 2;
    mapping["[Events]"] = 4;
    mapping["[Editor]"] = 1;

    // Retrieve contents into vectors:
    int x = 0;
    std::string line;
    std::vector<std::string> curV;
    for (int i = 0; i < 3; ++i) std::getline(osuFile, line);
    while (std::getline(osuFile, line)) {
        if (!mapping.count(line)) {
            curV.push_back(line);
            continue;
        }
        allLines[x] = curV;
        x = mapping[line];
        if (x == 6) { break; }
        curV.clear();
    }
    return construct(allLines, osuFile);
}