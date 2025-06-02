#include <afxwin.h>
#include <afxdialogex.h>

#include "socket.h"
#include"nbtaxDlg.h"
SOCKET m_listenSocket = INVALID_SOCKET;

void DisplayError1(const TCHAR* message, int errorCode = 0) {
    CString errorMessage;
    if (errorCode != 0) {
        errorMessage.Format(_T("%s: 错误代码 %d"), message, errorCode);
    }
    else {
        errorMessage = message;
    }
    AfxMessageBox(errorMessage);
}

BOOL InitializeWinsockAndSocket()
{
    // 开始初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        DisplayError1(_T("Winsock 初始化失败！"), WSAGetLastError());
        return FALSE;
    }

    // 开始创建监听套接字
    m_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listenSocket == INVALID_SOCKET)
    {
        DisplayError1(_T("创建套接字失败！"), WSAGetLastError());
        WSACleanup();
        return FALSE;
    }

    // 开始绑定套接字到 80 端口
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(80);

    if (bind(m_listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        DisplayError1(_T("绑定套接字失败！"), WSAGetLastError());
        // 不关闭 m_listenSocket，避免影响其他函数使用
        WSACleanup();
        return FALSE;
    }

    // 开始监听
    if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        DisplayError1(_T("开始监听失败！"), WSAGetLastError());
        // 不关闭 m_listenSocket，避免影响其他函数使用
        WSACleanup();
        return FALSE;
    }

    return TRUE;
}