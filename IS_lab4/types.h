#pragma once

const int SIZE = 15;

enum Player 
{
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

struct Move 
{
    int row, column, score;

    bool operator==(const Move& other) const 
    {
        return row == other.row && column == other.column;
    }

    bool operator<(const Move& other) const 
    {
        return row < other.row || (row == other.row && column < other.column);
    }
};