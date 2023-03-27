#pragma once

#include <string>

namespace ariel
{
    class Player
    {
    private:

    public:
        Player(std::string name);
        ~Player();

        int stacksize();
        int cardesTaken();
    };  
}

