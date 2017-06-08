#pragma once
#include "afxwin.h"


// CAddContentDlg dialog

class CAddContentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddContentDlg)

public:
	CAddContentDlg(CString content,CString repeatTimes,CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddContentDlg();

// Dialog Data
	enum { IDD = IDD_ADD_CONTENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();

public:
	CString m_sContent;
	CString	m_sRepeatTimes;

	CEdit m_ePath;
	CEdit m_eRepeatTimes;
};
