#include "CnbtaxDlg.h"
#include <afxdialogex.h>

IMPLEMENT_DYNAMIC(CnbtaxDlg, CDialogEx)

CnbtaxDlg::CnbtaxDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MY_DIALOG, pParent)
{
}

CnbtaxDlg::~CnbtaxDlg()
{
}

void CnbtaxDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CnbtaxDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CnbtaxDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    if (!m_networkManager.Initialize())
    {
        return FALSE;
    }

    if (!m_networkManager.StartListening())
    {
        return FALSE;
    }

    CListCtrlInitializer::InitializeWiznextList((CListCtrl*)GetDlgItem(ID_WIZNEXT));
    CListCtrlInitializer::InitializeList1List((CListCtrl*)GetDlgItem(IDC_LIST1));

    // 将“关于...”菜单项添加到系统菜单中。
    // IDM_ABOUTBOX 必须在系统命令范围内。
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);         // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标

    // 启动一个线程来处理客户端连接
    CClientConnectionHandler handler(this);
    AfxBeginThread([](LPVOID pParam) {
        CClientConnectionHandler* pHandler = (CClientConnectionHandler*)pParam;
        SOCKET listenSocket = ((CnbtaxDlg*)pHandler->GetDlg())->m_networkManager.GetListenSocket();
        pHandler->HandleConnections(listenSocket);
        return 0;
        }, &handler);

    return TRUE;
}