#pragma once

#include "player.hpp"

namespace ariel {
    class Game
    {
    private:
        
    public:
        Game(Player p1, Player p2);
        ~Game();

        void playTurn();
        void printLastTurn();
        void playAll();
        void printWiner();
        void printLog();
        void printStats();
    };
}
