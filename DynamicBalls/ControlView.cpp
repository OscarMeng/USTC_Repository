// ControlView.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicBalls.h"
#include "3DView.h"
#include "DynamicBallsView.h"
#include "ControlView.h"
#include "MyConstants.h"
#include "AddContentDlg.h"
#include "DynamicCurve.h"
#include "CurveData.h"

// CControlView

IMPLEMENT_DYNCREATE(CControlView, CFormView)

CControlView::CControlView()
	: CFormView(CControlView::IDD)
{
	m_bListInited = false;
	m_bCtrlsValid = false;

	m_bStopped = true;
	m_bWaiting = false;
	m_nNextListIndex = 0;
	m_nCurrentListIndex = 0;
	m_nRunMode = MODE_CONTENTS;
	m_nPlayIndex = 0;
	m_nFrameIndex = 0;
	m_nRepeatIndex = 0;
	m_nFrameTotal = 0;
	m_nRepeatTotal = 0;
}

CControlView::~CControlView()
{
}

void CControlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CONTENT, m_clContent);
	DDX_Control(pDX, IDC_BTN_ADD, m_btAdd);
	DDX_Control(pDX, IDC_BTN_MODIFY, m_btModify);
	DDX_Control(pDX, IDC_BTN_DELETE, m_btDelete);
	DDX_Control(pDX, IDC_STATIC_RUNCONTENTS, m_stRunContents);
	DDX_Control(pDX, IDC_BTN_BAR1, m_btBar1);
	DDX_Control(pDX, IDC_RADIO_ORDER, m_rbOrder);
	DDX_Control(pDX, IDC_RADIO_RAND, m_rbRand);
	DDX_Control(pDX, IDC_BTN_RUN, m_btRun);
	DDX_Control(pDX, IDC_BTN_STOP, m_btStop);
	DDX_Control(pDX, IDC_STATIC_MANUALLY, m_stManually);
	DDX_Control(pDX, IDC_BTN_LIMIT, m_btLimit);
	DDX_Control(pDX, IDC_BTN_LIMITTOZERO, m_btLimitToZero);
	DDX_Control(pDX, IDC_BTN_BACKTOZERO, m_btBackToZero);
	DDX_Control(pDX, IDC_BTN_MIDDLE, m_btMiddle);
	DDX_Control(pDX, IDC_STATIC_POS, m_stPos);
	DDX_Control(pDX, IDC_STATIC_SPEED, m_stSpeed);
	DDX_Control(pDX, IDC_EDIT_POS, m_ePos);
	DDX_Control(pDX, IDC_EDIT_SPEED, m_eTime);
	DDX_Control(pDX, IDC_BTN_DO, m_btDo);
	DDX_Control(pDX, IDC_BTN_BAR2, m_btBar2);
	DDX_Control(pDX, IDC_BTN_SHUTDOWN, m_btShutDown);
	DDX_Control(pDX, IDC_CHECK_LOOP, m_chLoop);
	DDX_Control(pDX, IDC_CHECK_AUTO, m_chAutoRotation);
}

BEGIN_MESSAGE_MAP(CControlView, CFormView)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_ADD, &CControlView::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CControlView::OnBnClickedBtnModify)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CControlView::OnBnClickedBtnDelete)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CONTENT, &CControlView::OnNMClickListContent)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CONTENT, &CControlView::OnNMDblclkListContent)
	ON_BN_CLICKED(IDC_BTN_RUN, &CControlView::OnBnClickedBtnRun)
	ON_BN_CLICKED(IDC_BTN_STOP, &CControlView::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_LIMIT, &CControlView::OnBnClickedBtnLimit)
	ON_BN_CLICKED(IDC_BTN_LIMITTOZERO, &CControlView::OnBnClickedBtnLimitToZero)
	ON_BN_CLICKED(IDC_BTN_BACKTOZERO, &CControlView::OnBnClickedBtnBackToZero)
	ON_BN_CLICKED(IDC_BTN_MIDDLE, &CControlView::OnBnClickedBtnMiddle)
	ON_BN_CLICKED(IDC_BTN_DO, &CControlView::OnBnClickedBtnDo)
	ON_BN_CLICKED(IDC_BTN_SHUTDOWN, &CControlView::OnBnClickedBtnShutdown)
	ON_BN_CLICKED(IDC_CHECK_AUTO, &CControlView::OnBnClickedCheckAuto)
END_MESSAGE_MAP()


// CControlView diagnostics

#ifdef _DEBUG
void CControlView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CControlView message handlers


void CControlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	if(!m_bListInited)
	{
		m_clContent.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE);
		//SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_ONECLICKACTIVATE);
		CImageList   m_imageList; 
		m_imageList.Create(1,20,ILC_COLOR24,1,1);   //1是图片宽，30是高度 
		m_clContent.SetImageList(&m_imageList,LVSIL_SMALL); 
		m_clContent.InsertColumn(0,NULL,LVCFMT_CENTER,0);
		m_clContent.InsertColumn(1,_T("档案"),LVCFMT_CENTER,220);
		m_clContent.InsertColumn(2,_T("重复次数"),LVCFMT_CENTER,50);
		m_clContent.DeleteColumn(0);

		ReadContents();

		m_bListInited = true;
	}

	m_btRun.EnableWindow(true);
	m_btStop.EnableWindow(false);

	m_rbOrder.SetCheck(true);
	m_rbRand.SetCheck(false);

	m_bCtrlsValid = true;
}


void CControlView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if(nIDEvent == IDT_RUN)
	{
		PlayRun();
	}

	CFormView::OnTimer(nIDEvent);
}


void CControlView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if(m_bCtrlsValid)
		LayoutCtrls();
	// TODO: Add your message handler code here
}


void CControlView::LayoutCtrls()
{
	/*
	int height = 0;
	CRect rect;
	GetClientRect(&rect);
	height += m_nTopMargin;
	m_clContent.MoveWindow(m_nTopMargin,height,rect.Width()-m_nLeftMargin-m_nRightMargin,rect.Height()-m_nAllHeight);
	m_clContent.SetColumnWidth(0,(rect.Width()-m_nLeftMargin-m_nRightMargin)/3*2);
	m_clContent.SetColumnWidth(1,(rect.Width()-m_nLeftMargin-m_nRightMargin)/3-10);
	m_clContent.Invalidate();
	height += rect.Height()-m_nAllHeight+m_nCtrlSpace;
	m_btAdd.MoveWindow(m_nLeftMargin,height,(rect.Width()-m_nLeftMargin-m_nRightMargin)/3,m_nButtonHeight);
	m_btModify.MoveWindow(m_nLeftMargin+(rect.Width()-m_nLeftMargin-m_nRightMargin)/3,height,(rect.Width()-m_nLeftMargin-m_nRightMargin)/3,m_nButtonHeight);
	m_btDelete.MoveWindow(m_nLeftMargin+(rect.Width()-m_nLeftMargin-m_nRightMargin)/3*2,height,(rect.Width()-m_nLeftMargin-m_nRightMargin)/3,m_nButtonHeight);
	height += m_nButtonHeight+m_nBarSpace;
	m_btBar1.MoveWindow(-m_nLeftMargin,height,rect.Width()+m_nLeftMargin+m_nRightMargin,m_nBarHeight);
	height += m_nBarHeight+m_nCtrlSpace;
	m_stRunContents.MoveWindow(m_nLeftMargin,height,rect.Width()-m_nLeftMargin-m_nRightMargin,m_nStaticHeight+m_nCtrlSpace+m_nButtonHeight);
	m_rbOrder.MoveWindow(m_nLeftMargin+m_nCtrlSpace,height+m_nStaticHeight,(rect.Width()-m_nLeftMargin-m_nRightMargin)/3-15,m_nButtonHeight);
	m_rbRand.MoveWindow(m_nLeftMargin+m_nCtrlSpace+(rect.Width()-m_nLeftMargin-m_nRightMargin)/3-15,height+m_nStaticHeight,(rect.Width()-m_nLeftMargin-m_nRightMargin)/3-15,m_nButtonHeight);
	m_btRun.MoveWindow(rect.Width()-m_nRightMargin-m_nButtonWidth*2-m_nCtrlSpace,height+m_nStaticHeight,m_nButtonWidth,m_nButtonHeight);
	m_btStop.MoveWindow(rect.Width()-m_nRightMargin-m_nButtonWidth-m_nCtrlSpace,height+m_nStaticHeight,m_nButtonWidth,m_nButtonHeight);
	height += m_nStaticHeight+m_nCtrlSpace+m_nButtonHeight+m_nCtrlSpace;
	m_stManually.MoveWindow(m_nLeftMargin,height,rect.Width()-m_nLeftMargin-m_nRightMargin,m_nButtonHeight+m_nStaticHeight*2+m_nCtrlSpace*5);	
	m_btLimit.MoveWindow(m_nLeftMargin+m_nCtrlSpace,height+m_nStaticHeight,(rect.Width()-m_nLeftMargin-m_nRightMargin-m_nCtrlSpace*2)/4,m_nButtonHeight);
	m_btLimitToZero.MoveWindow(m_nLeftMargin+m_nCtrlSpace+(rect.Width()-m_nLeftMargin-m_nRightMargin-m_nCtrlSpace*2)/4,height+m_nStaticHeight,(rect.Width()-m_nLeftMargin-m_nRightMargin-m_nCtrlSpace*2)/4,m_nButtonHeight);
	m_btBackToZero.MoveWindow(m_nLeftMargin+m_nCtrlSpace+(rect.Width()-m_nLeftMargin-m_nRightMargin-m_nCtrlSpace*2)/4*2,height+m_nStaticHeight,(rect.Width()-m_nLeftMargin-m_nRightMargin-m_nCtrlSpace*2)/4,m_nButtonHeight);
	m_btMiddle.MoveWindow(m_nLeftMargin+m_nCtrlSpace+(rect.Width()-m_nLeftMargin-m_nRightMargin-m_nCtrlSpace*2)/4*3,height+m_nStaticHeight,(rect.Width()-m_nLeftMargin-m_nRightMargin-m_nCtrlSpace*2)/4,m_nButtonHeight);
	height += m_nStaticHeight+m_nButtonHeight+m_nCtrlSpace;
	m_stPos.MoveWindow(m_nLeftMargin+m_nCtrlSpace,height+2,m_nButtonWidth*2,m_nStaticHeight);
	m_ePos.MoveWindow(m_nLeftMargin+m_nButtonWidth*2+m_nCtrlSpace,height,rect.Width()-m_nLeftMargin-m_nRightMargin-m_nButtonWidth*4,m_nStaticHeight);
	m_stSpeed.MoveWindow(m_nLeftMargin+m_nCtrlSpace,height+m_nStaticHeight+m_nCtrlSpace+2,m_nButtonWidth*2,m_nStaticHeight);
	m_eTime.MoveWindow(m_nLeftMargin+m_nButtonWidth*2+m_nCtrlSpace,height+m_nStaticHeight+m_nCtrlSpace,rect.Width()-m_nLeftMargin-m_nRightMargin-m_nButtonWidth*4,m_nStaticHeight);
	m_btDo.MoveWindow(rect.Width()-m_nRightMargin-m_nCtrlSpace-m_nButtonWidth,height+m_nCtrlSpace,m_nButtonWidth,m_nButtonHeight);
	height += m_nStaticHeight*2+m_nCtrlSpace*3;
	m_btBar2.MoveWindow(-m_nLeftMargin,height,rect.Width()+m_nLeftMargin+m_nRightMargin,m_nBarHeight);
	height += m_nBarHeight+m_nCtrlSpace;

	m_btSetting.MoveWindow(10,height,m_nButtonWidth,m_nButtonHeight);
	m_chLoop.MoveWindow(10+m_nButtonWidth+m_nCtrlSpace,height,m_nButtonWidth*2,m_nButtonHeight);
	m_btShutDown.MoveWindow(rect.Width()-m_nCtrlSpace-m_nButtonWidth,height,m_nButtonWidth,m_nButtonHeight);

*/
	int width = 0;
	int height = 0;
	CRect rect;
	GetClientRect(&rect);
	width += m_nLeftMargin;
	m_clContent.MoveWindow(m_nTopMargin,width,rect.Width()-m_nLeftMargin-m_nRightMargin-m_nAllWidth,rect.Height()-m_nTopMargin-m_nBottomMargin);
	m_clContent.SetColumnWidth(0,(rect.Width()-m_nLeftMargin-m_nRightMargin-m_nAllWidth)/3*2);
	m_clContent.SetColumnWidth(1,(rect.Width()-m_nLeftMargin-m_nRightMargin-m_nAllWidth)/3-10);
	m_clContent.Invalidate();	
	width += rect.Width()-m_nAllWidth-m_nRightMargin+m_nBarSpace;
	height = m_nTopMargin+(rect.Height()-m_nTopMargin-m_nBottomMargin-m_nButtonHeight*3-m_nCtrlSpace*2)/2;
	m_btAdd.MoveWindow(width,height,m_nButtonWidth,m_nButtonHeight);
	m_btModify.MoveWindow(width,height+m_nButtonHeight+m_nCtrlSpace,m_nButtonWidth,m_nButtonHeight);
	m_btDelete.MoveWindow(width,height+m_nButtonHeight*2+m_nCtrlSpace*2,m_nButtonWidth,m_nButtonHeight);
	
	width += m_nButtonWidth+m_nBarSpace;
	m_btBar1.MoveWindow(width,-10,m_nBarWidth,rect.Height()+10);
	
	width += m_nBarWidth+m_nCtrlSpace;
	height = m_nTopMargin+m_nStaticHeight+(rect.Height()-m_nTopMargin-m_nBottomMargin-m_nStaticHeight*3-m_nButtonHeight*2-m_nCtrlSpace*3)/2;
	m_stRunContents.MoveWindow(width,m_nTopMargin,m_nStaticWidth1,rect.Height()-m_nTopMargin-m_nBottomMargin);
	m_rbOrder.MoveWindow(width+m_nCtrlSpace,height,m_nStaticWidth1-m_nCtrlSpace*2,m_nStaticHeight);
	m_rbRand.MoveWindow(width+m_nCtrlSpace,height+m_nStaticHeight+m_nCtrlSpace,m_nStaticWidth1-m_nCtrlSpace*2,m_nStaticHeight);
	m_btRun.MoveWindow(width+(m_nStaticWidth1-m_nButtonWidth)/2,height+m_nStaticHeight*2+m_nCtrlSpace*2,m_nButtonWidth,m_nButtonHeight);
	m_btStop.MoveWindow(width+(m_nStaticWidth1-m_nButtonWidth)/2,height+m_nStaticHeight*2+m_nButtonHeight+m_nCtrlSpace*3,m_nButtonWidth,m_nButtonHeight);
	
	width += m_nStaticWidth1+m_nCtrlSpace;
	height = m_nTopMargin+(rect.Height()-m_nTopMargin-m_nBottomMargin-m_nStaticHeight-m_nButtonHeight*3-m_nCtrlSpace*2)/2;
	m_stManually.MoveWindow(width,m_nTopMargin,m_nStaticWidth2,rect.Height()-m_nTopMargin-m_nBottomMargin);
	m_btLimit.MoveWindow(width+m_nCtrlSpace,height,(m_nStaticWidth2-m_nCtrlSpace*2)/4,m_nButtonHeight);
	m_btLimitToZero.MoveWindow(width+m_nCtrlSpace+(m_nStaticWidth2-m_nCtrlSpace*2)/4,height,(m_nStaticWidth2-m_nCtrlSpace*2)/4,m_nButtonHeight);
	m_btBackToZero.MoveWindow(width+m_nCtrlSpace+(m_nStaticWidth2-m_nCtrlSpace*2)/4*2,height,(m_nStaticWidth2-m_nCtrlSpace*2)/4,m_nButtonHeight);
	m_btMiddle.MoveWindow(width+m_nCtrlSpace+(m_nStaticWidth2-m_nCtrlSpace*2)/4*3,height,(m_nStaticWidth2-m_nCtrlSpace*2)/4,m_nButtonHeight);
	m_stPos.MoveWindow(width+m_nCtrlSpace,height+m_nButtonHeight+m_nCtrlSpace,m_nStaticWidth1,m_nStaticHeight);
	m_ePos.MoveWindow(width+m_nStaticWidth1+m_nCtrlSpace,height+m_nButtonHeight+m_nCtrlSpace,m_nStaticWidth2-m_nCtrlSpace*3-m_nStaticWidth1-m_nButtonWidth,m_nStaticHeight);
	m_stSpeed.MoveWindow(width+m_nCtrlSpace,height+m_nButtonHeight*2+m_nCtrlSpace*2+2,m_nStaticWidth1,m_nStaticHeight);
	m_eTime.MoveWindow(width+m_nStaticWidth1+m_nCtrlSpace,height+m_nButtonHeight*2+m_nCtrlSpace*2+2,m_nStaticWidth2-m_nCtrlSpace*3-m_nStaticWidth1-m_nButtonWidth,m_nStaticHeight);
	m_btDo.MoveWindow(width+m_nStaticWidth2-m_nCtrlSpace-m_nButtonWidth,height+m_nButtonHeight+m_nCtrlSpace,m_nButtonWidth,m_nButtonHeight);
	
	width += m_nStaticWidth2+m_nCtrlSpace;	
	m_btBar2.MoveWindow(width,-10,m_nBarWidth,rect.Height()+10);

	width += m_nBarWidth+m_nCtrlSpace;
	height = m_nTopMargin+(rect.Height()-m_nTopMargin-m_nBottomMargin-m_nButtonHeight*3-m_nCtrlSpace*2)/2;
	m_chAutoRotation.MoveWindow(width,height,m_nButtonWidth*2,m_nButtonHeight);
	m_chLoop.MoveWindow(width,height+m_nButtonHeight+m_nCtrlSpace,m_nButtonWidth*2,m_nButtonHeight);
	m_btShutDown.MoveWindow(width,height+m_nButtonHeight*2+m_nCtrlSpace*2,m_nButtonWidth,m_nButtonHeight);
}

void CControlView::ReadContents()
{
	m_clContent.DeleteAllItems();
	m_nItem = -1;
	int index = 0;

	FILE* ff;
	char buf[1000];
	CFileStatus status;
	errno_t err;
	if(CFile::GetStatus(CONTENTS_PATH,status))
	{
		err = fopen_s(&ff,CONTENTS_PATH,"rt");
		CString str;
		if(err != 0)
		{			
			str.Format("文件%s无法打开！",CONTENTS_PATH);
			AfxMessageBox(str);
			return; 
		}
		while(fgets(buf,sizeof(buf)-1,ff) != NULL)
		{		
			str = buf;
			str = str.Mid(3,str.GetLength()-4);
			m_clContent.InsertItem(index,str);
			str = buf;
			str = str.Mid(0,2);
			int repeatTimes = atoi(str);
			str.Format("%d",repeatTimes);
			m_clContent.SetItemText(index,1,str);
			index++;
		}
		fclose(ff);
	}
}


void CControlView::WriteContents()
{	
	FILE* ff;
	errno_t err;
	CString str;

	err = fopen_s(&ff,CONTENTS_PATH,"wt");
	if(err != 0)
	{
		str.Format("文件%s无法写入！",CONTENTS_PATH);
		AfxMessageBox(str);
		return; 
	}
	for(int i = 0;i < m_clContent.GetItemCount();i++)
	{
		str = m_clContent.GetItemText(i,1);
		int repeatTimes = atoi(str);
		CString content = m_clContent.GetItemText(i,0);
		fprintf(ff,"%02d-%s\n",repeatTimes,content);	
	}
	fclose(ff);
}


void CControlView::OnBnClickedBtnAdd()
{
	// TODO: Add your control notification handler code here
	CAddContentDlg addContentDlg("","");
	if(addContentDlg.DoModal() == IDOK)
	{
		int count = m_clContent.GetItemCount();
		m_clContent.InsertItem(count,addContentDlg.m_sContent);
		m_clContent.SetItemText(count,1,addContentDlg.m_sRepeatTimes);

		WriteContents();
	}
}


void CControlView::OnBnClickedBtnModify()
{
	// TODO: Add your control notification handler code here
	if(m_nItem < 0)
	{
		AfxMessageBox("您没有选中任何内容！");
		return;
	}
	CString content = m_clContent.GetItemText(m_nItem,0);
	CString repeatTimes = m_clContent.GetItemText(m_nItem,1);
	CAddContentDlg addContentDlg(content,repeatTimes);
	if(addContentDlg.DoModal() == IDOK)
	{
		m_clContent.SetItemText(m_nItem,0,addContentDlg.m_sContent);
		m_clContent.SetItemText(m_nItem,1,addContentDlg.m_sRepeatTimes);

		WriteContents();
	}
}


void CControlView::OnBnClickedBtnDelete()
{
	// TODO: Add your control notification handler code here
	if(m_nItem < 0)
	{
		AfxMessageBox("您没有选中任何内容！");
		return;
	}
	CString content = m_clContent.GetItemText(m_nItem,0);
	CString msg;
	msg.Format("您确实要删除\"%s\"吗?",content);
	if(AfxMessageBox(msg,MB_YESNO) == IDYES)
	{
		m_clContent.DeleteItem(m_nItem);
		m_nItem = -1;
		WriteContents();
	}
}


void CControlView::OnNMClickListContent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_nItem = pNMItemActivate->iItem;
	m_clContent.Invalidate();

	*pResult = 0;
}


void CControlView::OnNMDblclkListContent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_nItem = pNMItemActivate->iItem;
	m_clContent.Invalidate();
	if(m_nItem < 0)
		return ;
	// TODO: Add your control notification handler code here
	if(!m_bStopped)
	{
		m_bWaiting = true;
		m_nNextListIndex = m_nItem;
		return ;
	}

	PlayBegin();

	KillTimer(IDT_RUN);
	m_nRunMode = MODE_CONTENTS;
	m_nPlayIndex = 0;
	m_clContent.SetItemColor(m_nCurrentListIndex,RGB(0,0,0));
	m_nCurrentListIndex = m_nItem;
	m_clContent.SetItemColor(m_nCurrentListIndex,RGB(255,0,0));
	m_clContent.Invalidate();
	SetTimer(IDT_RUN,100,NULL);

	*pResult = 0;
}


void CControlView::OnBnClickedBtnRun()
{
	// TODO: Add your control notification handler code here
	if(m_clContent.GetItemCount() <= 0)
	{
		AfxMessageBox("请添加档案！");
		return ;
	}

	PlayBegin();

	KillTimer(IDT_RUN);
	m_nRunMode = MODE_CONTENTS;
	m_nPlayIndex = 0;
	m_clContent.SetItemColor(m_nCurrentListIndex,RGB(255,0,0));
	m_clContent.Invalidate();
	SetTimer(IDT_RUN,100,NULL);
}


void CControlView::OnBnClickedBtnStop()
{
	// TODO: Add your control notification handler code here
	m_bStopped = true;
}

void CControlView::OnBnClickedBtnLimit()
{
	// TODO: Add your control notification handler code here
	PlayBegin();
	m_btStop.EnableWindow(false);
	KillTimer(IDT_RUN);
	m_nRunMode = MODE_LIMIT;
	m_nPlayIndex = 3;
	SetTimer(IDT_RUN,100,NULL);
}

void CControlView::OnBnClickedBtnLimitToZero()
{
	// TODO: Add your control notification handler code here
	PlayBegin();
	m_btStop.EnableWindow(false);
	KillTimer(IDT_RUN);
	m_nRunMode = MODE_LIMITTOZERO;
	m_nPlayIndex = 3;
	SetTimer(IDT_RUN,100,NULL);
}

void CControlView::OnBnClickedBtnBackToZero()
{
	// TODO: Add your control notification handler code here
	PlayBegin();
	m_btStop.EnableWindow(false);
	KillTimer(IDT_RUN);
	m_nRunMode = MODE_BACKTOZERO;
	m_nPlayIndex = 3;
	SetTimer(IDT_RUN,100,NULL);
}

void CControlView::OnBnClickedBtnMiddle()
{
	// TODO: Add your control notification handler code here
	PlayBegin();
	m_btStop.EnableWindow(false);
	KillTimer(IDT_RUN);
	m_nRunMode = MODE_MIDDLE;
	m_nPlayIndex = 3;
	SetTimer(IDT_RUN,100,NULL);
}

void CControlView::OnBnClickedBtnDo()
{
	// TODO: Add your control notification handler code here
	CString sPos;
	CString sTime;
	m_ePos.GetWindowText(sPos);
	m_eTime.GetWindowText(sTime);
	if(sPos.IsEmpty())
	{
		AfxMessageBox("位置不能为空！");
		return ;
	}
	if(sTime.IsEmpty())
	{
		AfxMessageBox("时间不能为空！");
		return ;
	}

	PlayBegin();
	KillTimer(IDT_RUN);
	m_nRunMode = MODE_MANUALLY;
	m_nPlayIndex = 3;
	SetTimer(IDT_RUN,100,NULL);
}


void CControlView::OnBnClickedBtnShutdown()
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox("您确定要关闭程序并关闭计算机吗？",MB_YESNO) == IDYES)
	{

	}
}

void CControlView::OnBnClickedCheckAuto()
{
	// TODO: Add your control notification handler code here
	if(m_chAutoRotation.GetCheck())
		m_pDynamicCurve->SetAutoRotation(true);
	else 
		m_pDynamicCurve->SetAutoRotation(false);
}

void CControlView::PlayRun()
{
	KillTimer(IDT_RUN);

	CTime tTime;
	switch(m_nPlayIndex)
	{
	case 0:
		m_nTimeTotal = m_pDynamicCurve->SetPosSpecialContents(MODE_MIDDLE);
		m_nPlayIndex ++;
	case 1:
//		DoRun();
		m_nStartTime = GetTickCount();
		m_nPlayIndex++;
	case 2:
		if((GetTickCount()-m_nStartTime)/1000.0 < (double)m_nTimeTotal)
		{
			SetColor();
			m_pDynamicBallsView->Invalidate();
			m_p3DView->InvalidateRect(NULL,false);
		}
		else
		{
			m_nPlayIndex++;
		}
		break;
	case 3:	
		m_nRepeatIndex = 0;
		m_nPlayIndex++;
	case 4:
		m_nFrameIndex = 0;
		m_nPlayIndex++;
	case 5:
		if(!SetValue())
			m_nPlayIndex = 20;
		else
			m_nPlayIndex++;
		break;
	case 6:
//		DoRun();
		m_nStartTime = GetTickCount();
		m_nPlayIndex++;
	case 7:
		if((GetTickCount()-m_nStartTime)/1000.0 < (double)m_nTimeTotal)
		{
			SetColor();
			m_pDynamicBallsView->Invalidate();
			m_p3DView->InvalidateRect(NULL,false);
		}
		else
		{
			m_nPlayIndex++;
		}
		break;
	case 8:
		m_nFrameIndex++;
		if(m_nFrameIndex < m_nFrameTotal)
		{	
			m_nPlayIndex = 5;
		}
		else
		{
			if(m_bStopped && m_nRunMode == MODE_CONTENTS)
			{
				PlayOver();
				return;
			}
			else if(m_bWaiting)
			{
				m_bWaiting = false;
				m_clContent.SetItemColor(m_nCurrentListIndex,RGB(0,0,0));
				m_nCurrentListIndex = m_nNextListIndex;
				m_clContent.SetItemColor(m_nCurrentListIndex,RGB(255,0,0));
				m_clContent.Invalidate();
				m_nPlayIndex = 0;
			}
			else
				m_nPlayIndex++;
		}
		break;
	case 9:
		m_nRepeatIndex++;
		if(m_nRepeatIndex < m_nRepeatTotal)
		{
			m_nPlayIndex = 4;
		}
		else
			m_nPlayIndex++;
		break;
	default:
		if(m_nRunMode != MODE_CONTENTS)
		{
			AfxMessageBox("运转完成");
			PlayOver();
			return ;
		}
		else
		{
			if(m_chLoop.GetCheck())
			{
				m_nPlayIndex = 3;
				break;
			}
			if(m_clContent.GetItemCount() == 0)
			{
				PlayOver();
				AfxMessageBox("请添加档案！");
				return ;
			}

			m_nRunMode = MODE_CONTENTS;
			PlayBegin();

			m_clContent.SetItemColor(m_nCurrentListIndex,RGB(0,0,0));
			if(m_rbOrder.GetCheck())
			{			
				m_nCurrentListIndex = (m_nCurrentListIndex+1)%m_clContent.GetItemCount();	
			}
			else
			{
				srand((unsigned)time(NULL)); 
				m_nCurrentListIndex = rand()%m_clContent.GetItemCount();
			}
			m_clContent.SetItemColor(m_nCurrentListIndex,RGB(255,0,0));
			m_clContent.Invalidate();

			m_nPlayIndex = 0;
		}
		break;
	}
	SetTimer(IDT_RUN,100,NULL);
}

void CControlView::PlayBegin()
{
	m_btRun.EnableWindow(false);
	m_btStop.EnableWindow(true);
	m_btLimit.EnableWindow(false);
	m_btLimitToZero.EnableWindow(false);
	m_btBackToZero.EnableWindow(false);
	m_btMiddle.EnableWindow(false);
	m_btDo.EnableWindow(false);

	m_bStopped = false;
}

void CControlView::PlayOver()
{
	m_btRun.EnableWindow(true);
	m_btStop.EnableWindow(false);
	m_btLimit.EnableWindow(true);
	m_btLimitToZero.EnableWindow(true);
	m_btBackToZero.EnableWindow(true);
	m_btMiddle.EnableWindow(true);
	m_btDo.EnableWindow(true);

	m_bStopped = true;
}

bool CControlView::SetValue()
{
	if(m_nRunMode == MODE_CONTENTS)
	{
		if(m_nRepeatIndex == 0 && m_nFrameIndex == 0)
		{
			if(m_nCurrentListIndex >= m_clContent.GetItemCount())
				return false;
			CString content = m_clContent.GetItemText(m_nCurrentListIndex,0);
			CString repeatTimes = m_clContent.GetItemText(m_nCurrentListIndex,1);

			CString path;
			path = CURVEDATA_PATH + content + ".ini";
			if(!m_pCurveData->ReadCurveData(path))
			{
				return false;
			}
			m_nRepeatTotal = atoi(repeatTimes);
			m_nFrameTotal = m_pCurveData->m_nFrameNum;
		}
		m_nTimeTotal = m_pDynamicCurve->SetPosSpeedContents(m_pCurveData->m_nCurveData[m_nFrameIndex],m_pCurveData->m_nTime[m_nFrameIndex]);
	}
	else if(m_nRunMode == MODE_MANUALLY)
	{
		CString sPos;
		CString sTime;
		m_ePos.GetWindowText(sPos);
		m_eTime.GetWindowText(sTime);
		int nPos = atoi(sPos);
		int nTime = atoi(sTime);

		if(nPos < 0)
			nPos = 0;
		else if(nPos > 9999)
			nPos = 9999;

		m_nRepeatTotal = 1;
		m_nFrameTotal = 1;
		m_nTimeTotal = m_pDynamicCurve->SetPosSpeedManually(nPos,nTime);
	}
	else
	{
		m_nRepeatTotal = 1;
		m_nFrameTotal = 1;
		m_nTimeTotal = m_pDynamicCurve->SetPosSpecialManually(m_nRunMode);
	}

	return true;
}

void CControlView::SetColor()
{
	if(m_nRunMode == MODE_CONTENTS)
	{
		m_pDynamicCurve->SetColorContents(m_nStartTime);
	}
	else if(m_nRunMode == MODE_MANUALLY || m_nRunMode == MODE_BACKTOZERO || m_nRunMode == MODE_MIDDLE)
	{
		m_pDynamicCurve->SetColorManually(m_nStartTime);
	}
	else if(m_nRunMode == MODE_LIMIT)
	{
		m_pDynamicCurve->SetColorManually(m_nStartTime);
	}
}

void CControlView::DoRun()
{
	if(m_nRunMode == MODE_CONTENTS)
	{
		m_pDynamicCurve->RunContents();
	}
	else if(m_nRunMode == MODE_LIMIT)
	{
		m_pDynamicCurve->ToLimitManually();
	}
	else if(m_nRunMode == MODE_LIMITTOZERO)
	{
		m_pDynamicCurve->FromLimitToZeroManually();
	}
	else if(m_nRunMode == MODE_BACKTOZERO)
	{
		m_pDynamicCurve->BackToZeroManually();
	}
	else if(m_nRunMode == MODE_MIDDLE)
	{
		m_pDynamicCurve->RunManually();
	}
	else if(m_nRunMode == MODE_MANUALLY)
	{
		m_pDynamicCurve->RunManually();
	}
}
