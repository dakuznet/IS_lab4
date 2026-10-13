#include "board.h"

Board::Board()
{
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            grid[i][j] = EMPTY;
    currentPlayer = BLACK;
}

void Board::makeMove(const Move& move)
{
    grid[move.row][move.column] = currentPlayer;
    history.push_back(move);
    currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK;
}

void Board::undoMove()
{
    if (history.empty())
        return;
    Move lastMove = history.back();
    history.pop_back();
    grid[lastMove.row][lastMove.column] = EMPTY;
    currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK;
}

bool Board::isFull() const
{
    return history.size() == SIZE * SIZE;
}

bool Board::checkWin(int row, int column, Player player) const
{
    int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
    for (auto& dir : directions)
    {
        int count = 1;
        for (int i = 1; i < 5; ++i)
        {
            int nr = row + dir[0] * i;
            int nc = column + dir[1] * i;
            if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE && grid[nr][nc] == player)
                count++;
            else
                break;
        }
        if (count >= 5)
            return true;
    }
    return false;
}

void Board::print() const
{
    std::cout << "   ";
    for (int i = 0; i < SIZE; ++i)
        std::cout << (char)('A' + i) << " ";
    std::cout << "\n";
    for (int i = 0; i < SIZE; ++i) {
        std::cout << (i < 9 ? " " : "") << i + 1 << " ";
        for (int j = 0; j < SIZE; ++j) {
            if (grid[i][j] == EMPTY)
                std::cout << ". ";
            else if (grid[i][j] == BLACK)
                std::cout << "X ";
            else
                std::cout << "O ";
        }
        std::cout << "\n";
    }
    std::cout << "-------------------------\n";
}