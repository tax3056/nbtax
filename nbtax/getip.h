#ifndef GETIP_H
#define GETIP_H

#include <winsock2.h>
#include <afxwin.h>
#include <afxcmn.h>
#include "nbtaxDlg.h"
// ǰ������ CnbtaxDlg ��
class CnbtaxDlg;

// ��������ͻ������ӵ��߳�

void StartClientConnectionThread(CnbtaxDlg* pDlg);

// ����ͻ������ӵ��̺߳���
UINT HandleClientConnections(LPVOID pParam);

#endif