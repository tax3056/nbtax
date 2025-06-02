#pragma once
#include "afxdialogex.h"


// pzkl 对话框

class pzkl : public CDialogEx
{
	DECLARE_DYNAMIC(pzkl)

public:
	pzkl(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~pzkl();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
