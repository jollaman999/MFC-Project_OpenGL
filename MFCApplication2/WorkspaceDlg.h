#include "resource.h"

#pragma once


// CWorkspaceDlg ��ȭ �����Դϴ�.

class CWorkspaceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWorkspaceDlg)

public:
	CWorkspaceDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CWorkspaceDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WORKSPACE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
