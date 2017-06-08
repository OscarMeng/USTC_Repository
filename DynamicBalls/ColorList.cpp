// ColorList.cpp : implementation file
//

#include "stdafx.h"
#include "ColorList.h"

// CColorList

IMPLEMENT_DYNAMIC(CColorList, CListCtrl)

CColorList::CColorList()
{

}

CColorList::~CColorList()
{
	for (POSITION pos = m_ItemTextColor.GetTailPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ItemTextColor.GetPrev(pos);
		delete pColor;
	}
}


BEGIN_MESSAGE_MAP(CColorList, CListCtrl)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// CColorList message handlers

void CColorList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	TCHAR lpBuffer[256];

	LV_ITEM lvi;

	lvi.mask = LVIF_TEXT | LVIF_PARAM ;
	lvi.iItem = lpDrawItemStruct->itemID ; 
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer ;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));

	LV_COLUMN lvc, lvcprev ;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;

	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	pDC->SetBkMode(TRANSPARENT);  

	CString szText; 
	COLORREF color;
	if(!(lpDrawItemStruct->itemAction & ODA_DRAWENTIRE))
		return;

	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
	{ 
		CRect rcItem; 
		if (!GetSubItemRect(lpDrawItemStruct->itemID,nCol, LVIR_LABEL,rcItem)) 
			continue; 
		BOOL bIsSelected = (lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState & ODS_SELECTED);  
		CBrush brushBk;
		if(bIsSelected)
		{
			brushBk.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));  
			pDC->FillRect(rcItem,&brushBk);
		}
		if(FindItemColor(lpDrawItemStruct->itemID,color))
			pDC->SetTextColor(color);
		else
			pDC->SetTextColor(RGB(0,0,0));
		szText = GetItemText( lpDrawItemStruct->itemID,nCol); 
		CSize size = pDC->GetTextExtent(szText);
		rcItem.top = rcItem.top + (rcItem.Height()-size.cy)/2;
		rcItem.bottom = rcItem.bottom - (rcItem.Height()-size.cy)/2;
		if(nCol != 2)
		{
			rcItem.left = rcItem.left + (rcItem.Width()-size.cx)/2; 
			rcItem.right = rcItem.right - (rcItem.Width()-size.cx)/2; 
		}
		else
		{
			rcItem.left = rcItem.left + 4;
		}
		pDC->DrawText(szText, lstrlen(szText), &rcItem, DT_NOPREFIX|DT_SINGLELINE); 
	} 
}

void CColorList::SetItemColor(int row,COLORREF color)
{
	for (POSITION pos = m_ItemTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ItemTextColor.GetNext(pos);
		if (pColor->nRow == row)
		{
			pColor->rgb = color;
			break;
		}
	}
}

void CColorList::InsertItem(int row,LPCSTR lpszItem)
{
	stColor *pColor = new stColor;
	pColor->nRow = row;
	pColor->rgb = RGB(0,0,0);
	m_ItemTextColor.AddTail(pColor);

	CListCtrl::InsertItem(row,lpszItem);
}

bool CColorList::FindItemColor(int row,COLORREF &color)
{
	int flag = 0;
	for (POSITION pos = m_ItemTextColor.GetHeadPosition();pos!=NULL;)
	{
		stColor *pColor = (stColor*)m_ItemTextColor.GetNext(pos);
		if (pColor->nRow==row)
		{
			flag = 1;
			color = pColor->rgb;
			break;
		}
	}
	if (1==flag)
	{
		return true;
	}
	return false;
}
