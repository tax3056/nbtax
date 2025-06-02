// NetworkManager.cpp
#include "NetworkManager.h"

CNetworkManager::CNetworkManager() : m_listenSocket(INVALID_SOCKET)
{
}

CNetworkManager::~CNetworkManager()
{
    if (m_listenSocket != INVALID_SOCKET)
    {
        closesocket(m_listenSocket);
    }
    WSACleanup();
}

bool CNetworkManager::Initialize()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        AfxMessageBox(_T("Winsock ³õÊ¼»¯Ê§°Ü£¡"));
        return false;
    }

    m_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listenSocket == INVALID_SOCKET)
    {
        AfxMessageBox(_T("´´½¨Ì×½Ó×ÖÊ§°Ü£¡"));
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(80);

    if (bind(m_listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        AfxMessageBox(_T("°ó¶¨Ì×½Ó×ÖÊ§°Ü£¡"));
        closesocket(m_listenSocket);
        WSACleanup();
        return false;
    }

    return true;
}

bool CNetworkManager::StartListening()
{
    if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        AfxMessageBox(_T("¿ªÊ¼¼àÌýÊ§°Ü£¡"));
        closesocket(m_listenSocket);
        WSACleanup();
        return false;
    }
    return true;
}
