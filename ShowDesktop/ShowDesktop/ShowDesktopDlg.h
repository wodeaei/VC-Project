// ShowDesktopDlg.h : 头文件
//

#pragma once
#include <vector>
#include "afxcmn.h"
#include "ShellLink.h"
#include "WndShadow/WndShadow.h"
#include "NewsDlg.h"
const int nTopAreaHeight = 50;
const int nNewsAreaHeight = 280;
const int nWindowWidth = 320;
const int nWindowHeight = 600;
typedef struct DESKTOP_FILE_INFO{
	CString strTitle;
	CString strFilePath;
	CString strParam;
}DESKTOP_FILE_INFO;
typedef std::vector<DESKTOP_FILE_INFO>VECTOR_DESKTOP_FILE_INFO;

// CShowDesktopDlg 对话框
class CShowDesktopDlg : public CDialog
{
// 构造
public:
	CShowDesktopDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SHOWDESKTOP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	VECTOR_DESKTOP_FILE_INFO m_vctDesktopFile;
	CWndShadow m_WndShadow;
	CNewsDlg   m_NewsDlg;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	void SupportDropFiles();
	BOOL ShellLinkInfo(CString strFilePath,DESKTOP_FILE_INFO &fileInfo);
	BOOL AddFileInfo(CString strFilePath);
	void UpdateAllList();

	CListCtrl m_listAllFile;
	void InitList(CListCtrl &listCtrl);
	void SetImageToList(HWND hWnd);
	int GetIconIndex(LPCTSTR lpszPath, BOOL bIsDir = FALSE, BOOL bSelected = FALSE);
	afx_msg void OnLvnGetdispinfoListAllFile(NMHDR *pNMHDR, LRESULT *pResult);
    void Relayout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
