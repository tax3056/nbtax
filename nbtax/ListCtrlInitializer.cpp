// ListCtrlInitializer.cpp
#include "ListCtrlInitializer.h"

void CListCtrlInitializer::InitializeWiznextList(CListCtrl* pListCtrl)
{
    if (pListCtrl) {
        pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        pListCtrl->InsertColumn(0, _T("����-IP"), LVCFMT_LEFT, 140);
        pListCtrl->InsertColumn(1, _T("����-IP"), LVCFMT_LEFT, 140);
        pListCtrl->InsertColumn(2, _T("ϵͳ"), LVCFMT_LEFT, 160);
        pListCtrl->InsertColumn(3, _T("������"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(4, _T("�û���"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(5, _T("����λ��"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(6, _T("CPU"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(7, _T("�ڴ�"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(8, _T("Ӳ��"), LVCFMT_LEFT, 100);
        pListCtrl->InsertColumn(9, _T("��Ƶ"), LVCFMT_LEFT, 180);  
        pListCtrl->InsertColumn(10, _T("�ӳ�"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(11, _T("��ע"), LVCFMT_LEFT, 100);
        pListCtrl->InsertColumn(12, _T("��װ����"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(13, _T("ɱ��"), LVCFMT_LEFT, 180);
    }
}

void CListCtrlInitializer::InitializeList1List(CListCtrl* pListCtrl)
{
    if (pListCtrl) {
        pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        pListCtrl->InsertColumn(0, _T("ʱ��"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(1, _T("����"), LVCFMT_LEFT, 180);
        pListCtrl->InsertColumn(2, _T("�¼�"), LVCFMT_LEFT, 260);
    }
}