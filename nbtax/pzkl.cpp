// pzkl.cpp: 实现文件
//

#include "pch.h"
#include "nbtax.h"
#include "afxdialogex.h"
#include "pzkl.h"


// pzkl 对话框

IMPLEMENT_DYNAMIC(pzkl, CDialogEx)

pzkl::pzkl(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

pzkl::~pzkl()
{
}

void pzkl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(pzkl, CDialogEx)
END_MESSAGE_MAP()


// pzkl 消息处理程序
