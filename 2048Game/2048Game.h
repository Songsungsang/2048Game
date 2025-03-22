#pragma once
#include <vector>
#include <random>
#include <algorithm>

class Game2048
{
private:
    std::vector<std::vector<int>> board;
    int score;
    int highScore = 0;
    int size;
    bool gameOver;
    bool infinite = false;
    std::random_device rd;
    std::mt19937 gen;

public:
    enum Direction { UP, DOWN, LEFT, RIGHT };

    Game2048(int boardSize = 4) : size(boardSize), score(0), gameOver(false), gen(rd())
    {
        board.resize(size, std::vector<int>(size, 0));
        // 게임 시작 시 무작위로 두 개의 타일 생성
        addRandomTile();
        addRandomTile();
    }

    // 게임 보드 초기화
    void resetGame()
    {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                board[i][j] = 0;

        score = 0;
        gameOver = false;
        infinite = false;
        addRandomTile();
        addRandomTile();
    }

    // 무작위 위치에 2 또는 4 타일 추가
    void addRandomTile()
    {
        std::vector<std::pair<int, int>> emptyCells;

        // 빈 셀 찾기
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (board[i][j] == 0)
                    emptyCells.push_back(std::make_pair(i, j));
            }
        }

        if (!emptyCells.empty())
        {
            // 무작위로 빈 셀 선택
            std::uniform_int_distribution<> pos_dist(0, emptyCells.size() - 1);
            int index = pos_dist(gen);

            // 90% 확률로 2, 10% 확률로 4 생성
            std::uniform_int_distribution<> val_dist(0, 9);
            int value = val_dist(gen) < 9 ? 2 : 4;

            board[emptyCells[index].first][emptyCells[index].second] = value;
        }
    }

    // 타일 이동 가능성 확인
    bool canMove()
    {
        // 빈 셀이 있는지 확인
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (board[i][j] == 0)
                    return true;

        // 인접한 타일이 같은 값을 가지는지 확인
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size - 1; j++)
            {
                if (board[i][j] == board[i][j + 1])
                    return true;
            }
        }

        for (int j = 0; j < size; j++)
        {
            for (int i = 0; i < size - 1; i++)
            {
                if (board[i][j] == board[i + 1][j])
                    return true;
            }
        }

        return false;
    }

    // 타일 이동
    bool moveTiles(Direction dir)
    {
        bool moved = false;
        std::vector<std::vector<bool>> merged(size, std::vector<bool>(size, false));

        switch (dir)
        {
        case UP:
            for (int j = 0; j < size; j++)
            {
                for (int i = 1; i < size; i++)
                {
                    if (board[i][j] != 0)
                    {
                        int row = i;
                        while (row > 0 && board[row - 1][j] == 0)
                        {
                            board[row - 1][j] = board[row][j];
                            board[row][j] = 0;
                            row--;
                            moved = true;
                        }

                        if (row > 0 && board[row - 1][j] == board[row][j] && !merged[row - 1][j])
                        {
                            board[row - 1][j] *= 2;
                            score += board[row - 1][j];
                            board[row][j] = 0;
                            merged[row - 1][j] = true;
                            moved = true;
                        }
                    }
                }
            }
            break;

        case DOWN:
            for (int j = 0; j < size; j++)
            {
                for (int i = size - 2; i >= 0; i--)
                {
                    if (board[i][j] != 0)
                    {
                        int row = i;
                        while (row < size - 1 && board[row + 1][j] == 0)
                        {
                            board[row + 1][j] = board[row][j];
                            board[row][j] = 0;
                            row++;
                            moved = true;
                        }

                        if (row < size - 1 && board[row + 1][j] == board[row][j] && !merged[row + 1][j])
                        {
                            board[row + 1][j] *= 2;
                            score += board[row + 1][j];
                            board[row][j] = 0;
                            merged[row + 1][j] = true;
                            moved = true;
                        }
                    }
                }
            }
            break;

        case LEFT:
            for (int i = 0; i < size; i++)
            {
                for (int j = 1; j < size; j++)
                {
                    if (board[i][j] != 0)
                    {
                        int col = j;
                        while (col > 0 && board[i][col - 1] == 0)
                        {
                            board[i][col - 1] = board[i][col];
                            board[i][col] = 0;
                            col--;
                            moved = true;
                        }

                        if (col > 0 && board[i][col - 1] == board[i][col] && !merged[i][col - 1])
                        {
                            board[i][col - 1] *= 2;
                            score += board[i][col - 1];
                            board[i][col] = 0;
                            merged[i][col - 1] = true;
                            moved = true;
                        }
                    }
                }
            }
            break;

        case RIGHT:
            for (int i = 0; i < size; i++)
            {
                for (int j = size - 2; j >= 0; j--)
                {
                    if (board[i][j] != 0)
                    {
                        int col = j;
                        while (col < size - 1 && board[i][col + 1] == 0)
                        {
                            board[i][col + 1] = board[i][col];
                            board[i][col] = 0;
                            col++;
                            moved = true;
                        }

                        if (col < size - 1 && board[i][col + 1] == board[i][col] && !merged[i][col + 1])
                        {
                            board[i][col + 1] *= 2;
                            score += board[i][col + 1];
                            board[i][col] = 0;
                            merged[i][col + 1] = true;
                            moved = true;
                        }
                    }
                }
            }
            break;
        }

        // 타일이 이동했으면 새로운 타일 추가
        if (moved)
        {
            addRandomTile();

            // 게임 오버 확인
            if (!canMove())
                gameOver = true;
        }

        return moved;
    }

    // 게임 보드 상태 얻기
    int getTileValue(int row, int col) const
    {
        return board[row][col];
    }

    // 점수 얻기
    int getScore() const
    {
        return score;
    }

    // 점수 얻기
    int getHighScore() const
    {
        return highScore;
    }

    // 무한모드 설정
    void setHighScore(int s)
    {
        highScore = s;
    }

    // 무한모드 설정
    void setInfinite(bool i)
    {
        infinite = i;
    }

    // 게임 오버 상태 확인
    bool isGameOver() const
    {
        return gameOver;
    }

    // 게임 승리 확인 (2048 타일 존재 여부)
    bool isWin() const
    {   
        if (infinite == true) return false; // 무한모드 활성화시 승리조건 확인 무시

        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                if (board[i][j] >= 2048)
                    return true;
        return false;
    }
};