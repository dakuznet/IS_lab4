#include "heuristic.h"

int evaluate(const Board& board, Player myColor, Player opponentColor)
{
	int score = 0;
	int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
	for (int row = 0; row < SIZE; ++row) 
	{
		for (int column = 0; column < SIZE; ++column)
		{
			for (auto& dir : directions)
			{
				int line[5];
				bool valid = true;
				for (int i = 0; i < 5; ++i) 
				{
					int nr = row + dir[0] * i;
					int nc = column + dir[1] * i;
					if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE) 
						line[i] = board.grid[nr][nc];
					else 
					{
						valid = false;
						break;
					}
				}

				if (!valid) continue;

				int myCount = 0;
				int opponentCount = 0;
				int emptyCount = 0;
				for (int i = 0; i < 5; ++i)
				{
					int nr = row + dir[0] * i;
					int nc = column + dir[1] * i;
					if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE)
					{
						if (board.grid[nr][nc] == myColor)
							myCount++;
						else if (board.grid[nr][nc] == opponentColor)
							opponentCount++;
						else
							emptyCount++;
					}
					else
						break;
				}

				if (myCount == 5) 
					score += 100000;
				else if (myCount == 4 && emptyCount == 1) 
					score += 1000000;
				else if (myCount == 3 && emptyCount == 2) 
					score += 1000;
				else if (myCount == 2 && emptyCount == 3) 
					score += 100;

				if (opponentCount == 5) 
					score -= 100000;
				else if (opponentCount == 4 && emptyCount == 1) 
					score -= 1000000;
				else if (opponentCount == 3 && emptyCount == 2) 
					score -= 1000;
				else if (opponentCount == 2 && emptyCount == 3) 
					score -= 100;
			}
		}
	}
	return score;
}