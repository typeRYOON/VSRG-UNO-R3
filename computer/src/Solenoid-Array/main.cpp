#include "..\..\include\Solenoid-Array\osuenv.h"
#include "..\..\include\Solenoid-Array\omaniaparser.h"
#include <algorithm>
std::string DR = R"(C:\Users\Ryoon\AppData\Local\osu!)";


int main()
{
    OManiaParser* parser = nullptr;
    try { parser = new OManiaParser(DR); }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Error :: " << e.what() << std::endl;
        return -1;
    }
    BeatMap* mp = nullptr;
    std::string mapPath;
    while (true) {
        std::cout << " Enter the full .osu path:\n ";
        std::getline(std::cin, mapPath);
        if (mapPath == "e") { break; }

        mapPath.erase(
            std::remove_if(
                mapPath.begin(), 
                mapPath.end(),
                [](char c) { return c == '"'; }), 
            mapPath.end()
        );

        mp = parser->from_osu(mapPath);
        if (mp == nullptr) {
            std::cerr << "Error :: Bad read on map file." << '\n';
            std::cerr << "      :: " << mapPath << '\n';
            continue; 
        }
        mp->play();
        delete mp;
    }
    return 0;
}
