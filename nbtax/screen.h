// SocketServer.h
#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <afxwin.h>
#include <cstring>
#include <vector>
void HandleClient();

SOCKET clientSocket;
// ����ȫ�ֱ���
extern SOCKET m_listenSocket;

// ��������ͻ������ӵĺ���

