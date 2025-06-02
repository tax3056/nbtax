// ClientConnectionHandler.h
#pragma once
#include <winsock2.h>
#include <afxwin.h>
#include <afxcmn.h>

class CClientConnectionHandler
{
public:
    CClientConnectionHandler(CWnd* pDlg);
    void HandleConnections(SOCKET listenSocket);

private:
    CWnd* m_pDlg;
};
