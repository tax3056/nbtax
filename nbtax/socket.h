#include <afxwin.h>
#include <afxdialogex.h>
#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>

#include"nbtaxDlg.h"
#pragma comment(lib, "ws2_32.lib")

// ��ʼ�� Winsock �����������׽�����غ�������
BOOL InitializeWinsockAndSocket();

#endif