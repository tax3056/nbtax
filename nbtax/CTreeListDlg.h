#pragma once
#include "afxdialogex.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

#pragma comment(lib, "ws2_32.lib")

// 线程安全的FileInfo结构
struct FileInfo {
    CString name;
    CString size;
    CString date;
    FileInfo() : name(_T("")), size(_T("")), date(_T("")) {}
    FileInfo(const CString& n, const CString& s, const CString& d) : name(n), size(s), date(d) {}
};

class CTreeListDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CTreeListDlg)

public:
    CTreeListDlg(CWnd* pParent = nullptr);
    virtual ~CTreeListDlg();

    void InitializeWinsock();
    CListCtrl m_listCtrl;
    SOCKET m_listenSocket;
    std::atomic<bool> m_winsockInitialized;
    std::atomic<bool> m_serverRunning;


protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    // 自定义消息
    enum {
        WM_SERVER_INITIALIZED = WM_USER + 100,
        WM_CLIENT_CONNECTED = WM_USER + 101,
        WM_UPDATE_FILE_LIST = WM_USER + 102,
        WM_SERVER_ERROR = WM_USER + 103
    };

    DECLARE_MESSAGE_MAP()

    // 服务器相关

    void StartServer();
    void StopServer();
    void ServerThread();
    void HandleClient(SOCKET clientSocket);

    // 消息处理函数
    LRESULT OnServerInitialized(WPARAM wParam, LPARAM lParam);
    LRESULT OnClientConnected(WPARAM wParam, LPARAM lParam);
    LRESULT OnUpdateFileList(WPARAM wParam, LPARAM lParam);

    // 测试数据
    void InsertTestData();

private:

};