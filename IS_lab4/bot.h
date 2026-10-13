#pragma once
#include "board.h"
#include <chrono>

struct SearchState 
{
    std::chrono::steady_clock::time_point start_time;
    int timeLimitMS = 5;
    bool timeUp = false;
    Move bestMove;
    int bestScore;
};

Move getBestMove(Board& board, Player myColor, Player opponentColor);