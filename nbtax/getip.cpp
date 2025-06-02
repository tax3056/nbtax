#include "getip.h"
#include "nbtaxDlg.h"  // 假设 CnbtaxDlg 类定义在这个头文件中
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
// 函数声明
int SomeFunction(CDialogEx* pDlg, SOCKET clientSocket);
void DisplayError(const CString& errorMessage, int errorCode = 0);
void AddToReportView(CListCtrl* pListCtrl, int nItemIndex, const CString* values, int columnCount);
int ReceiveInnerIp(SOCKET clientSocket, CString& wideStrInnerIp);
int ReceiveData(SOCKET clientSocket, CString& wideStrData, const CString& infoType);


// 处理客户端连接的线程函数
UINT HandleClientConnections(LPVOID pParam)
{
    CnbtaxDlg* pDlg = (CnbtaxDlg*)pParam;
    if (!pDlg)
    {
        DisplayError(_T("传入的对话框指针无效"));
        return 1;
    }

    // 检查 m_listenSocket 是否有效
    if (m_listenSocket == INVALID_SOCKET)
    {
        DisplayError(_T("监听套接字无效，请先初始化 Winsock 并创建监听套接字"), WSAGetLastError());
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
                AfxMessageBox(_T("SomeFunction 函数调用结束，信息已插入报表视图"));
            }
            // 处理完客户端连接后，关闭客户端套接字
            closesocket(clientSocket);
        }
        else
        {
            DisplayError(_T("接受客户端连接失败"), WSAGetLastError());
        }
    }

    return 0;
}

// 将值添加到报表视图
void AddToReportView(CListCtrl* pListCtrl, int nItemIndex, const CString* values, int columnCount)
{
    if (nItemIndex != -1)
    {
        for (int i = 0; i < columnCount; ++i)
        {
            if (!pListCtrl->SetItemText(nItemIndex, i, values[i]))
            {
                DisplayError(_T("设置列表项文本失败，信息未添加成功"));
                return;
            }
        }
      
    }
    else
    {
        DisplayError(_T("插入列表项失败，信息未添加成功"));
    }
}


// 假设 CnbtaxDlg 类定义在这个头文件中


#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "psapi.lib")

// 启动处理客户端连接的线程
void StartClientConnectionThread(CnbtaxDlg* pDlg)
{

    CWinThread* pThread = AfxBeginThread(HandleClientConnections, pDlg);
    if (!pThread)
    {
        DisplayError(_T("启动线程失败"), GetLastError());
    }
   
}

// 显示错误信息
void DisplayError(const CString& errorMessage, int errorCode)
{
    CString msg;
    if (errorCode != 0)
    {
        msg.Format(_T("%s，错误代码: %d"), errorMessage, errorCode);
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
        DisplayError(_T("获取客户端地址失败"), WSAGetLastError());
        return 1;
    }

    char ipBuffer[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &clientAddr.sin_addr, ipBuffer, INET_ADDRSTRLEN) == nullptr) {
        DisplayError(_T("inet_ntop 失败"), WSAGetLastError());
        return 1;
    }

    wchar_t wideIpBuffer[INET_ADDRSTRLEN];
    MultiByteToWideChar(CP_UTF8, 0, ipBuffer, -1, wideIpBuffer, INET_ADDRSTRLEN);

    DWORD bufferSize = INET_ADDRSTRLEN;
    if (WSAAddressToStringW((PSOCKADDR)&clientAddr, clientAddrLen, NULL, wideIpBuffer, &bufferSize) != 0) {
        DisplayError(_T("WSAAddressToStringW 失败"), WSAGetLastError());
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
        errorMsg.Format(_T("接收 %s 失败"), infoType);
        DisplayError(errorMsg, WSAGetLastError());
        return 1;
    }
    dataUtf8[recvSize] = '\0';

    int len = MultiByteToWideChar(CP_UTF8, 0, dataUtf8, -1, NULL, 0);
    wchar_t* wideData = new wchar_t[len];
    if (!wideData) {
        CString errorMsg;
        errorMsg.Format(_T("内存分配失败（%s）"), infoType);
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

    // 依次获取信息
    if (ReceiveData(clientSocket, wideStrExternalIp, _T("外网 IP 信息")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveInnerIp(clientSocket, wideStrInnerIp) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveData(clientSocket, wideStrSystemVersion, _T("系统版本信息")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveData(clientSocket, wideStrHostname, _T("主机名信息")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveData(clientSocket, wideStrUsername, _T("用户名信息")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveData(clientSocket, wideStrCpuInfo, _T("CPU 信息")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveData(clientSocket, wideStrMemoryInfo, _T("内存信息")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    if (ReceiveData(clientSocket, wideStrDiskInfo, _T("硬盘信息")) != 0)
    {
        closesocket(clientSocket);
        return 1;
    }

    closesocket(clientSocket);

    // 获取列表控件指针
    CListCtrl* pListCtrlWiznext = (CListCtrl*)pDlg->GetDlgItem(ID_WIZNEXT);
    if (!pListCtrlWiznext)
    {
        DisplayError(_T("获取列表控件指针失败，无法添加信息"));
        return 1;
    }

    // 准备要插入的数据数组，按照指定列顺序排列
    CString values[9];
    values[0] = wideStrExternalIp;
    values[1] = wideStrSystemVersion;
    values[2] = wideStrHostname;
    values[3] = wideStrUsername;
    values[4] = wideStrCpuInfo;
    values[5] = _T(""); // 原代码中缺少 values[5] 的赋值，这里补上空字符串
    values[6] = wideStrMemoryInfo;
    values[7] = wideStrDiskInfo;
    values[8] = _T("");
    
// 插入新的列表项
    int nItemIndex = pListCtrlWiznext->InsertItem(pListCtrlWiznext->GetItemCount(), _T(""));
    if (nItemIndex != -1)
    {
        // 将数据插入到对应的列
        for (int i = 0; i < 9; ++i) // 修改循环条件，从 8 改为 9
        {
            if (!pListCtrlWiznext->SetItemText(nItemIndex, i, values[i]))
            {
                DisplayError(_T("设置列表项文本失败，信息未添加成功"));
                return 1;
            }
        }
    
    }
    else
    {
        DisplayError(_T("插入列表项失败，信息未添加成功"));
        return 1;
    }

    return 0;
}
