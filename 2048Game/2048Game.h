// 2048Game.h: 헤더 파일
//

#pragma once
#pragma comment(lib, "Shell32.lib")
#include <Shlobj.h>
#include "pch.h"
#include "resource.h"  // 리소스 ID를 위한 헤더
#include <vector>
#include <random>
#include <algorithm>
#include <afxdialogex.h>  // CDialogEx 클래스를 위한 헤더
#include "afxwinappex.h"

class Game2048 {
private:
    std::vector<std::vector<int>> board;
    int score;
    int highScore;
    int size;
    int m_TempHighScore;  // 세션 최고 점수 추적용
    bool gameOver;
    bool infinite;
    std::random_device rd;
    std::mt19937 gen;

    struct GameState {
        std::vector<std::vector<int>> boardState;
        int scoreState;
        int highScoreState;
        int tempHighScoreState;  // 세션 최고 점수 추가
    };

    std::vector<GameState> history;
    int maxUndoCount;
    int remainingUndos;
    void saveHighScore();
    void loadHighScore();
public:
    enum Direction { UP, DOWN, LEFT, RIGHT };

    Game2048(int boardSize = 7);

    void resetGame();
    void changeBoardSize(int newSize);
    int getBoardSize() const;
    void addRandomTile();
    bool canMove();
    bool moveTiles(Direction dir);
    int getTileValue(int row, int col) const;
    int getScore() const;
    int getHighScore() const;
    void setHighScore(int s);
    void setInfinite(bool i);
    bool isGameOver() const;
    bool isWin() const;
    void saveState();
    bool undoMove();
    int getRemainingUndos() const;
    void plusRemainingUndos();
};