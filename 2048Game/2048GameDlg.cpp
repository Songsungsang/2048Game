﻿
// 2048GameDlg.cpp: 구현 파일
//

#include "pch.h"
#include "2048Game.h"
#include "2048GameDlg.h"
#include <afxdialogex.h>  // CDialogEx 클래스를 위한 헤더
#include "resource.h"  // 리소스 ID를 위한 헤더

BEGIN_MESSAGE_MAP(C2048GameDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_KEYDOWN()
    ON_BN_CLICKED(IDC_BUTTON_NEW_GAME, &C2048GameDlg::OnBnClickedButtonNewGame)
    ON_CBN_SELCHANGE(IDC_COMBO_SIZE, &C2048GameDlg::OnCbnSelchangeComboSize)
END_MESSAGE_MAP()

C2048GameDlg::C2048GameDlg(CWnd* pParent)
    : CDialogEx(C2048GameDlg::IDD, pParent)
{
    // 타일 색상 정의
    tileColors[0] = RGB(238, 228, 218); // 2
    tileColors[1] = RGB(237, 224, 200); // 4
    tileColors[2] = RGB(242, 177, 121); // 8
    tileColors[3] = RGB(245, 149, 99);  // 16
    tileColors[4] = RGB(246, 124, 95);  // 32
    tileColors[5] = RGB(246, 94, 59);   // 64
    tileColors[6] = RGB(237, 207, 114); // 128
    tileColors[7] = RGB(237, 204, 97);  // 256
    tileColors[8] = RGB(237, 200, 80);  // 512
    tileColors[9] = RGB(237, 197, 63);  // 1024
    tileColors[10] = RGB(237, 194, 46); // 2048
    tileColors[11] = RGB(60, 58, 50);   // 4096+
}

void C2048GameDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_SCORE, m_ScoreStatic);
    DDX_Control(pDX, IDC_BUTTON_NEW_GAME, m_NewGameButton);
    DDX_Control(pDX, IDC_STATIC_HIGHSCORE, m_HighScoreStatic);
    DDX_Control(pDX, IDC_COMBO_SIZE, m_SizeComboBox);
}

BOOL C2048GameDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 대화 상자 크기 설정
    SetWindowPos(NULL, 0, 0, 600, 700, SWP_NOMOVE | SWP_NOZORDER);

    // 그리드 영역 설정
    gridRect.SetRect(45, 100, 550, 600);

    // 타일 폰트 설정
    tileFont.CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // 점수 폰트 설정
    scoreFont.CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // 키보드 포커스 설정
    SetFocus();

    // 게임 모드 콤보박스 초기화
    m_SizeComboBox.AddString(_T("3x3"));
    m_SizeComboBox.AddString(_T("4x4"));
    m_SizeComboBox.AddString(_T("5x5"));
    m_SizeComboBox.AddString(_T("7x7"));
    m_SizeComboBox.SetCurSel(1);  // 기본값 4x4 선택

    // 점수 초기화
    UpdateScore();

    //[추가] 기본 선택된 콤보박스 사이즈로 보드 사이즈 맞추기
    int index = m_SizeComboBox.GetCurSel();
    int newSize = index + 3;
    game.changeBoardSize(newSize);

    return TRUE;
}

// 콤보박스 변경 핸들러 함수 추가:
void C2048GameDlg::OnCbnSelchangeComboSize()
{
    int index = m_SizeComboBox.GetCurSel();
    int newSize = index + 3;  // 인덱스 0은 3x3, 1은 4x4, 2는 5x5

    game.changeBoardSize(newSize);
    UpdateScore();
    Invalidate();  // 화면 갱신
}

void C2048GameDlg::OnPaint()
{
    CPaintDC dc(this);

    // 더블 버퍼링을 위한 메모리 DC 생성
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);

    CBitmap memBitmap;
    memBitmap.CreateCompatibleBitmap(&dc, gridRect.Width() + 100, gridRect.Height() + 200);
    CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

    // 배경 그리기
    memDC.FillSolidRect(0, 0, gridRect.Width() + 100, gridRect.Height() + 200, RGB(250, 248, 239));

    // 그리드 배경 그리기
    memDC.FillSolidRect(gridRect, RGB(187, 173, 160));

    // 타일 그리기
    int boardSize = game.getBoardSize();
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            DrawTile(&memDC, i, j);
        }
    }

    // 메모리 DC에서 화면으로 복사
    dc.BitBlt(0, 0, gridRect.Width() + 100, gridRect.Height() + 200, &memDC, 0, 0, SRCCOPY);

    // 메모리 DC 정리
    memDC.SelectObject(pOldBitmap);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
}

void C2048GameDlg::DrawTile(CDC* pDC, int row, int col)
{
    int boardSize = game.getBoardSize();
    int tileSize = (gridRect.Width() - (boardSize + 1) * 10) / boardSize;
    int spacing = 10;

    int left = gridRect.left + col * (tileSize + spacing) + spacing;
    int top = gridRect.top + row * (tileSize + spacing) + spacing;

    CRect tileRect(left, top, left + tileSize, top + tileSize);

    int value = game.getTileValue(row, col);
    COLORREF tileColor = RGB(205, 193, 180); // 빈 타일 색상

    if (value > 0)
    {
        int colorIndex = log2(value) - 1;
        if (colorIndex >= 0 && colorIndex < 11)
            tileColor = tileColors[colorIndex];
        else
            tileColor = tileColors[11]; // 4096 이상은 동일한 색상
    }

    // 타일 배경 그리기
    pDC->FillSolidRect(tileRect, tileColor);

    // 타일 값이 있으면 표시
    if (value > 0)
    {
        // 타일 크기에 따라 폰트 크기 조정
        CFont tileFont;
        int fontSize = tileSize / 3;
        tileFont.CreateFont(fontSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, _T("Arial"));

        CFont* pOldFont = pDC->SelectObject(&tileFont);
        pDC->SetBkMode(TRANSPARENT);

        // 값에 따라 텍스트 색상 변경
        if (value <= 4)
            pDC->SetTextColor(RGB(119, 110, 101)); // 어두운 텍스트
        else
            pDC->SetTextColor(RGB(249, 246, 242)); // 밝은 텍스트

        CString strValue;
        strValue.Format(_T("%d"), value);

        // 텍스트 중앙 정렬
        pDC->DrawText(strValue, tileRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        pDC->SelectObject(pOldFont);
        tileFont.DeleteObject();
    }
}

void C2048GameDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    bool moved = false;

    switch (nChar)
    {
    case VK_UP:
        moved = game.moveTiles(Game2048::UP);
        break;
    case VK_DOWN:
        moved = game.moveTiles(Game2048::DOWN);
        break;
    case VK_LEFT:
        moved = game.moveTiles(Game2048::LEFT);
        break;
    case VK_RIGHT:
        moved = game.moveTiles(Game2048::RIGHT);
        break;
    }

    if (moved)
    {
        UpdateScore();
        Invalidate();

        if (game.getScore() > game.getHighScore()) // 최고 점수 체크 및 갱신
        {
            game.setHighScore(game.getScore());
            UpdateHighScore();
        }

        // 게임 승리 또는 게임 오버 확인
        if (game.isWin())
        {
            int result = MessageBox(_T("축하합니다! 2048을 달성했습니다!\n게임을 계속 진행하시겠습니까?"), _T("승리"), MB_YESNO | MB_ICONQUESTION);
            if (result == IDYES)
            {
                game.setInfinite(true);  // 무한 모드 활성화
            }
            if (result == IDNO) 
            {
                MessageBox(_T("다음 게임을 준비합니다."), _T("게임 재시작"), MB_OK | MB_ICONINFORMATION);
                OnBnClickedButtonNewGame(); // 새 게임 시작
            }
        }
        else if (game.isGameOver())
        {
            MessageBox(_T("게임 오버! 더 이상 이동할 수 없습니다."), _T("게임 오버"), MB_OK | MB_ICONINFORMATION);
        }
    }

    CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL C2048GameDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        OnKeyDown(pMsg->wParam, 0, 0);
        return TRUE;  // 방향키 이벤트 처리했음
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

void C2048GameDlg::OnBnClickedButtonNewGame()
{
    game.resetGame();
    UpdateScore();
    Invalidate();
}

void C2048GameDlg::UpdateScore()
{
    CString strScore;
    strScore.Format(_T("점수: %d"), game.getScore());
    m_ScoreStatic.SetWindowText(strScore);
}

void C2048GameDlg::UpdateHighScore()
{
    CString strHigh;
    strHigh.Format(_T("최고 점수: %d"), game.getHighScore());
    m_HighScoreStatic.SetWindowText(strHigh);
}

COLORREF C2048GameDlg::GetTileColor(int value)
{
    if (value == 0)
        return RGB(205, 193, 180); // 빈 타일

    int index = log2(value) - 1;
    if (index >= 0 && index < 11)
        return tileColors[index];

    return tileColors[11]; // 4096 이상
}