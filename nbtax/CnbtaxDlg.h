// CnbtaxDlg.h
#pragma once
#include "NetworkManager.h"
#include "ListCtrlInitializer.h"
#include "ClientConnectionHandler.h"
#include <afxwin.h>

class CnbtaxDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CnbtaxDlg)

public:
    CnbtaxDlg(CWnd* pParent = nullptr);
    virtual ~CnbtaxDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MY_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()
private:
    CNetworkManager m_networkManager;
public:
    virtual BOOL OnInitDialog();
};