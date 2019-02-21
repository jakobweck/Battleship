#include <iostream>
#include "BoardMonitor.h"
#include "HitResult.h"
#include "Player.h"
#include "PlayerManager.h"
#include "PlayerResult.h"
#include <thread>
#include <pthread.h>
#include <vector>
#include <future>
#include <mutex>
#include <stdio.h>
#include <iterator>

bool go = false;
bool printFullReport;
char** board1;
char** board2;
std::mutex pNumLock;
std::mutex boardAssignmentLock;

char** copyBoard(char** b, int size){
    char** ret = new char*[size];
    for(int i =0;i<size;i++){
        ret[i] = new char[size];
        for(int j =0;j<size;j++){
            ret[i][j] = b[i][j];
        }
    }
    return ret;
}
PlayerResult prepareFight(Player* p){
    while(!go){

    }
    bool winner =  p->fight();
    PlayerResult res;
    res.isWinner = (winner);
    res.p = p;
    return res;
}
Player* createPlayer(PlayerManager* mngr, int n, int ships){
    //todo this call needs to be threadsafe to guarantee unique sequential player nums
    pNumLock.lock();
    int playerNum = mngr->getAndIncCreatedPlayers();
    Player* p = new Player(n, ships, mngr, playerNum);
    pNumLock.unlock();
    //save initial boards for printing at the end
    boardAssignmentLock.lock();
    if(printFullReport && p->playerNumber==0)
    {
        board1 = copyBoard(p->board->board, n);
        mngr->printBoard(board1);
    }
    else if(printFullReport && p->playerNumber==1){
        board2 = copyBoard(p->board->board, n);
        mngr->printBoard(board2);
    }
    boardAssignmentLock.unlock();
    return p;
}
int main(int argc, char *argv[])
{
    srand(time(NULL));
    int playerCount = atoi(argv[1]);
    int n = atoi(argv[2]);
    int ships = atoi(argv[3]);
    if(playerCount<2 || n<0 || ships<0 || ships>(n*n)){
        std::cout << "Invalid parameters" << std::endl;
        return 1;
    }
    printFullReport = (playerCount==2 && n<=40);
    PlayerManager* mngr = new PlayerManager(playerCount, n);
    std::vector<std::future<Player*>> playerInitThreads;
    std::vector<Player*> players;
    for(int i =0;i<playerCount;i++)
    {
        playerInitThreads.push_back(std::async(createPlayer, mngr, n, ships));
    }

    for(auto &thread : playerInitThreads)
    {
        std::cout << "waiting player init thread" <<std::endl;
        thread.wait();
        std::cout << "joined player init thread" << std::endl;
        Player* p = thread.get();
        players.push_back(p);
    }

    std::vector<std::future<PlayerResult>> playerFightThreads;
    for(int i =0;i<playerCount;i++)
    {
        playerFightThreads.push_back(std::async(std::launch::async, prepareFight, players[i]));
        std::cout << "Created player fight thread" << std::endl;
    }
    go = true;
    int winner = -1;
    std::vector<PlayerResult> results;
    for(auto &thread : playerFightThreads){
        std::cout << "Waiting fight thread" << std::endl;
        thread.wait();
        PlayerResult res = thread.get();
        results.push_back(res);
    }
    for(PlayerResult res : results){
        if(res.isWinner && res.p->livingShips>0){
            winner = res.p->playerNumber+1;
        }
    }

    if(winner!=-1) std::cout << "Player " << winner << " wins." << std::endl;
    else std::cout << "Tie!" << std::endl;
    
    mngr->printReport(board1, board2);
    return 0;
}

