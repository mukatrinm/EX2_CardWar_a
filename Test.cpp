#include "doctest.h"

#include "sources/card.hpp"
#include "sources/game.hpp"
#include "sources/player.hpp"

using namespace ariel;

/**
 * if we run out of cards, we turn our collected cards face down , shufflle and continue playing.
 * 
 * identical cards of same rank (war), we play 1 cards facedown staggered on top of the face-up card 
 * then each plaver flips one card face-up
 * * the player with the higher rank wins all the cards in the war including the facedown
 * * if they tie again, another ward
 * * if the player doesn't have cards to place down because they ran out of cards in their deck and discard then they lose
 *   the war the first player to collect all the cards in the deck wins.
 */

TEST_CASE("Initial Values") {
    Player p1("Alice");
    Player p2("Bob");

    Game game(p1,p2);
    
    CHECK_EQ(p1.stacksize(), 26);
    CHECK_EQ(p2.stacksize(), 26);

    CHECK_EQ(p1.cardesTaken(), 0);
    CHECK_EQ(p2.cardesTaken(), 0);

    // if we restart the game after play turn, players should get 26 cards.
    game.playTurn();
    Game new_game(p1,p2);
    CHECK_EQ(p1.stacksize(), 26);
    CHECK_EQ(p2.stacksize(), 26);
    CHECK_EQ(p1.cardesTaken(), 0);
    CHECK_EQ(p2.cardesTaken(), 0);

    // if we restart the game after playing all, players should get 26 cards.
    game.playAll();
    Game new_game_2(p1,p2);
    CHECK_EQ(p1.stacksize(), 26);
    CHECK_EQ(p2.stacksize(), 26);
    CHECK_EQ(p1.cardesTaken(), 0);
    CHECK_EQ(p2.cardesTaken(), 0);
}

TEST_CASE("check cards taken") {
    Player p1("Alice");
    Player p2("Bob");
    Game game(p1,p2);
    bool valid_cards_taken = false;

    SUBCASE("Play One Turn") {
        game.playTurn();

        bool player_1_took_the_cards = p1.cardesTaken() > 0 && p2.cardesTaken() == 0;
        bool player_2_took_the_cards = p2.cardesTaken() > 0 && p1.cardesTaken() == 0;
        bool tie = p1.cardesTaken() == 26 && p2.cardesTaken() == 26;

        valid_cards_taken = player_1_took_the_cards || player_2_took_the_cards || tie;
        CHECK(valid_cards_taken);

        // player should take no more than all the cards available in the game
        CHECK_LE(p1.cardesTaken(), 26 * 2);
        CHECK_LE(p2.cardesTaken(), 26 * 2);
    }

    SUBCASE("Play Multiple Turns") {
        for (int i=0;i<5;i++) {
            game.playTurn();
        }

        bool player_1_took_cards = p1.cardesTaken() > 0;
        bool player_2_took_cards = p2.cardesTaken() > 0;

        valid_cards_taken = player_1_took_cards || player_2_took_cards;
        CHECK(valid_cards_taken);

        // player should take no more than all the cards available in the game
        CHECK_LE(p1.cardesTaken(), 26 * 2);
        CHECK_LE(p2.cardesTaken(), 26 * 2);
    }

    SUBCASE("Play All") {
        game.playAll();
        bool player_1_took_cards = p1.cardesTaken() > 0;
        bool player_2_took_cards = p2.cardesTaken() > 0;

        valid_cards_taken = player_1_took_cards || player_2_took_cards;         
        CHECK(valid_cards_taken);

        // player should take no more than all the cards available in the game
        CHECK_LE(p1.cardesTaken(), 26 * 2);
        CHECK_LE(p2.cardesTaken(), 26 * 2);
    }
}

TEST_CASE("check stacks update") {
    Player p1("Alice");
    Player p2("Bob");
    Game game(p1,p2);
    bool players_stack_update = false;
    bool end_turn_condition = false;
    bool normal_turn_condition = false;
    bool last_war_condition = false;

    SUBCASE("Play One Turn") {
        game.playTurn();

        bool player_1_stack_updates = p1.stacksize() < 26;
        bool player_2_stack_updates = p2.stacksize() < 26;

        players_stack_update = player_1_stack_updates && player_2_stack_updates;
        
        CHECK(players_stack_update);
        CHECK_GE(p1.stacksize(), 0);
        CHECK_GE(p2.stacksize(), 0);

        // edge case if one of the players has 3 cards left and the other has 2 or 1 card
        // left and they get a war, then one of the players will have 1 card left
        // while the other player has no cards.
        normal_turn_condition = p1.stacksize() == p2.stacksize();
        last_war_condition = (p1.stacksize() <= 2 && p2.stacksize() == 0) || (p2.stacksize() <= 2 && p1.stacksize() == 0);
        end_turn_condition = normal_turn_condition || last_war_condition;
        CHECK(end_turn_condition);

        // new_stacksize = prev_sacksize - total cards thrown
        if (p1.cardesTaken() != 26 || p2.cardesTaken() != 26) {
            CHECK(p1.stacksize() == 26 - (p1.cardesTaken() + p2.cardesTaken()));
        }
    }

    SUBCASE("Play Multiple Turns") {
        for (int i=0;i<5;i++) {
            game.playTurn();
        }

        bool player_1_stack_updates = p1.stacksize() <= 21;
        bool player_2_stack_updates = p2.stacksize() <= 21;

        players_stack_update = player_1_stack_updates && player_2_stack_updates;
        
        CHECK(players_stack_update);
        CHECK_GE(p1.stacksize(), 0);
        CHECK_GE(p2.stacksize(), 0);

        // edge case if one of the players has 3 cards left and the other has 2 or 1 card
        // left and they get a war, then one of the players will have 1 card left
        // while the other player has no cards.
        normal_turn_condition = p1.stacksize() == p2.stacksize();
        last_war_condition = (p1.stacksize() <= 1 && p2.stacksize() == 0) || (p2.stacksize() <= 1 && p1.stacksize() == 0);
        end_turn_condition = normal_turn_condition || last_war_condition;
        CHECK(end_turn_condition);
    }

    SUBCASE("Play All") {
        game.playAll();
        bool player_1_stack_updates = p1.stacksize() == 0;
        bool player_2_stack_updates = p2.stacksize() == 0;

        players_stack_update = player_1_stack_updates && player_2_stack_updates;
        
        CHECK(players_stack_update);
        CHECK_GE(p1.stacksize(), 0);
        CHECK_GE(p2.stacksize(), 0);

        // edge case if one of the players has 3 cards left and the other has 2 or 1 card
        // left and they get a war, then one of the players will have 1 card left
        // while the other player has no cards.
        normal_turn_condition = p1.stacksize() == 0 && p2.stacksize() == 0;
        last_war_condition = (p1.stacksize() <= 2 && p2.stacksize() == 0) || (p2.stacksize() <= 2 && p1.stacksize() == 0);
        end_turn_condition = normal_turn_condition || last_war_condition;
        CHECK(end_turn_condition);
    }
}

// performance check, games shouldn't take longer than 10 seconds (ik this is a long time...)
TEST_CASE("time limit" * doctest::timeout(10)) {
    Player p1("Alice");
    Player p2("Bob");
    Game game(p1,p2);
    
    SUBCASE("play Turn") {
        game.playTurn();
    }

    SUBCASE("play All") {
        game.playAll();
    }
}

// here we have a higher chance of Wars, to check if game ends properly.
// https://math.stackexchange.com/questions/1562945/probability-of-getting-war-in-a-game-of-war-card-game
TEST_CASE("Play a Large Number Of Games") {
    Player p1("Alice");
    Player p2("Bob");

    bool player_1_stack_updates = false;
    bool player_2_stack_updates = false;
    bool players_stack_update = false;

    bool player_1_took_the_cards = false;
    bool player_2_took_the_cards = false;
    bool valid_cards_taken = false;

    SUBCASE("doesn't matter when war happens") {
        for (int i=0;i<2000;i++) {
            Game game(p1,p2);
            game.playAll();

            CHECK_NOTHROW(p1.stacksize());
            CHECK_NOTHROW(p2.stacksize());
            CHECK_NOTHROW(p1.cardesTaken());
            CHECK_NOTHROW(p2.cardesTaken());
            CHECK_NOTHROW(game.printLastTurn());
            CHECK_NOTHROW(game.printWiner());
            CHECK_NOTHROW(game.printLog());
            CHECK_NOTHROW(game.printStats());

            bool player_1_took_cards = p1.cardesTaken() > 0;
            bool player_2_took_cards = p2.cardesTaken() > 0;
            valid_cards_taken = player_1_took_cards || player_2_took_cards; 
            CHECK(valid_cards_taken);

            bool player_1_stack_updates = p1.stacksize() == 0;
            bool player_2_stack_updates = p2.stacksize() == 0;
            players_stack_update = player_1_stack_updates && player_2_stack_updates;
            CHECK(players_stack_update);

            // player should take no more than all the cards available in the game
            CHECK_LE(p1.cardesTaken(), 26 * 2);
            CHECK_LE(p2.cardesTaken(), 26 * 2);
        }
    }
}

// for each function (playAll time and so on...)
TEST_CASE("Exceptions Test") {
    Player p1("Alice");
    Player p2("Bob");
    Game game(p1,p2);

    SUBCASE("initial") {
        CHECK_NOTHROW(p1.stacksize());
        CHECK_NOTHROW(p2.stacksize());
        CHECK_NOTHROW(p1.cardesTaken());
        CHECK_NOTHROW(p2.cardesTaken());
        CHECK_NOTHROW(game.printLastTurn());
        CHECK_NOTHROW(game.printWiner());
        CHECK_NOTHROW(game.printLog());
        CHECK_NOTHROW(game.printStats());
    }

    SUBCASE("One Turn") {
        game.playTurn();
        CHECK_NOTHROW(p1.stacksize());
        CHECK_NOTHROW(p2.stacksize());
        CHECK_NOTHROW(p1.cardesTaken());
        CHECK_NOTHROW(p2.cardesTaken());
        CHECK_NOTHROW(game.printLastTurn());
        CHECK_NOTHROW(game.printWiner());
        CHECK_NOTHROW(game.printLog());
        CHECK_NOTHROW(game.printStats());
    }

    SUBCASE("Multiple Turns") {
        for (int i=0;i<5;i++) {
            game.playTurn();
        }
        CHECK_NOTHROW(p1.stacksize());
        CHECK_NOTHROW(p2.stacksize());
        CHECK_NOTHROW(p1.cardesTaken());
        CHECK_NOTHROW(p2.cardesTaken());
        CHECK_NOTHROW(game.printLastTurn());
        CHECK_NOTHROW(game.printWiner());
        CHECK_NOTHROW(game.printLog());
        CHECK_NOTHROW(game.printStats());
    }
    
    SUBCASE("play All") {
        game.playAll();
        CHECK_NOTHROW(p1.stacksize());
        CHECK_NOTHROW(p2.stacksize());
        CHECK_NOTHROW(p1.cardesTaken());
        CHECK_NOTHROW(p2.cardesTaken());
        CHECK_NOTHROW(game.printLastTurn());
        CHECK_NOTHROW(game.printWiner());
        CHECK_NOTHROW(game.printLog());
        CHECK_NOTHROW(game.printStats());
    }

    SUBCASE("multiple Play All") {
        for (int i=0;i<5;i++) {
            game.playAll();
        }
        CHECK_NOTHROW(p1.stacksize());
        CHECK_NOTHROW(p2.stacksize());
        CHECK_NOTHROW(p1.cardesTaken());
        CHECK_NOTHROW(p2.cardesTaken());
        CHECK_NOTHROW(game.printLastTurn());
        CHECK_NOTHROW(game.printWiner());
        CHECK_NOTHROW(game.printLog());
        CHECK_NOTHROW(game.printStats());
    }

    SUBCASE("Play Turn Then Play All") {
        game.playTurn();
        game.playAll();
        
        CHECK_NOTHROW(p1.stacksize());
        CHECK_NOTHROW(p2.stacksize());
        CHECK_NOTHROW(p1.cardesTaken());
        CHECK_NOTHROW(p2.cardesTaken());
        CHECK_NOTHROW(game.printLastTurn());
        CHECK_NOTHROW(game.printWiner());
        CHECK_NOTHROW(game.printLog());
        CHECK_NOTHROW(game.printStats());
    }

    SUBCASE("Play All Then Play All Turns") {
        game.playAll();
        game.playTurn();

        CHECK_NOTHROW(p1.stacksize());
        CHECK_NOTHROW(p2.stacksize());
        CHECK_NOTHROW(p1.cardesTaken());
        CHECK_NOTHROW(p2.cardesTaken());
        CHECK_NOTHROW(game.printLastTurn());
        CHECK_NOTHROW(game.printWiner());
        CHECK_NOTHROW(game.printLog());
        CHECK_NOTHROW(game.printStats());
    }

    SUBCASE("Player With Same Name") {
        CHECK_NOTHROW(Player("Bob"));
    }

    SUBCASE("Player Can't Play With Himself") {
        CHECK_THROWS(Game(p1,p1));
    }

    SUBCASE("Player in One Game Only") {
        Player p3("Bob");
        CHECK_THROWS(Game(p1,p3));
    }
}