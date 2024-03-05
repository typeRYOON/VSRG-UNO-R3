#pragma once
#include "osuenv.hpp"
typedef std::vector<std::vector<std::pair<unsigned int, unsigned int>>> HC;


class ArduinoHandler {
private:
    OsuEnv& env;

public:
    ArduinoHandler(OsuEnv& ref) : env(ref) {}
    void trackActiveKeys(int);
    void runKey(int, std::vector<int>&, HC&, int&, int&);
    void programLoop();
};