#include "..\..\include\Keystrokes-Display\osuenv.h"
#include "..\..\include\Keystrokes-Display\keychecker.h"
std::string DR = R"(C:\Users\Esperanza\AppData\Local\osu!)";


int main()
{
    // Create OsuEnv object ::
    OsuEnv* env = nullptr;
    try { env = new OsuEnv(DR); }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    KeyChecker* keyChecker = new KeyChecker(*env);

    keyChecker->trackKeys(7);

    return 0;
}