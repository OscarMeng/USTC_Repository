// AddContentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicBalls.h"
#include "AddContentDlg.h"
#include "afxdialogex.h"
#include "MyConstants.h"
// CAddContentDlg dialog

IMPLEMENT_DYNAMIC(CAddContentDlg, CDialogEx)

CAddContentDlg::CAddContentDlg(CString content,CString repeatTimes,CWnd* pParent /*=NULL*/)
	:m_sContent(content),
	m_sRepeatTimes(repeatTimes),
	CDialogEx(CAddContentDlg::IDD, pParent)
{
	
}

CAddContentDlg::~CAddContentDlg()
{
}

void CAddContentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ePath);
	DDX_Control(pDX, IDC_EDIT2, m_eRepeatTimes);
}


BEGIN_MESSAGE_MAP(CAddContentDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CAddContentDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_OK, &CAddContentDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CAddContentDlg::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CAddContentDlg message handlers


BOOL CAddContentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CString path;
	if(!m_sContent.IsEmpty())
	{
		path = CURVEDATA_PATH+m_sContent;
	}
	else
		path = "";
	m_ePath.SetWindowText(path);
	m_eRepeatTimes.SetWindowText(m_sRepeatTimes);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAddContentDlg::OnBnClickedBtnOpen()
{
	// TODO: Add your control notification handler code here
	CFileDialog fileDialog(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Ini Files(*.ini)|*.ini||"));
	fileDialog.m_ofn.lpstrInitialDir = CURVEDATA_PATH;	
	if(fileDialog.DoModal() == IDOK)
	{
		CString path = fileDialog.GetPathName();
		m_sContent = fileDialog.GetFileTitle();
		m_ePath.SetWindowText(path);
	}
}


void CAddContentDlg::OnBnClickedBtnOk()
{
	// TODO: Add your control notification handler code here
	CString path;
	m_ePath.GetWindowText(path);
	CString type;
	GetPrivateProfileString("Type","type","",type.GetBuffer(100),100,path);
	type.ReleaseBuffer();
	type.Trim();
	if(type != "curveData")
	{
		CString msg;
		msg.Format("文件[%s]格式不正确！",path);
		AfxMessageBox(msg);
	//	return ;
	}
	int index1 = 0;
	int index2 = 0;
	for(int i = 0;i < path.GetLength();i++)
	{
		if(path[i] == '/' || path[i] == '\\')
			index1 = i;
		if(path[i] == '.')
			index2 = i;
	}
	m_sContent = path.Mid(index1+1,index2-index1-1);

	m_eRepeatTimes.GetWindowText(m_sRepeatTimes);
	if(m_sRepeatTimes.IsEmpty())
	{
		AfxMessageBox("循环次数不能为空！");
		return ;
	}

	int nRepeatTimes = atoi(m_sRepeatTimes);
	if(nRepeatTimes < 0)
		nRepeatTimes = 0;
	else if(nRepeatTimes > 99)
		nRepeatTimes = 99;
	m_sRepeatTimes.Format("%d",nRepeatTimes);

	CDialogEx::OnOK();
}


void CAddContentDlg::OnBnClickedBtnCancel()
{
	// TODO: Add your control notification handler code here

	CDialogEx::OnCancel();
}


