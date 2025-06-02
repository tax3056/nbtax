#include <afxwin.h>
#include <afxdialogex.h>
#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>

#include"nbtaxDlg.h"
#pragma comment(lib, "ws2_32.lib")

// 初始化 Winsock 并创建监听套接字相关函数声明
BOOL InitializeWinsockAndSocket();

#endif