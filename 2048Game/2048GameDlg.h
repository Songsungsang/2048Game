
// 2048GameDlg.h: 헤더 파일
//

#pragma once
#include "2048Game.h"

class C2048GameDlg : public CDialogEx
{
private:
    Game2048 game;
    CRect gridRect;
    CFont tileFont;
    CFont scoreFont;
    COLORREF tileColors[12];
    CComboBox m_SizeComboBox;  // 사이즈 선택 콤보박스
    CButton m_UndoButton;  // 되돌리기 버튼 추가
    CStatic m_UndoCountStatic;  // 남은 되돌리기 횟수 표시 스태틱
    CStatic m_TempHighScoreStatic;  // 추가
    int m_TempHighScore;             // 추가
    void UpdateTempHighScore();

public:
    C2048GameDlg(CWnd* pParent = nullptr);
    enum { IDD = IDD_DIALOG_2048 };
    afx_msg void OnBnClickedButtonUndo();  // 되돌리기 버튼 핸들러 추가
    void UpdateUndoCount();  // 되돌리기 횟수 업데이트 함수

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnPaint();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnBnClickedButtonNewGame();
    afx_msg void OnCbnSelchangeComboSize();
    void UpdateBasic();
    DECLARE_MESSAGE_MAP()

private:
    CStatic m_ScoreStatic;
    CStatic m_HighScoreStatic;
    CButton m_NewGameButton;
    void DrawTile(CDC* pDC, int row, int col);
    void UpdateScore();
    void UpdateHighScore(); 
public:
    afx_msg void OnStnClickedStaticHighscore();
};