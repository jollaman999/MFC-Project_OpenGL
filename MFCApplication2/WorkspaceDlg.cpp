// WorkspaceDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication2.h"
#include "WorkspaceDlg.h"
#include "afxdialogex.h"


// CWorkspaceDlg 대화 상자입니다.

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