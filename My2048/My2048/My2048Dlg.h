// My2048Dlg.h : 头文件
//

#pragma once
#include <map>
#include <vector>

#define TIMER_REDRAW 0
const int nTopMargin = 50;
const int nOtherMargin = 10;
const int nCellMargin = 5;
const COLORREF colCellbg = RGB(187,173,160);
typedef struct TransData{
	int nValue;
	BOOL bRedraw;
}TransData;
typedef std::vector<TransData>VECTOR_INTEGER;
typedef struct CellInfo{
	int nValue;
	CRect rect;
	COLORREF TextColor;
	COLORREF BackColor;
	BOOL bRedraw;
}tagCellInfo,*PtagCellInfo;
typedef std::map<CString,tagCellInfo> MAP_ALL_CELL;
typedef enum CellMoveType{
	TO_LEFT = 0,
	TO_RIGHT = 1,
	TO_TOP = 2,
	TO_BOTTOM = 3
}CellMoveType;

// CMy2048Dlg 对话框
class CMy2048Dlg : public CDialog
{
// 构造
public:
	CMy2048Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY2048_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

   MAP_ALL_CELL m_allCellInfo;
   int m_nRows,m_nColumns;
   LOGFONT m_lf;
   CFont m_BoldFont;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void ReDraw(CDC *pDC);

	void InitMap(int nRow = 4,int nColumns = 4);
	void RelayoutCell();
	void RandValue(BOOL bInit = FALSE);
	int GetCellValue(int nRow,int nColumn);
	BOOL SetCellValue(int nRow,int nColumn,TransData transData);
	void CalculateValue(VECTOR_INTEGER &ArrValue,BOOL &bSkip,TransData transData);
	void MoveCell(int nMoveType = TO_LEFT);
    BOOL IsGameOver();
	void UpdateGrade();
	COLORREF GetCellBackColor(int nValue);
	COLORREF GetCellTextColor(int nValue);
	int  m_nGapCell;
	UINT m_nGrade;
	UINT m_nTopGrade;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonRestart();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
