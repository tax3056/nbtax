#pragma once
#include "afxdialogex.h"
#include <winsock2.h>  // 包含Winsock库头文件
#pragma comment(lib, "ws2_32.lib")  // 链接Winsock库

// pzwj 对话框
class pzwj : public CDialogEx
{
    DECLARE_DYNAMIC(pzwj)

public:
    pzwj(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~pzwj();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG2 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();  // 重写OnInitDialog函数用于初始化Winsock

    DECLARE_MESSAGE_MAP()

    // 按钮点击事件处理函数声明
    afx_msg void OnBnClickedButtonTest();
public:
    afx_msg void OnEnChangeEdit1();
    afx_msg void OnEnChangeEdit2();
};