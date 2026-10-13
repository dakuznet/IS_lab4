#pragma once

#include "types.h"
#include <vector>
#include <iostream>

class Board 
{
public:
    int grid[SIZE][SIZE];
    std::vector<Move> history;
    Player currentPlayer;

    Board();
    void makeMove(const Move& move);
    void undoMove();
    bool isFull() const;
    bool checkWin(int row, int column, Player player) const;
    void print() const;
};