// ShowDesktopDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ShowDesktop.h"
#include "ShowDesktopDlg.h"
#include "shlwapi.h"
#include "TransparentWindow.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CShowDesktopDlg 对话框




CShowDesktopDlg::CShowDesktopDlg(CWnd* pParent /*=NULL*/)
: CDialog(CShowDesktopDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShowDesktopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALL_FILE, m_listAllFile);
}

BEGIN_MESSAGE_MAP(CShowDesktopDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ALL_FILE, &CShowDesktopDlg::OnLvnGetdispinfoListAllFile)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CShowDesktopDlg 消息处理程序

BOOL CShowDesktopDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SupportDropFiles();
	InitList(m_listAllFile);
	m_WndShadow.Create(GetSafeHwnd());
	m_WndShadow.SetSize(8);
	m_WndShadow.SetSharpness(9);
	m_WndShadow.SetDarkness(45);
	CRect rect;
	GetWindowRect(rect);
	CTransparentWindow::InitWindowStyle(m_hWnd,nWindowWidth,nWindowHeight);
	if(m_NewsDlg.Create(IDD_DIALOG_NEWS,this))
	{
		m_NewsDlg.ShowWindow(SW_SHOW);
	}
	Relayout();
	m_WndShadow.ShowShadowWindow(TRUE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CShowDesktopDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CShowDesktopDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

typedef WINUSERAPI BOOL WINAPI CHANGEWINDOWMESSAGEFILTER(UINT message, DWORD dwFlag);
void CShowDesktopDlg::SupportDropFiles()
{
	//该函数功能，使win7下支持文件拖拽操作
	HINSTANCE hDllInst = LoadLibrary(_T("user32.dll"));
	if (hDllInst)
	{
		CHANGEWINDOWMESSAGEFILTER *pAddMessageFilterFunc = (CHANGEWINDOWMESSAGEFILTER *)GetProcAddress(hDllInst, "ChangeWindowMessageFilter");

		if (pAddMessageFilterFunc)
		{
			pAddMessageFilterFunc(0x0049,MSGFLT_ADD);
			pAddMessageFilterFunc(WM_DROPFILES, MSGFLT_ADD);
		}
		FreeLibrary(hDllInst);
	}
}

void CShowDesktopDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TCHAR *lpszFileName=new TCHAR[512];
	int nFileCount,i;
	BOOL bUpdateList = FALSE;
	nFileCount=::DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,512);
	for (i=0;i< nFileCount;i++)
	{
		UINT nChars=::DragQueryFile (hDropInfo,i,&lpszFileName[0],512);
		CString str( &lpszFileName[0],nChars);
		bUpdateList = AddFileInfo(str);
	}
	::DragFinish(hDropInfo);// 释放内存
	delete []lpszFileName;
	if (bUpdateList)
	{
		UpdateAllList();
	}
	//CDialog::OnDropFiles(hDropInfo);
}

BOOL CShowDesktopDlg::ShellLinkInfo(CString strFilePath,DESKTOP_FILE_INFO &fileInfo)
{
	ShellLink shellLink;
	if (shellLink.Load(strFilePath.GetBuffer()))
	{
		if(shellLink.TargetPathExists())
		{
			fileInfo.strFilePath = shellLink.GetTargetPath();
			fileInfo.strTitle = shellLink.GetDescription();
			fileInfo.strParam = shellLink.GetArguments();
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CShowDesktopDlg::AddFileInfo(CString strFilePath)
{
	DESKTOP_FILE_INFO newFileInfo;
	BOOL bRet = TRUE;
	if (strFilePath.Find(_T(".lnk")) != -1)
	{
		bRet = ShellLinkInfo(strFilePath,newFileInfo);
	}else
	{
		newFileInfo.strFilePath = strFilePath;

	}

	if (bRet)
	{
		if (newFileInfo.strTitle.IsEmpty() || newFileInfo.strTitle.GetLength() == 0)
		{
			newFileInfo.strTitle = PathFindFileName(strFilePath);
			newFileInfo.strTitle = newFileInfo.strTitle.Left(newFileInfo.strTitle.ReverseFind(_T('.')));
		}

		m_vctDesktopFile.push_back(newFileInfo);
	}
	return bRet;
}

void CShowDesktopDlg::UpdateAllList()
{
	m_listAllFile.SetItemCount(m_vctDesktopFile.size());
}

void CShowDesktopDlg::InitList(CListCtrl &listCtrl)
{
	LONG lStyle;
	lStyle = GetWindowLong(listCtrl.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= /*LVS_REPORT*/LVS_ICON; //设置style
	lStyle |= LVS_SHOWSELALWAYS;
	SetWindowLong(listCtrl.m_hWnd, GWL_STYLE, lStyle);//设置style
	DWORD ExStyle = listCtrl.GetExtendedStyle();
	//ExStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	//ExStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//ExStyle |= LVS_EX_CHECKBOXES;
	ExStyle |= LVS_EX_DOUBLEBUFFER;
	ExStyle |= LVS_OWNERDATA;
	ExStyle |= LVS_EX_INFOTIP;

	listCtrl.SetExtendedStyle(ExStyle);
	CRect rect;
	listCtrl.GetClientRect(&rect);
	//m_ctlFileList.SetImageList(&theApp.m_SystemIconList, LVSIL_SMALL);
	listCtrl.InsertColumn(0,_T("Title"), LVCFMT_RIGHT, 100);
	listCtrl.InsertColumn(1,_T("LocalFile"), LVCFMT_LEFT, rect.Width() - 100);
	SetImageToList(listCtrl.m_hWnd);
}

void CShowDesktopDlg::SetImageToList(HWND hWnd)
{
	HIMAGELIST himlSmall;
	HIMAGELIST himlLarge;
	SHFILEINFO sfi;
	TCHAR  cSysDir[MAX_PATH];
	CString  strBuf;

	memset(cSysDir, 0, MAX_PATH);

	GetWindowsDirectory(cSysDir, MAX_PATH);
	strBuf = cSysDir;
	_tprintf_s(cSysDir, _T("%s"), strBuf.Left(strBuf.Find(_T("\\"))+1));

	himlSmall = (HIMAGELIST)SHGetFileInfo ((LPCWSTR)cSysDir,  
		0,  
		&sfi, 
		sizeof(SHFILEINFO),  
		SHGFI_SYSICONINDEX | SHGFI_SMALLICON );

	himlLarge = (HIMAGELIST)SHGetFileInfo((LPCWSTR)cSysDir,  
		0,  
		&sfi,  
		sizeof(SHFILEINFO),  
		SHGFI_SYSICONINDEX | SHGFI_LARGEICON);

	if (himlSmall && himlLarge)
	{
		::SendMessage(hWnd, LVM_SETIMAGELIST,
			(WPARAM)LVSIL_SMALL, (LPARAM)himlSmall);
		::SendMessage(hWnd, LVM_SETIMAGELIST,
			(WPARAM)LVSIL_NORMAL, (LPARAM)himlLarge);
	}
}

int CShowDesktopDlg::GetIconIndex(LPCTSTR lpszPath, BOOL bIsDir, BOOL bSelected)
{
	SHFILEINFO sfi;
	memset(&sfi, 0, sizeof(sfi));

	if (bIsDir)
	{
		SHGetFileInfo(lpszPath,  
			FILE_ATTRIBUTE_DIRECTORY,  
			&sfi,  
			sizeof(sfi),  
			SHGFI_SMALLICON | SHGFI_SYSICONINDEX |
			SHGFI_USEFILEATTRIBUTES |(bSelected ? SHGFI_OPENICON : 0));  
		return  sfi.iIcon;
	}
	else
	{
		SHGetFileInfo (lpszPath,  
			FILE_ATTRIBUTE_NORMAL,  
			&sfi,  
			sizeof(sfi),  
			SHGFI_SMALLICON | SHGFI_SYSICONINDEX |  
			SHGFI_USEFILEATTRIBUTES | (bSelected ? SHGFI_OPENICON : 0));
		return   sfi.iIcon;
	}
	return  -1;
}

void CShowDesktopDlg::OnLvnGetdispinfoListAllFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	LV_ITEM* pItem= &(pDispInfo)->item;
	//Which item number?
	int itemid = pItem->iItem;

	if (m_vctDesktopFile.empty() || itemid >= m_vctDesktopFile.size())
	{
		return;
	}
	//Do the list need text information?
	if (pItem->mask & LVIF_TEXT)
	{
		CString text;
		switch (pItem->iSubItem)
		{
		case 0:     
			pItem->iImage = GetIconIndex(m_vctDesktopFile[itemid].strFilePath);
			text.Format(_T("%s"),m_vctDesktopFile[itemid].strTitle);	
			break;
		case 1: 
			text.Format(_T("%s"),m_vctDesktopFile[itemid].strFilePath);	
			break;
		default:
			break;
		}
		lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
		text.Empty();
	}

	*pResult = 0;
}

void CShowDesktopDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	Relayout();
}

void CShowDesktopDlg::Relayout()
{
	if (m_listAllFile.GetSafeHwnd())
	{
		CRect rect,rectStatic,rectMove,rectNews;
		GetWindowRect(rect);
		ScreenToClient(rect);
		rectMove = rect;
		rectNews = rect;
		rectMove.bottom = rect.top + nTopAreaHeight - 1;
		GetDlgItem(IDC_STATIC_QUICK_START_TIP)->GetWindowRect(rectStatic);
		rectMove.top = rectMove.CenterPoint().y - rectStatic.Height()/2;
		rectMove.bottom = rectMove.top + rectStatic.Height();
		GetDlgItem(IDC_STATIC_QUICK_START_TIP)->MoveWindow(rectMove);
		rect.DeflateRect(-1,nTopAreaHeight - 1,0,nNewsAreaHeight);
		m_listAllFile.MoveWindow(rect);
		rectNews.top = rectNews.bottom - nNewsAreaHeight;
		if(m_NewsDlg.GetSafeHwnd())
		{
			m_NewsDlg.MoveWindow(rectNews);
		}

	}
}

void CShowDesktopDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(rect);
	rect.bottom = rect.top + nTopAreaHeight;
	if (rect.PtInRect(point))
	{
		ReleaseCapture();   
		SendMessage(WM_NCLBUTTONDOWN,   HTCAPTION,   0);
	}

	CDialog::OnMouseMove(nFlags, point);
}
