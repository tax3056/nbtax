#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "nbtax.h"
#include "afxdialogex.h"
#include "CTreeListDlg.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

// 定义接收缓冲区大小
const int BUFFER_SIZE = 1024;



// 线程安全的全局数据
namespace {
    std::mutex g_dataMutex;
    std::vector<FileInfo> g_fileData;
}

// CTreeListDlg 对话框
IMPLEMENT_DYNAMIC(CTreeListDlg, CDialogEx)

// 自定义消息
#define WM_SERVER_INITIALIZED (WM_USER + 100)
#define WM_CLIENT_CONNECTED (WM_USER + 101)
#define WM_UPDATE_FILE_LIST (WM_USER + 102)
#define WM_SERVER_ERROR (WM_USER + 103)

BOOL CTreeListDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 1. 设置 List Control 为 Report 模式（类似文件管理器）
    m_listCtrl.ModifyStyle(0, LVS_REPORT);

    // 2. 设置扩展样式（网格线、整行选择、双缓冲等）
    DWORD dwExStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER;
    m_listCtrl.SetExtendedStyle(dwExStyle);

    // 3. 添加列（文件名、大小、修改时间）
    m_listCtrl.InsertColumn(0, _T("文件名"), LVCFMT_LEFT, 200);   // 左对齐，宽度200
    m_listCtrl.InsertColumn(1, _T("大小"), LVCFMT_RIGHT, 100);   // 右对齐，宽度100
    m_listCtrl.InsertColumn(2, _T("修改日期"), LVCFMT_LEFT, 150); // 左对齐，宽度150

    // 4. 可选：设置排序方式（点击列标题排序）
    m_listCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_HEADERDRAGDROP);

    // 5. 可选：添加测试数据（调试用）
    // InsertTestData();

    return TRUE;
}
void CTreeListDlg::InsertTestData()
{
    // 插入示例数据（文件名、大小、修改时间）
    int nIndex = m_listCtrl.InsertItem(0, _T("Document.txt"));
    m_listCtrl.SetItemText(nIndex, 1, _T("1.5 KB"));
    m_listCtrl.SetItemText(nIndex, 2, _T("2024-03-15 10:30"));

    nIndex = m_listCtrl.InsertItem(1, _T("Image.jpg"));
    m_listCtrl.SetItemText(nIndex, 1, _T("2.3 MB"));
    m_listCtrl.SetItemText(nIndex, 2, _T("2024-03-14 15:45"));

    nIndex = m_listCtrl.InsertItem(2, _T("Program.exe"));
    m_listCtrl.SetItemText(nIndex, 1, _T("5.7 MB"));
    m_listCtrl.SetItemText(nIndex, 2, _T("2024-03-10 09:20"));
}

CTreeListDlg::CTreeListDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG3, pParent)
    , m_listenSocket(INVALID_SOCKET)
    , m_winsockInitialized(false)
    , m_serverRunning(false)
{
}

CTreeListDlg::~CTreeListDlg()
{
    StopServer();
}

void CTreeListDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST5, m_listCtrl); // 确保 IDC_LIST5 是 List Control
}

BEGIN_MESSAGE_MAP(CTreeListDlg, CDialogEx)
    ON_MESSAGE(WM_SERVER_INITIALIZED, &CTreeListDlg::OnServerInitialized)
    ON_MESSAGE(WM_CLIENT_CONNECTED, &CTreeListDlg::OnClientConnected)
    ON_MESSAGE(WM_UPDATE_FILE_LIST, &CTreeListDlg::OnUpdateFileList)
END_MESSAGE_MAP()

void CTreeListDlg::InitializeWinsock()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        CString errMsg;
        errMsg.Format(_T("WSAStartup failed with error: %d"), result);
        PostMessage(WM_SERVER_ERROR, 0, (LPARAM)new CString(errMsg));
        return;
    }

    m_winsockInitialized = true;
    PostMessage(WM_SERVER_INITIALIZED, (WPARAM)TRUE, 0);

    StartServer();
}

void CTreeListDlg::StartServer()
{
    if (!m_winsockInitialized || m_serverRunning) return;

    m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_listenSocket == INVALID_SOCKET) {
        CString errMsg;
        errMsg.Format(_T("Socket creation failed: %d"), WSAGetLastError());
        PostMessage(WM_SERVER_ERROR, 0, (LPARAM)new CString(errMsg));
        return;
    }

    // 设置地址重用
    BOOL reuseAddr = TRUE;
    if (setsockopt(m_listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseAddr, sizeof(reuseAddr)) == SOCKET_ERROR) {
        CString errMsg;
        errMsg.Format(_T("Set socket option failed: %d"), WSAGetLastError());
        PostMessage(WM_SERVER_ERROR, 0, (LPARAM)new CString(errMsg));
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
        return;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        CString errMsg;
        errMsg.Format(_T("Bind failed: %d"), WSAGetLastError());
        PostMessage(WM_SERVER_ERROR, 0, (LPARAM)new CString(errMsg));
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
        return;
    }

    if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        CString errMsg;
        errMsg.Format(_T("Listen failed: %d"), WSAGetLastError());
        PostMessage(WM_SERVER_ERROR, 0, (LPARAM)new CString(errMsg));
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
        return;
    }

    m_serverRunning = true;
    std::thread serverThread(&CTreeListDlg::ServerThread, this);
    serverThread.detach();
}

void CTreeListDlg::ServerThread()
{
    fd_set readSet;
    timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    while (m_serverRunning) {
        FD_ZERO(&readSet);
        FD_SET(m_listenSocket, &readSet);

        int result = select(0, &readSet, nullptr, nullptr, &timeout);
        if (result == SOCKET_ERROR) {
            if (m_serverRunning) {
                CString errMsg;
                errMsg.Format(_T("Select failed: %d"), WSAGetLastError());
                PostMessage(WM_SERVER_ERROR, 0, (LPARAM)new CString(errMsg));
            }
            break;
        }

        if (result > 0 && FD_ISSET(m_listenSocket, &readSet)) {
            SOCKET clientSocket = accept(m_listenSocket, nullptr, nullptr);
            if (clientSocket == INVALID_SOCKET) {
                if (m_serverRunning) {
                    CString errMsg;
                    errMsg.Format(_T("Accept failed: %d"), WSAGetLastError());
                    PostMessage(WM_SERVER_ERROR, 0, (LPARAM)new CString(errMsg));
                }
                continue;
            }

            std::thread clientThread(&CTreeListDlg::HandleClient, this, clientSocket);
            clientThread.detach();
        }
    }
}

void CTreeListDlg::HandleClient(SOCKET clientSocket)
{
    char buffer[BUFFER_SIZE];
    int bytesReceived=0;

    // 设置接收超时
    DWORD timeout = 5000; // 5秒
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

    while (m_serverRunning && (bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        CString receivedData(buffer);

        // 验证数据格式
        if (receivedData.Find(_T(',')) != -1 && receivedData.Find(_T('\n')) != -1) {
            CString* pData = new CString(receivedData);
            PostMessage(WM_UPDATE_FILE_LIST, 0, (LPARAM)pData);
        }
    }

    if (bytesReceived == SOCKET_ERROR && m_serverRunning) {
        CString errMsg;
        errMsg.Format(_T("Receive error: %d"), WSAGetLastError());
        PostMessage(WM_SERVER_ERROR, 0, (LPARAM)new CString(errMsg));
    }

    closesocket(clientSocket);
}

void CTreeListDlg::StopServer()
{
    if (!m_serverRunning) return;

    m_serverRunning = false;

    // 关闭监听套接字
    if (m_listenSocket != INVALID_SOCKET) {
        shutdown(m_listenSocket, SD_BOTH);
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
    }

    WSACleanup();
    m_winsockInitialized = false;
}

LRESULT CTreeListDlg::OnServerInitialized(WPARAM wParam, LPARAM lParam)
{
    if (wParam) {
        MessageBox(_T("服务器初始化成功，正在监听端口1234"), _T("成功"), MB_OK);
    }
    else {
        MessageBox(_T("服务器初始化失败"), _T("错误"), MB_ICONERROR);
    }
    return 0;
}

LRESULT CTreeListDlg::OnClientConnected(WPARAM wParam, LPARAM lParam)
{
    CString* pMsg = reinterpret_cast<CString*>(lParam);
    if (pMsg) {
        MessageBox(*pMsg, _T("客户端连接"), MB_OK);
        delete pMsg;
    }
    return 0;
}
LRESULT CTreeListDlg::OnUpdateFileList(WPARAM wParam, LPARAM lParam)
{
    CString* pData = reinterpret_cast<CString*>(lParam);
    if (!pData) return 0;

    CString data = *pData;
    delete pData;

    // 解析数据
    int pos = 0;
    CString line;
    while ((line = data.Tokenize(_T("\n"), pos)) != _T("")) {
        int subPos = 0;
        CString name = line.Tokenize(_T(","), subPos);
        CString size = line.Tokenize(_T(","), subPos);
        CString date = line.Tokenize(_T(","), subPos);

        if (!name.IsEmpty() && !size.IsEmpty() && !date.IsEmpty()) {
            // 更新UI
            int index = m_listCtrl.GetItemCount();
            m_listCtrl.InsertItem(index, name);
            m_listCtrl.SetItemText(index, 1, size);
            m_listCtrl.SetItemText(index, 2, date);

            // 线程安全地存储数据
            std::lock_guard<std::mutex> lock(g_dataMutex);
            g_fileData.emplace_back(name, size, date);
        }
    }

    return 0;
}

