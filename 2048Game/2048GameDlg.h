
// 2048GameDlg.h: 헤더 파일
//

#pragma once
#include "resource.h"
#include "pch.h"
#include "2048Game.h"

class C2048GameDlg : public CDialogEx
{
private:
    Game2048 game;
    CRect gridRect;
    CFont tileFont;
    CFont scoreFont;
    COLORREF tileColors[12];

public:
    C2048GameDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_DIALOG_2048 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnPaint();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnBnClickedButtonNewGame();
    DECLARE_MESSAGE_MAP()

private:
    CStatic m_ScoreStatic;
    CButton m_NewGameButton;
    COLORREF GetTileColor(int value);
    void DrawTile(CDC* pDC, int row, int col);
    void UpdateScore();
};