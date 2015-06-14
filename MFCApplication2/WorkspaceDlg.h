#include "resource.h"

#pragma once


// CWorkspaceDlg 대화 상자입니다.

class CWorkspaceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWorkspaceDlg)

public:
	CWorkspaceDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWorkspaceDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WORKSPACE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
