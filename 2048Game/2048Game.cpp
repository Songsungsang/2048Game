
// 2048Game.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

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