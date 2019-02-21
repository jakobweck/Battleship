#include <iostream>
#include <stdio.h>
#include "Player.h"
#include "PlayerManager.h"
#include "PlayersAlive.h"
using namespace std;
    PlayerManager::PlayerManager(int playerNum, int n){
        numPlayers = playerNum;
        boardSize = n;
        playersCreated = 0;
    }
    int PlayerManager::getAndIncCreatedPlayers(){
        return playersCreated++;
    }
    Player* PlayerManager::getLivingOpponentFor(int attackerNum){
        Player* chosenPlayer;
        bool found = false;
        bool rezFound = false;
        int randomIndex = rand() % players.size();
        unsigned int playersSeen = 0;
        bool rez = doRez();
        while(!found)
        {
            Player* currPlayer = players[randomIndex];
            playersSeen++;
            if(currPlayer->isAlive() && currPlayer->playerNumber!=attackerNum)
            {
                chosenPlayer = currPlayer;
                found = true;
            }
            randomIndex = (randomIndex+1) % players.size();
            //made it back around, all players are dead or current player
            if(playersSeen > players.size()){
                chosenPlayer = nullptr;
                break; //don't rez if can't find a target?
            }
        }
        if(rez)
        {
            //notify all because they'll reenter queue if still dead or can't find a target_
            unsigned int rezzesTried = 0;
            int rezIndex = rand()%players.size();
            while(!rezFound){
                Player* rezPlayer = players[rezIndex];
                rezzesTried++;
                if(!rezPlayer->isAlive() && rezPlayer->playerNumber!=attackerNum){
                    rezPlayer->rez();
                    suspendedQueue.notify_all(); //here?
                }
                rezIndex = (rezIndex+1)%players.size();
                if(rezzesTried > players.size()){
                    break;
                }
            }

        }
        
        return chosenPlayer;
    }
    bool PlayerManager::doRez(){
        //1 in 10 chance to return true before any other condition
        int num = rand()%10;
        unsigned int playersUp = 0;
        //check whether less than half of players are still alive
        for(Player* p : players){
            if(p->isAlive()) playersUp++;
        }
        return(num==1 && (playersUp<(players.size()/2)) && (playersUp>1));
    }

    PlayersAlive PlayerManager::getSurvivingPlayers(){
        bool onePlayerUp = false;
        bool noPlayersUp = true;
        for(Player* p : players)
        {
            if(p->isAlive())
            {
                if(onePlayerUp)
                {
                    onePlayerUp = false;
                }
                else onePlayerUp = true;
                noPlayersUp = false;
            }
        }
        if(noPlayersUp) return None;
        else if (onePlayerUp) return One;
        else return Multiple;
    }

    void PlayerManager::printReport(char** board1, char** board2){
        if(numPlayers==2 && boardSize<=40){
            std::cout << "Player 1's initial board:\n\n";
            printBoard(board1);
            std::cout << "Player 2's initial board:\n\n";
            printBoard(board2);
            std::cout << "Player 1's final board:\n\n";
            printBoard(players[0]->board->board);
            std::cout << "Player 2's final board:\n\n";
            printBoard(players[1]->board->board);
        }
        int playerIndex = 1;
        for(Player* p : players){
            std::cout << "Player " << playerIndex << "'s stats:\n";
            std::map<std::string, int> stats = p->getPlayerStats();
            for(auto elem : stats)
            {
                std::cout << elem.first << " : " << elem.second << "\n";
            }
            playerIndex++;
            std::cout << "\n";
        }
    }

    void PlayerManager::printBoard(char** board){
        for(int i =0;i<boardSize;i++){
            for(int j =0;j<boardSize;j++){
                std::cout << board[i][j] << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
    }