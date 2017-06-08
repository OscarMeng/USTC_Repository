#pragma once
#include "afxcmn.h"

#include "ColorList.h"
#include "afxwin.h"
// CControlView form view

#define  IDT_RUN		WM_USER+100

class CMainFrame;
class C3DView;
class CDynamicBallsView;
class CDynamicCurve;
class CCurveData;
class CControlView : public CFormView
{
	DECLARE_DYNCREATE(CControlView)

protected:
	CControlView();           // protected constructor used by dynamic creation
	virtual ~CControlView();

public:
	enum { IDD = IDD_CONTROL_VIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	static const int m_nTopMargin = 10;
	static const int m_nLeftMargin = 10;
	static const int m_nRightMargin = 10;
	static const int m_nBottomMargin = 10;
	static const int m_nButtonWidth = 40;
	static const int m_nButtonHeight = 30;
	static const int m_nRadioHeight = 20;
	static const int m_nStaticHeight = 20;
	static const int m_nStaticWidth1 = 80;
	static const int m_nStaticWidth2 = 300;
	static const int m_nBarWidth = 4;
	static const int m_nBarSpace = 2;
	static const int m_nCtrlSpace = 10;
	static const int m_nAllWidth = 560;
	static const int m_nTimerTime = 40;
public:
	CColorList	m_clContent;
	CButton		m_btAdd;
	CButton		m_btModify;
	CButton		m_btDelete;
	CButton		m_btBar1;
	CStatic		m_stRunContents;
	CButton		m_rbOrder;
	CButton		m_rbRand;
	CButton		m_btRun;
	CButton		m_btStop;
	CStatic		m_stManually;
	CButton		m_btLimit;
	CButton		m_btLimitToZero;
	CButton		m_btBackToZero;
	CButton		m_btMiddle;
	CStatic		m_stPos;
	CStatic		m_stSpeed;
	CEdit		m_ePos;
	CEdit		m_eTime;
	CButton		m_btDo;
	CButton		m_btBar2;
	CButton		m_btShutDown;
	CButton		m_chLoop;
	CButton		m_chAutoRotation;
private:
	bool		m_bListInited;
	bool		m_bCtrlsValid;

private:
	void LayoutCtrls();


public:
	virtual void OnInitialUpdate();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	void ReadContents();
	void WriteContents();

private:
	int		m_nItem;
	bool	m_bStopped;
	bool	m_bWaiting;
	int		m_nRunMode;
	int		m_nCurrentListIndex;
	int		m_nNextListIndex;
	int		m_nPlayIndex;
	int		m_nFrameIndex;
	int		m_nRepeatIndex;

	int		m_nFrameTotal;
	int		m_nRepeatTotal;
	DWORD	m_nStartTime;		//开始运行时的毫秒数
	int		m_nTimeTotal;				//运行总秒数
private:
	CMainFrame*			m_pMainFrame;
	C3DView*			m_p3DView;
	CDynamicBallsView*	m_pDynamicBallsView;
	CDynamicCurve*		m_pDynamicCurve;
	CCurveData*			m_pCurveData;
public:
	inline void		SetMainFrame(CMainFrame* pMainFrame)							{m_pMainFrame = pMainFrame;}
	inline void		Set3DView(C3DView* p3DView)										{m_p3DView = p3DView;}
	inline void		SetDynamicBallsView(CDynamicBallsView* pDynamicBallsView)		{m_pDynamicBallsView = pDynamicBallsView;}
	inline void		SetDynamicCurve(CDynamicCurve* pDynamicCurve)					{m_pDynamicCurve = pDynamicCurve;}
	inline void		SetCurveData(CCurveData* pCurveData)							{m_pCurveData = pCurveData;}

	inline bool		IsRunning()			{return !m_bStopped;}
public:
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnModify();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnNMClickListContent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListContent(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnRun();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnLimit();
	afx_msg void OnBnClickedBtnLimitToZero();
	afx_msg void OnBnClickedBtnBackToZero();
	afx_msg void OnBnClickedBtnMiddle();	
	afx_msg void OnBnClickedBtnDo();
	afx_msg void OnBnClickedCheckAuto();
	afx_msg void OnBnClickedBtnShutdown();
private:
	void PlayRun();
	void PlayBegin();
	void PlayOver();
	bool SetValue();
	void DoRun();
	void SetColor();
public:



};


