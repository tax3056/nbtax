#include <afxwin.h>
#include <afxdialogex.h>
// nbtaxDlg.h: 头文件

#pragma once
// CnbtaxDlg 对话框
class CnbtaxDlg : public CDialogEx
{
// 构造
public:
	
	SOCKET GetListenSocket();
	SOCKET m_listenSocket; // 假设添加监听套接字的成员变量
	CnbtaxDlg(CWnd* pParent = nullptr);	// 标准构造函数
private:
	CMenu m_menu;



// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NBTAX_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	

	afx_msg void OnClickedButton1();


public:



	afx_msg void OnBnClickedMfcmenubutton1();
	afx_msg void OnNMRClickWiznext(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void screen();
	afx_msg void filemanger();
};
