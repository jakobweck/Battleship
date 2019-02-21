#ifndef PLAYER_H
#define PLAYER_H
#include "PlayerManager.h"
#include "BoardMonitor.h"
#include "AttackResult.h"
#include "HitResult.h"
#include <map>
#include <string>
#include <condition_variable>
#include <mutex>
    class PlayerManager;
    class Player
    {
        public:
            BoardMonitor* board; 
            PlayerManager* mgr;
            Player(int n, int ships, PlayerManager* mgr, int num);
            Player(const Player &p2);
            bool fight();
            std::map<std::string, int> getPlayerStats();
            bool isAlive();
            void rez();
            AttackResult recvAttack();
            int playerNumber;
            int livingShips;
        private:
            std::mutex coutLock;
            int rezCount;
            int attacksRecvd;
            int attacksSent;
            int hits;
            int dupeHits;
            int misses;
            int dupeMisses;
    };

#endif