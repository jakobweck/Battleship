#ifndef BOARDMON_H
#define BOARDMON_H
#include <condition_variable>
#include <mutex>
#include "HitResult.h"
    class BoardMonitor
    {
        public:
            BoardMonitor(int n, int m);
            int* getAttackLocation();
            HitResult recvAttack(int x, int y);
            int ships;
            int size;
            char** board;
        private:
            std::mutex methodLock;

            
    };
#endif
