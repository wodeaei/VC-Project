// MFC_Excel_DemoDlg.h : ͷ�ļ�
//

#pragma once
#include "BasicExcel.hpp"
#include "afxwin.h"
#include "afxcmn.h"
using namespace YExcel;
// CMFC_Excel_DemoDlg �Ի���
class CMFC_Excel_DemoDlg : public CDialog
{
// ����
public:
	CMFC_Excel_DemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFC_EXCEL_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
private:
    BasicExcel m_baseExcel;//�������excel�ļ�
    BasicExcelWorksheet *m_pBaseWorksheet;//������


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
