
// ClientConnectionHandler.cpp
#include "ClientConnectionHandler.h"
#include <ws2tcpip.h>
CClientConnectionHandler::CClientConnectionHandler(CWnd* pDlg) : m_pDlg(pDlg)
{
}

void CClientConnectionHandler::HandleConnections(SOCKET listenSocket)
{
    while (true)
    {
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket != INVALID_SOCKET)
        {
            DWORD ipAddrValue = clientAddr.sin_addr.S_un.S_addr;
            char debugMsg[50];
            sprintf_s(debugMsg, "clientAddr value: %u\n", ipAddrValue);
            OutputDebugStringA(debugMsg);

            char ipBuffer[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &clientAddr.sin_addr, ipBuffer, INET_ADDRSTRLEN) == nullptr)
               
            {
                int errorCode = WSAGetLastError();
                CString errorMsg;
                errorMsg.Format(_T("inet_ntop 失败，错误代码: %d"), errorCode);
                AfxMessageBox(errorMsg);
                closesocket(clientSocket);
                continue;
            }

            CStringA strIP = ipBuffer;
            CString wideStr(strIP);

            CString message;
            message.Format(_T("新客户端连接，IP 地址: %s"), wideStr);

            CListCtrl* pListCtrlWiznext = (CListCtrl*)m_pDlg->GetDlgItem(ID_WIZNEXT);
            if (pListCtrlWiznext) {
                int nItemIndex = pListCtrlWiznext->InsertItem(pListCtrlWiznext->GetItemCount(), _T(""));
                if (nItemIndex != -1)
                {
                    pListCtrlWiznext->SetItemText(nItemIndex, 1, wideStr);
                }
            }
        }
        else
        {
            int errorCode = WSAGetLastError();
            if (errorCode != WSAEWOULDBLOCK)
            {
                CString errorMsg;
                errorMsg.Format(_T("accept 失败，错误代码: %d"), errorCode);
                AfxMessageBox(errorMsg);
            }
        }
    }
}