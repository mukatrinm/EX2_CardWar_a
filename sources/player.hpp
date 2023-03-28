#pragma once

#include <string>

namespace ariel
{
    class Player
    {
    private:

    public:
        Player(std::string name) {};
        ~Player() {};

        int stacksize() {return 0;};
        int cardesTaken() {return 0;};
    };  
}

