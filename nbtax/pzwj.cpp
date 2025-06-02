#include "pch.h"
#include "nbtax.h"
#include "afxdialogex.h"
#include "pzwj.h"
#include <atlconv.h> 

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

// pzwj 对话框
IMPLEMENT_DYNAMIC(pzwj, CDialogEx)

pzwj::pzwj(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG2, pParent)
{
}

pzwj::~pzwj()
{
}

void pzwj::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(pzwj, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_TEST, &pzwj::OnBnClickedButtonTest)
    ON_EN_CHANGE(IDC_EDIT1, &pzwj::OnEnChangeEdit1)
    ON_EN_CHANGE(IDC_EDIT2, &pzwj::OnEnChangeEdit2)
END_MESSAGE_MAP()

// Winsock初始化在OnInitDialog函数中
BOOL pzwj::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        AfxMessageBox(_T("Winsock初始化失败"));
        return FALSE;
    }

    return TRUE;
}

void pzwj::OnBnClickedButtonTest()
{
    // 获取地址和端口
    CString addressStr, portStr;
    GetDlgItemText(IDC_EDIT_ADDRESS, addressStr);
    GetDlgItemText(IDC_EDIT_PORT, portStr);

    // 检查地址和端口是否为空
    if (addressStr.IsEmpty() || portStr.IsEmpty())
    {
        AfxMessageBox(_T("请输入有效的地址和端口"));
        return;
    }

    // 初始化Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        CString errorMsg;
        errorMsg.Format(_T("Winsock初始化失败，错误代码: %d"), WSAGetLastError());
        AfxMessageBox(errorMsg);
        return;
    }

    // 创建监听Socket
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET)
    {
        CString errorMsg;
        errorMsg.Format(_T("监听Socket创建失败，错误代码: %d"), WSAGetLastError());
        AfxMessageBox(errorMsg);
        WSACleanup();
        return;
    }

    sockaddr_in listenAddr;
    listenAddr.sin_family = AF_INET;
    listenAddr.sin_port = htons(_ttoi(portStr));
    listenAddr.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(listenSocket, (sockaddr*)&listenAddr, sizeof(listenAddr)) == SOCKET_ERROR)
    {
        CString errorMsg;
        errorMsg.Format(_T("绑定端口失败，错误代码: %d"), WSAGetLastError());
        AfxMessageBox(errorMsg);
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    if (listen(listenSocket, 5) == SOCKET_ERROR)
    {
        CString errorMsg;
        errorMsg.Format(_T("监听端口失败，错误代码: %d"), WSAGetLastError());
        AfxMessageBox(errorMsg);
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    // 检测端口是否被监听（这里其实监听Socket已创建并绑定监听，此步骤可用于额外确认）
    SOCKET testSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (testSocket == INVALID_SOCKET)
    {
        CString errorMsg;
        errorMsg.Format(_T("测试Socket创建失败，错误代码: %d"), WSAGetLastError());
        AfxMessageBox(errorMsg);
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    sockaddr_in testAddr;
    testAddr.sin_family = AF_INET;
    testAddr.sin_port = htons(_ttoi(portStr));
    if (inet_pton(AF_INET, CT2CA(addressStr), &testAddr.sin_addr) != 1)
    {
        CString errorMsg;
        errorMsg.Format(_T("IP地址格式错误或转换失败，错误代码: %d"), WSAGetLastError());
        AfxMessageBox(errorMsg);
        closesocket(testSocket);
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    int testResult = connect(testSocket, (sockaddr*)&testAddr, sizeof(testAddr));
    closesocket(testSocket);

    if (testResult == SOCKET_ERROR)
    {
        int errorCode = WSAGetLastError();
        if (errorCode == WSAECONNREFUSED)
        {
            CString errorMsg;
            errorMsg.Format(_T("端口未被成功监听，错误代码: %d"), errorCode);
            AfxMessageBox(errorMsg);
            closesocket(listenSocket);
            WSACleanup();
            return;
        }
        else
        {
            CString errorMsg;
            errorMsg.Format(_T("连接测试失败，错误代码: %d"), errorCode);
            AfxMessageBox(errorMsg);
            closesocket(listenSocket);
            WSACleanup();
            return;
        }
    }

    // 创建用于连接的Socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        CString errorMsg;
        errorMsg.Format(_T("Socket创建失败，错误代码: %d"), WSAGetLastError());
        AfxMessageBox(errorMsg);
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_ttoi(portStr));
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) != 1)
    {
        CString errorMsg;
        errorMsg.Format(_T("本地回环地址转换失败，错误代码: %d"), WSAGetLastError());
        AfxMessageBox(errorMsg);
        closesocket(clientSocket);
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    // 连接本地回环地址的测试端口
    int result = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR)
    {
        CString errorMsg;
        errorMsg.Format(_T("连接本地回环地址的端口失败，错误代码: %d"), WSAGetLastError());
        AfxMessageBox(errorMsg);
    }
    else
    {
        // 拼接连接成功的提示信息
        CString infoMsg;
        infoMsg.Format(_T("域名/IP =>上线信息=>\n上线域名=>%s\n解析地址=>%s\n连接端口=> %s\n连接状态=>连接成功..."),
            addressStr,  addressStr, portStr);
        AfxMessageBox(infoMsg);
    }

    // 关闭Socket并释放Winsock资源，无论连接成功或失败
    closesocket(clientSocket);
    closesocket(listenSocket);
    WSACleanup();
}

void pzwj::OnEnChangeEdit1()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
}

void pzwj::OnEnChangeEdit2()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
}
