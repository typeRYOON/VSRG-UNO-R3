#ifndef BEATMAP_H
#define BEATMAP_H
#include <condition_variable>
#include <Windows.h>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <tuple>
typedef std::vector<std::vector<std::pair<unsigned int, unsigned int>>> HC;


class BeatMap
{
private:
    HC hitChart;
    std::string title, artist, creator, version;
    unsigned short keyCount;

public:
    BeatMap(std::vector<std::string>& metadata, int keys, HC hitChart);
    std::vector<int> virtualKeys;
    bool hitChartIsEmpty() { return hitChart.empty(); };
    void play();
};
#endif