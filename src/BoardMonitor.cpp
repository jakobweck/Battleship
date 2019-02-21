#include "BoardMonitor.h"

    BoardMonitor::BoardMonitor(int n, int m){
        board = new char*[n];
        for(int i = 0; i < n; i++)
        {
            board[i] = new char[n];
            for(int j=0;j<n;j++){
                board[i][j]= '_';
            }
        }
        size = n;
        //set M random locations to 0 (ships)
        //if the randomly-generated location has a ship already, increment till we find a space
        for(int i=0;i<m;i++){
            int x = rand() % n;
            int y = rand() % n;
            char currChar = board[x][y];
            while(currChar == '0'){
                if(y==(n-1)) x = (x+1) % n;
                y = (y+1)%n;
                currChar = board[x][y];
            }
            board[x][y] = '0';
        }
        ships = m;

    }
    int* BoardMonitor::getAttackLocation(){
        std::unique_lock<std::mutex> ml(methodLock);
        char currSquare = '*';
        int* res = new int[2];
        int x;
        int y;
        while(currSquare == '*' || currSquare=='.'){
            x = rand()%size;
            y = rand()%size;
            currSquare = board[x][y];
        }
        res[0] = x;
        res[1] = y;
        return res;
    }

    HitResult BoardMonitor::recvAttack(int x, int y){
        std::unique_lock<std::mutex> ml(methodLock);
        switch(board[x][y])
        {
            case '0':
            {
                board[x][y] = '*';
                return Hit;
            }
            case '_':
            {
                board[x][y] = '.';
                return Miss;
            }
            case '*':
            {
                return SecondHit;
            }
            case '.':
            {
                return SecondMiss;
            }
            default:
            {
                return Miss;
            }
        }
    }