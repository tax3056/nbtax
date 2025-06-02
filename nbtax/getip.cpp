#include "getip.h"
#include "nbtaxDlg.h"  // ���� CnbtaxDlg �ඨ�������ͷ�ļ���
#include <ws2tcpip.h>
#include "Resource.h"
#include <afx.h>
#include <afxwin.h>
#include <winsock2.h>
#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <tchar.h>
#include<string.h>

extern SOCKET m_listenSocket;
// ��������
int SomeFunction(CDialogEx* pDlg, SOCKET clientSocket);
void DisplayError(const CString& errorMessage, int errorCode = 0);
void AddToReportView(CListCtrl* pListCtrl, int nItemIndex, const CString* values, int columnCount);
int ReceiveInnerIp(SOCKET clientSocket, CString& wideStrInnerIp);
int ReceiveData(SOCKET clientSocket, CString& wideStrData, const CString& infoType);


// ����ͻ������ӵ��̺߳���
UINT HandleClientConnections(LPVOID pParam)
{
    CnbtaxDlg* pDlg = (CnbtaxDlg*)pParam;
    if (!pDlg)
    {
        DisplayError(_T("����ĶԻ���ָ����Ч"));
        return 1;
    }

    // ��� m_listenSocket �Ƿ���Ч
    if (m_listenSocket == INVALID_SOCKET)
    {
        DisplayError(_T("�����׽�����Ч�����ȳ�ʼ�� Winsock �����������׽���"), WSAGetLastError());
        return 1;
    }

    while (true)
    {
        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(m_listenSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket != INVALID_SOCKET)
        {
            if (SomeFunction(pDlg, clientSocket) == 0)
            {
                AfxMessageBox(_T("SomeFunction �������ý�������Ϣ�Ѳ��뱨����ͼ"));
            }
            // ������ͻ������Ӻ󣬹رտͻ����׽���
            closesocket(clientSocket);
        }
        else
        {
            DisplayError(_T("���ܿͻ�������ʧ��"), WSAGetLastError());
        }
    }

    return 0;
}

// ��ֵ��ӵ�������ͼ
void AddToReportView(CListCtrl* pListCtrl, int nItemIndex, const CString* values, int columnCount)
{
    if (nItemIndex != -1)
    {
        for (int i = 0; i < columnCount; ++i)
        {
            if (!pListCtrl->SetItemText(nItemIndex, i, values[i]))
            {
                DisplayError(_T("�����б����ı�ʧ�ܣ���Ϣδ��ӳɹ�"));
                return;
            }
        }
      
    }
    else
    {
        DisplayError(_T("�����б���ʧ�ܣ���Ϣδ��ӳɹ�"));
    }
}


// ���� CnbtaxDlg �ඨ�������ͷ�ļ���


#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "psapi.lib")

// ��������ͻ������ӵ��߳�
void StartClientConnectionThread(CnbtaxDlg* pDlg)
{

    CWinThread* pThread = AfxBeginThread(HandleClientConnections, pDlg);
    if (!pThread)
    {
        DisplayError(_T("�����߳�ʧ��"), GetLastError());
    }
   
}

// ��ʾ������Ϣ
void DisplayError(const CString& errorMessage, int errorCode)
{
    CString msg;
    if (errorCode != 0)
    {
        msg.Format(_T("%s���������: %d"), errorMessage, errorCode);
    }
    else
    {
        msg = errorMessage;
    }
    AfxMessageBox(msg);
}

int ReceiveInnerIp(SOCKET clientSocket, CString& wideStrInnerIp)
{
    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    if (getpeername(clientSocket, (sockaddr*)&clientAddr, &clientAddrLen) == SOCKET_ERROR) {
        DisplayError(_T("��ȡ�ͻ��˵�ַʧ��"), WSAGetLastError());
        return 1;
    }

    char ipBuffer[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &clientAddr.sin_addr, ipBuffer, INET_ADDRSTRLEN) == nullptr) {
        DisplayError(_T("inet_ntop ʧ��"), WSAGetLastError());
        return 1;
    }

    wchar_t wideIpBuffer[INET_ADDRSTRLEN];
    MultiByteToWideChar(CP_UTF8, 0, ipBuffer, -1, wideIpBuffer, INET_ADDRSTRLEN);

    DWORD bufferSize = INET_ADDRSTRLEN;
    if (WSAAddressToStringW((PSOCKADDR)&clientAddr, clientAddrLen, NULL, wideIpBuffer, &bufferSize) != 0) {
        DisplayError(_T("WSAAddressToStringW ʧ��"), WSAGetLastError());
        return 1;
    }

    wideStrInnerIp = wideIpBuffer;
    return 0;
}

int ReceiveData(SOCKET clientSocket, CString& wideStrData, const CString& infoType)
{
    char dataUtf8[1024];
    int recvSize = recv(clientSocket, dataUtf8, sizeof(dataUtf8) - 1, 0);
    if (recvSize <= 0) {
        CString errorMsg;
        errorMsg.Format(_T("���� %s ʧ��"), infoType);
        DisplayError(errorMsg, WSAGetLastError());
        return 1;
    }
    dataUtf8[recvSize] = '\0';

    int len = MultiByteToWideChar(CP_UTF8, 0, dataUtf8, -1, NULL, 0);
    wchar_t* wideData = new wchar_t[len];
    if (!wideData) {
        CString errorMsg;
        errorMsg.Format(_T("�ڴ����ʧ�ܣ�%s��"), infoType);
        DisplayError(errorMsg, 0);
        return 1;
    }
    MultiByteToWideChar(CP_UTF8, 0, dataUtf8, -1, wideData, len);
    wideStrData = wideData;
    delete[] wideData;
    return 0;
}

int SomeFunction(CDialogEx* pDlg, SOCKET clientSocket)
{
    CString wideStrExternalIp, wideStrInnerIp, wideStrSystemVersion, wideStrHostname, wideStrUsername, wideStrCpuInfo, wideStrMemoryInfo, wideStrDiskInfo;

    // ���λ�ȡ��Ϣ
    if (ReceiveData(clientSocket, wideStrExternalIp, _T("���� IP ��Ϣ")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveInnerIp(clientSocket, wideStrInnerIp) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveData(clientSocket, wideStrSystemVersion, _T("ϵͳ�汾��Ϣ")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveData(clientSocket, wideStrHostname, _T("��������Ϣ")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveData(clientSocket, wideStrUsername, _T("�û�����Ϣ")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveData(clientSocket, wideStrCpuInfo, _T("CPU ��Ϣ")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveData(clientSocket, wideStrMemoryInfo, _T("�ڴ���Ϣ")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveData(clientSocket, wideStrDiskInfo, _T("Ӳ����Ϣ")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    closesocket(clientSocket);

    // ��ȡ�б�ؼ�ָ��
    CListCtrl* pListCtrlWiznext = (CListCtrl*)pDlg->GetDlgItem(ID_WIZNEXT);
    if (!pListCtrlWiznext)
    {
        DisplayError(_T("��ȡ�б�ؼ�ָ��ʧ�ܣ��޷������Ϣ"));
        return 1;
    }

    // ׼��Ҫ������������飬����ָ����˳������
    CString values[9];
    values[0] = wideStrExternalIp;
    values[1] = wideStrSystemVersion;
    values[2] = wideStrHostname;
    values[3] = wideStrUsername;
    values[4] = wideStrCpuInfo;
    values[5] = _T(""); // ԭ������ȱ�� values[5] �ĸ�ֵ�����ﲹ�Ͽ��ַ���
    values[6] = wideStrMemoryInfo;
    values[7] = wideStrDiskInfo;
    values[8] = _T("");
    
// �����µ��б���
    int nItemIndex = pListCtrlWiznext->InsertItem(pListCtrlWiznext->GetItemCount(), _T(""));
    if (nItemIndex != -1)
    {
        // �����ݲ��뵽��Ӧ����
        for (int i = 0; i < 9; ++i) // �޸�ѭ���������� 8 ��Ϊ 9
        {
            if (!pListCtrlWiznext->SetItemText(nItemIndex, i, values[i]))
            {
                DisplayError(_T("�����б����ı�ʧ�ܣ���Ϣδ��ӳɹ�"));
                return 1;
            }
        }
    
    }
    else
    {
        DisplayError(_T("�����б���ʧ�ܣ���Ϣδ��ӳɹ�"));
        return 1;
    }

    return 0;
}
