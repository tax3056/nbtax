// NetworkManager.h
#pragma once
#include <winsock2.h>
#include <afxwin.h>
#ifndef NETWORK_MODULE_H
#define NETWORK_MODULE_H
class CnbtaxDlg;
class CNetworkManager
{
public:
    CNetworkManager();
    ~CNetworkManager();

    bool Initialize();
    bool StartListening();

 

private:
    SOCKET m_listenSocket;
};
#endif
