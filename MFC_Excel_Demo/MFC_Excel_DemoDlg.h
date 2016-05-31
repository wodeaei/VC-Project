// MFC_Excel_DemoDlg.h : 头文件
//

#pragma once
#include "BasicExcel.hpp"
#include "afxwin.h"
#include "afxcmn.h"
using namespace YExcel;
// CMFC_Excel_DemoDlg 对话框
class CMFC_Excel_DemoDlg : public CDialog
{
// 构造
public:
	CMFC_Excel_DemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFC_EXCEL_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
private:
    BasicExcel m_baseExcel;//负责操作excel文件
    BasicExcelWorksheet *m_pBaseWorksheet;//工作簿


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpen();
	void InitListStyle(CListCtrl &wndList,DWORD dwStyle = LVS_REPORT,DWORD dwExStyle = LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_OWNERDATA/*|LVS_EX_GRIDLINES*/,BOOL bShowImage = TRUE);
	CComboBox m_wndComboBoxSheetNames;
	void ChangeSheet(int nIndex);
	void ResetList(int nTotalCols);
	CString GetData(int nRow,int nCol);
	
	CListCtrl m_wndListData;
	afx_msg void OnLvnGetdispinfoListData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboSheetname();
	void ReLayout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void UpdateTips(CString strText);
};
