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
// 声明全局变量
extern SOCKET m_listenSocket;

// 声明处理客户端连接的函数

