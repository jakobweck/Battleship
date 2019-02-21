#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H
#include "Player.h"
#include "PlayersAlive.h"  
#include <vector>
#include <condition_variable>
#include <mutex>
    class Player;
    class PlayerManager{
        public:
            int numPlayers;
            int playersCreated;
            int boardSize;
            std::condition_variable suspendedQueue;
            std::mutex suspendedMutex;
            std::vector<Player*> players;
            PlayerManager(int players, int n);
            Player* getLivingOpponentFor(int attackerNum);
            Player* createPlayer(int n, int m);
            bool doRez();
            PlayersAlive getSurvivingPlayers();
            void printReport(char** board1, char** board2);
            int getAndIncCreatedPlayers();
            void printBoard(char** board);

    };
#endif