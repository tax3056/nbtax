#ifndef GETIP_H
#define GETIP_H

#include <winsock2.h>
#include <afxwin.h>
#include <afxcmn.h>
#include "nbtaxDlg.h"
// 前向声明 CnbtaxDlg 类
class CnbtaxDlg;

// 启动处理客户端连接的线程

void StartClientConnectionThread(CnbtaxDlg* pDlg);

// 处理客户端连接的线程函数
UINT HandleClientConnections(LPVOID pParam);

#endif