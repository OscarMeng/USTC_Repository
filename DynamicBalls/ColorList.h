#pragma once


// CColorList
struct stColor
{
	int nRow;
	COLORREF rgb;
};

class CColorList : public CListCtrl
{
	DECLARE_DYNAMIC(CColorList)

public:
	CColorList();
	virtual ~CColorList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
private:
	CPtrList m_ItemTextColor; //保存单元格字体颜色
public:
	void SetItemColor(int row,COLORREF color);
	void InsertItem(int row,LPCSTR lpszItem);
	bool FindItemColor(int row,COLORREF &color);
};


