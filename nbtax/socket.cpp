#include <afxwin.h>
#include <afxdialogex.h>

#include "socket.h"
#include"nbtaxDlg.h"
SOCKET m_listenSocket = INVALID_SOCKET;

void DisplayError1(const TCHAR* message, int errorCode = 0) {
    CString errorMessage;
    if (errorCode != 0) {
        errorMessage.Format(_T("%s: ������� %d"), message, errorCode);
    }
    else {
        errorMessage = message;
    }
    AfxMessageBox(errorMessage);
}

BOOL InitializeWinsockAndSocket()
{
    // ��ʼ��ʼ�� Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        DisplayError1(_T("Winsock ��ʼ��ʧ�ܣ�"), WSAGetLastError());
        return FALSE;
    }

    // ��ʼ���������׽���
    m_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listenSocket == INVALID_SOCKET)
    {
        DisplayError1(_T("�����׽���ʧ�ܣ�"), WSAGetLastError());
        WSACleanup();
        return FALSE;
    }

    // ��ʼ���׽��ֵ� 80 �˿�
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(80);

    if (bind(m_listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        DisplayError1(_T("���׽���ʧ�ܣ�"), WSAGetLastError());
        // ���ر� m_listenSocket������Ӱ����������ʹ��
        WSACleanup();
        return FALSE;
    }

    // ��ʼ����
    if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        DisplayError1(_T("��ʼ����ʧ�ܣ�"), WSAGetLastError());
        // ���ر� m_listenSocket������Ӱ����������ʹ��
        WSACleanup();
        return FALSE;
    }

    return TRUE;
}