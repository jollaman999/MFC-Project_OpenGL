// WorkspaceDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication2.h"
#include "WorkspaceDlg.h"
#include "afxdialogex.h"


// CWorkspaceDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CWorkspaceDlg, CDialogEx)

CWorkspaceDlg::CWorkspaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWorkspaceDlg::IDD, pParent)
{

}

CWorkspaceDlg::~CWorkspaceDlg()
{
}

void CWorkspaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWorkspaceDlg, CDialogEx)
END_MESSAGE_MAP()