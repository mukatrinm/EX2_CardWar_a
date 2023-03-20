#pragma once

#include <string>
class Player
{
private:

public:
    Player(std::string name);
    ~Player();

    int stacksize();
    int cardesTaken();
};
