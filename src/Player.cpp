
#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <string>
#include <thread>
#include "Player.h"
#include "AttackResult.h"
#include "PlayerManager.h"
using namespace std;

Player::Player(int n, int ships, PlayerManager* mgr, int num){
    board = new BoardMonitor(n, ships);
    this->mgr = mgr;
    playerNumber = num;
    livingShips = ships;
    (mgr->players).push_back(this);
}
Player::Player(const Player &p2){
    board = p2.board;
    mgr = p2.mgr;
    playerNumber = p2.playerNumber;
}

bool Player::isAlive(){
    return (livingShips>0);
}

void Player::rez(){
    board = new BoardMonitor(board->size, 2);
    livingShips = 2;
    rezCount++;
}

AttackResult Player::recvAttack(){
    //get unattacked attack location
    int* coords = board->getAttackLocation();
    AttackResult res;
    res.x = coords[0];
    res.y = coords[1];
    HitResult hitRes = board->recvAttack(res.x, res.y);
    if(hitRes == Hit){
        livingShips--;
    }
    res.hitType = hitRes;
    attacksRecvd++;
    return res;
}

bool Player::fight(){
    std::thread::id this_id = std::this_thread::get_id();
    while(true){
        if(this->isAlive()){
            Player* target = mgr->getLivingOpponentFor(playerNumber);
            cout << this_id << "Got target" << endl;
            //no living targets
            if(target==nullptr){
                cout << this_id << "Could not get target" << endl;
                if(this->isAlive())
                {
                    cout << "Winner found all other players dead" << endl;
                     return true; //winner
                }
                else return false; //all dead
            }
            AttackResult res = target->recvAttack();
            attacksSent++;
            HitResult hitRes = res.hitType;
            switch(hitRes){
                case Hit:
                {
                    hits++;
                    break;
                }
                case Miss:
                {
                    misses++;
                    break;
                }
                case SecondHit:
                {
                    dupeHits++;
                    break;
                }
                case SecondMiss:
                {
                    dupeMisses++;
                    break;
                }
            }
        }
        if(!this->isAlive()){
            //wait on cond variable here
          //  std::unique_lock<std::mutex> lk(mgr->suspendedMutex);
         //   (mgr->suspendedQueue).wait(lk);
            std::cout<<"Passed cond var"<<endl;
            if(mgr->getSurvivingPlayers() == None || mgr->getSurvivingPlayers() == One){
                if(this->isAlive()) 
                {
                    std::cout << "Winner only one alive from suspended animation" << endl;
                    return true; //winner
                }
                else return false; //dead
            }
            
        }
    }
}

map<string, int> Player::getPlayerStats(){
    map<string, int> stats;
    stats.insert(pair<string,int>("Ships Alive",livingShips));
    stats.insert(pair<string,int>("Times Resurrected",rezCount));
    stats.insert(pair<string,int>("Attacks Taken",attacksRecvd));
    stats.insert(pair<string,int>("Attacks Sent",attacksSent));
    stats.insert(pair<string,int>("Hits Made",hits));
    stats.insert(pair<string,int>("Repeat Hits Made",dupeHits));
    stats.insert(pair<string,int>("Misses Made",misses));
    stats.insert(pair<string,int>("Repeat Misses Made",dupeMisses));

    return stats;
}