#include <iostream>
#include <string>
#include <cctype>
#include "types.h"
#include "board.h"
#include "bot.h"

Move parseMove(const std::string& token) 
{
    if (token.empty() || token.length() < 2)
        return { -1, -1, 0 };

    char col_char = std::tolower(token[0]);
    int row = 0;

    for (size_t i = 1; i < token.length(); ++i) 
    {
        if (std::isdigit(token[i]))
            row = row * 10 + (token[i] - '0');
    }

    if (col_char < 'a' || col_char > 'o' || row < 1 || row > 15)
        return { -1, -1, 0 };

    int col = col_char - 'a';
    row--;

    return { row, col, 0 };
}

std::string formatMove(const Move& m) 
{
    char col = 'a' + m.column;
    int row = m.row + 1;
    return std::string() + col + std::to_string(row);
}

int tryUndo(const std::string& token, Board& board) 
{
    if (token.empty() || std::tolower(token[0]) != 'u')
        return 0;

    int undoCount = 1;
    if (token.length() > 1 && std::isdigit(token[1]))
        undoCount = token[1] - '0';

    int actualUndone = 0;
    for (int i = 0; i < undoCount && !board.history.empty(); ++i) 
    {
        board.undoMove();
        actualUndone++;
    }

    std::cout << "Отменено ходов: " << actualUndone
        << " (запрошено: " << undoCount << ")\n";
    std::cout << "Текущее состояние:\n";
    board.print();
    std::cout << "Ожидаю новый ход...\n";

    return actualUndone;
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        std::cerr << "Ошибка: не указан цвет (0 или 1)\n";
        return 1;
    }

    std::string color = argv[argc - 1];
    Player myColor = (color == "0") ? BLACK : WHITE;
    Player opponentColor = (myColor == BLACK) ? WHITE : BLACK;

    Board board;
    std::cout << "Бот запущен. Мой цвет: " << (myColor == BLACK ? "Черные (первые)" : "Белые (вторые)") << "\n";

    while (true) 
    {
        if (board.currentPlayer == myColor) 
        {
            std::cout << "Думаю...\n";
            board.print();

            Move best = getBestMove(board, myColor, opponentColor);
            board.makeMove(best);

            std::string move = formatMove(best);
            std::cerr << move << std::endl;

            if (board.checkWin(best.row, best.column, myColor)) 
            {

                std::cout << "Я победил!\n";
                return 0;
            }
            if (board.isFull()) 
            {
                std::cout << "Ничья!\n";
                return 4;
            }
        }
        else 
        {
            std::string token;
            if (!(std::cin >> token))
                break;

            int undone = tryUndo(token, board);
            if (undone > 0)
                continue;

            Move opp_move = parseMove(token);
            if (opp_move.row == -1 || opp_move.column == -1) 
            {
                std::cerr << "Некорректный ход: " << token << "\n";
                continue;
            }

            if (board.grid[opp_move.row][opp_move.column] != EMPTY) 
            {
                std::cerr << "Клетка занята: " << token << "\n";
                continue;
            }

            board.makeMove(opp_move);

            if (board.checkWin(opp_move.row, opp_move.column, opponentColor)) 
            {
                std::cout << "Противник победил!\n";
                return 3;
            }
            if (board.isFull()) 
            {
                std::cout << "Ничья!\n";
                return 4;
            }
        }
    }

    return 0;
}