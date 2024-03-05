#include "..\include\arduinohandler.hpp"
#include "..\include\osuenv.hpp"
#include <algorithm>
#include <fstream>


OsuEnv* startUp()
{
    std::string osuDir, modulesInUse = "16";

    // No config file found :: - -                                                     - -
    if (!fs::exists("VSRG-UNO-R3.cfg"))
    {
        std::cout << " :: Enter your osu directory (use \\\\ for backslash): \n";
        std::getline(std::cin, osuDir);
        osuDir.erase(
            std::remove_if(
                osuDir.begin(),
                osuDir.end(),
                [](char c) { return c == '"'; }),
            osuDir.end()
        );
    }
    // Config file found, parse it :: - -                                              - -
    else 
    {
        std::ifstream configs("VSRG-UNO-R3.cfg");
        if (!configs.is_open()) {
            throw std::invalid_argument("Unable to open config file.\n\n");
        }
        std::getline(configs, osuDir);
        std::getline(configs, modulesInUse);
        configs.close();
    }
    return new OsuEnv(osuDir, std::stoi(modulesInUse));
}


int main()
{
    OsuEnv* env = nullptr;
    try   { env = startUp(); }
    catch (const std::invalid_argument& e)
    {
        std::cerr << " :: Error :: " << e.what() << std::endl;
        return -1;
    }
    ArduinoHandler* handler = new ArduinoHandler(*env);
    handler->programLoop();

    return 0;
}