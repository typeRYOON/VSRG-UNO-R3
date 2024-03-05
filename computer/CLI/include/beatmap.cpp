#include "beatmap.hpp"


HC BeatMap::getHC()
{
    return hitChart;
}


BeatMap::BeatMap(std::vector<std::string>& metadata, int keys, HC hitChart)
{
    title = metadata[0];
    artist = metadata[2];
    creator = metadata[3];
    version = metadata[1];
    keyCount = keys;
    this->hitChart = hitChart;
}