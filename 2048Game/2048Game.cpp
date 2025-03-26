// 2048Game.cpp: 게임의 입력과 결과들에 대한 연산과 처리를 담당
#include "pch.h"
#include "2048Game.h"
#include "2048GameDlg.h"

class CMyApp : public CWinApp
{
public:
    CMyApp() {}

    virtual BOOL InitInstance()
    {
        CWinApp::InitInstance();
        C2048GameDlg dlg;
        m_pMainWnd = &dlg;
        dlg.DoModal();
        return FALSE;
    }
};
CMyApp theApp;

Game2048::Game2048(int boardSize)
    : size(boardSize), score(0), highScore(0), gameOver(false),
    infinite(false), gen(rd()), maxUndoCount(3), remainingUndos(3)
{
    loadHighScore();  // 저장된 최고 점수 불러오기
    board.resize(size, std::vector<int>(size, 0));
    addRandomTile();
    addRandomTile();
}

void Game2048::resetGame() {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            board[i][j] = 0;
    score = 0;
    gameOver = false;
    infinite = false;
    remainingUndos = maxUndoCount;
    history.clear();
    addRandomTile();
    addRandomTile();
}

void Game2048::changeBoardSize(int newSize) {
    size = newSize;
    board.clear();
    board.resize(size, std::vector<int>(size, 0));
    score = 0;
    gameOver = false;
    infinite = false;
    remainingUndos = maxUndoCount;
    history.clear();
    addRandomTile();
    addRandomTile();
}

int Game2048::getBoardSize() const {
    return size;
}

void Game2048::addRandomTile() {
    std::vector<std::pair<int, int>> emptyCells;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == 0)
                emptyCells.emplace_back(i, j);

    if (!emptyCells.empty()) {
        std::uniform_int_distribution<> pos_dist(0, emptyCells.size() - 1);
        int index = pos_dist(gen);
        std::uniform_int_distribution<> val_dist(0, 9);
        int value = val_dist(gen) < 9 ? 2 : 4;
        board[emptyCells[index].first][emptyCells[index].second] = value;
    }
}

bool Game2048::canMove() {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == 0)
                return true;

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size - 1; j++)
            if (board[i][j] == board[i][j + 1])
                return true;

    for (int j = 0; j < size; j++)
        for (int i = 0; i < size - 1; i++)
            if (board[i][j] == board[i + 1][j])
                return true;

    return false;
}

bool Game2048::moveTiles(Direction dir) {
    saveState();
    bool moved = false;
    std::vector<std::vector<bool>> merged(size, std::vector<bool>(size, false));

    auto moveLine = [&](int& row, int& col, int dRow, int dCol) {
        while (row + dRow >= 0 && row + dRow < size && col + dCol >= 0 && col + dCol < size && board[row + dRow][col + dCol] == 0) {
            board[row + dRow][col + dCol] = board[row][col];
            board[row][col] = 0;
            row += dRow;
            col += dCol;
            moved = true;
        }
        if (row + dRow >= 0 && row + dRow < size && col + dCol >= 0 && col + dCol < size &&
            board[row + dRow][col + dCol] == board[row][col] && !merged[row + dRow][col + dCol]) {
            board[row + dRow][col + dCol] *= 2;
            score += board[row + dRow][col + dCol];
            board[row][col] = 0;
            merged[row + dRow][col + dCol] = true;
            moved = true;
        }
    };

    if (dir == UP || dir == DOWN) {
        for (int j = 0; j < size; j++) {
            if (dir == UP) {
                for (int i = 1; i < size; i++)
                    if (board[i][j] != 0) {
                        int row = i, col = j;
                        moveLine(row, col, -1, 0);
                    }
            } else {
                for (int i = size - 2; i >= 0; i--)
                    if (board[i][j] != 0) {
                        int row = i, col = j;
                        moveLine(row, col, 1, 0);
                    }
            }
        }
    } else {
        for (int i = 0; i < size; i++) {
            if (dir == LEFT) {
                for (int j = 1; j < size; j++)
                    if (board[i][j] != 0) {
                        int row = i, col = j;
                        moveLine(row, col, 0, -1);
                    }
            } else {
                for (int j = size - 2; j >= 0; j--)
                    if (board[i][j] != 0) {
                        int row = i, col = j;
                        moveLine(row, col, 0, 1);
                    }
            }
        }
    }

    if (moved) {
        addRandomTile();
        if (!canMove()) gameOver = true;
    }

    return moved;
}

void Game2048::saveState() {
    if (history.size() >= maxUndoCount) {
        history.erase(history.begin());
    }
    history.push_back({ board, score, highScore, m_TempHighScore });  // m_TempHighScore 추가
}

bool Game2048::undoMove() {
    if (history.empty() || remainingUndos <= 0) return false;

    GameState state = history.back();
    history.pop_back();

    board = state.boardState;
    score = state.scoreState;
    highScore = state.highScoreState;
    m_TempHighScore = state.tempHighScoreState;  // 세션 최고 점수 복원
    remainingUndos--;

    return true;
}

bool Game2048::isGameOver() const {
    return gameOver;
}

bool Game2048::isWin() const {
    if (infinite) return false;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] >= 2048)
                return true;
    return false;
}

int Game2048::getTileValue(int row, int col) const {
    return board[row][col];
}

int Game2048::getScore() const {
    return score;
}

int Game2048::getHighScore() const {
    return highScore;
}

void Game2048::setHighScore(int s) {
    highScore = s;
    saveHighScore();  // 최고 점수 갱신 시 파일에 저장
}

void Game2048::setInfinite(bool i) {
    infinite = i;
}

int Game2048::getRemainingUndos() const {
    return remainingUndos;
}

void Game2048::plusRemainingUndos() {
    remainingUndos++;
}

void Game2048::saveHighScore() {
    // 파일에 최고 점수 저장
    CString filePath;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath))) {
        PathAppend(szPath, _T("2048Game"));
        CreateDirectory(szPath, NULL);  // 디렉토리가 없으면 생성

        filePath.Format(_T("%s\\highscore.dat"), szPath);

        CFile file;
        if (file.Open(filePath, CFile::modeCreate | CFile::modeWrite)) {
            file.Write(&highScore, sizeof(highScore));
            file.Close();
        }
    }
}

void Game2048::loadHighScore() {
    // 파일에서 최고 점수 불러오기
    CString filePath;
    TCHAR szPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath))) {
        PathAppend(szPath, _T("2048Game"));

        filePath.Format(_T("%s\\highscore.dat"), szPath);

        CFile file;
        if (file.Open(filePath, CFile::modeRead)) {
            file.Read(&highScore, sizeof(highScore));
            file.Close();
        }
    }
}
