#include "bot.h"
#include "heuristic.h"
#include <vector>
#include <algorithm>

std::vector<Move> generateMoves(const Board& board) {
    std::vector<Move> moves;

    if (board.history.empty())
        return { {7, 7, 0} };

    bool considered[SIZE][SIZE] = { false };
    for (int r = 0; r < SIZE; ++r) 
    {
        for (int c = 0; c < SIZE; ++c) 
        {
            if (board.grid[r][c] != EMPTY) 
            {
                for (int dr = -2; dr <= 2; ++dr) 
                {
                    for (int dc = -2; dc <= 2; ++dc) 
                    {
                        int nr = r + dr, nc = c + dc;
                        if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE) 
                        {
                            if (board.grid[nr][nc] == EMPTY && !considered[nr][nc]) 
                            {
                                considered[nr][nc] = true;
                                moves.push_back({ nr, nc, 0 });
                            }
                        }
                    }
                }
            }
        }
    }
    return moves;
}

int alphabeta(Board& board, int depth, int alpha, int beta, bool isMaximizing, SearchState& state, Player myColor, Player opponentColor) 
{
    if (state.timeUp) 
        return 0;

    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - state.start_time).count() > state.timeLimitMS) 
    {
        state.timeUp = true;
        return 0;
    }

    if (depth == 0) 
    {
        return evaluate(board, myColor, opponentColor);
    }

    std::vector<Move> moves = generateMoves(board);

    for (auto& move : moves) 
    {
        board.makeMove(move);
        move.score = evaluate(board, myColor, opponentColor);
        board.undoMove();
    }

    if (isMaximizing) {
        std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
            return a.score > b.score;
            });
    }
    else {
        std::sort(moves.begin(), moves.end(), [](const Move& a, const Move& b) {
            return a.score < b.score;
            });
    }

    if (isMaximizing) 
    {
        int maxEval = INT_MIN;
        for (const auto& move : moves) 
        {
            board.makeMove(move);
            int eval = alphabeta(board, depth - 1, alpha, beta, false, state, myColor, opponentColor);
            board.undoMove();

            if (state.timeUp) 
                return maxEval;

            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) 
                break;
        }
        return maxEval;
    }
    else 
    {
        int minEval = INT_MAX;
        for (const auto& move : moves) 
        {
            board.makeMove(move);
            int eval = alphabeta(board, depth - 1, alpha, beta, true, state, myColor, opponentColor);
            board.undoMove();

            if (state.timeUp) 
                return minEval;

            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) 
                break;
        }
        return minEval;
    }
}

Move getBestMove(Board& board, Player myColor, Player opponentColor) {
    SearchState state;
    state.start_time = std::chrono::steady_clock::now();
    state.bestScore = INT_MIN;

    std::vector<Move> moves = generateMoves(board);
    if (moves.empty()) 
        return {7, 7, 0};
    state.bestMove = moves[0];

    for (const auto& move : moves) 
    {
        board.makeMove(move);
        if (board.checkWin(move.row, move.column, myColor))
        {
            board.undoMove();
            return move;
        }
        board.undoMove();
    }

    for (const auto& move : moves) 
    {
        board.makeMove(move);
        if (board.checkWin(move.row, move.column, opponentColor)) 
        {
            board.undoMove();
            return move;
        }
        board.undoMove();
    }

    for (int depth = 1; depth <= 15; ++depth) 
    {
        state.timeUp = false;

        for (const auto& move : moves) 
        {
            board.makeMove(move);
            int score = alphabeta(board, depth - 1, INT_MIN, INT_MAX, false, state, myColor, opponentColor);
            board.undoMove();

            if (state.timeUp) 
                break;

            if (score > state.bestScore) 
            {
                state.bestScore = score;
                state.bestMove = move;
            }
        }

        if (state.timeUp) 
            break;
    }

    return state.bestMove;
}