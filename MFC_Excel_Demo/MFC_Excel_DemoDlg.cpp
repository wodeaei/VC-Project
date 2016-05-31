// MFC_Excel_DemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFC_Excel_Demo.h"
#include "MFC_Excel_DemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMFC_Excel_DemoDlg �Ի���


CMFC_Excel_DemoDlg::CMFC_Excel_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFC_Excel_DemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pBaseWorksheet = NULL;
}

void CMFC_Excel_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SHEETNAME, m_wndComboBoxSheetNames);
	DDX_Control(pDX, IDC_LIST_DATA, m_wndListData);
}

BEGIN_MESSAGE_MAP(CMFC_Excel_DemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CMFC_Excel_DemoDlg::OnBnClickedButtonOpen)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DATA, &CMFC_Excel_DemoDlg::OnLvnGetdispinfoListData)
	ON_CBN_SELCHANGE(IDC_COMBO_SHEETNAME, &CMFC_Excel_DemoDlg::OnCbnSelchangeComboSheetname)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMFC_Excel_DemoDlg ��Ϣ�������

BOOL CMFC_Excel_DemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
    InitListStyle(m_wndListData,LVS_REPORT,/*LVS_EX_CHECKBOXES|*/LVS_EX_FULLROWSELECT|LVS_OWNERDATA|LVS_EX_GRIDLINES,FALSE);
	ReLayout();
	UpdateTips(_T("��ѡ��һ��Ҫ�鿴��Excel�ļ���"));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFC_Excel_DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFC_Excel_DemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFC_Excel_DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFC_Excel_DemoDlg::InitListStyle(CListCtrl &wndList,DWORD dwStyle /*= LVS_REPORT*/,DWORD dwExStyle /*= LVS_EX_CHECKBOXES*/,BOOL bShowImage/* = TRUE*/)
{
	LONG lStyle;
	lStyle = GetWindowLong(wndList.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= dwStyle; //����style
	SetWindowLong(wndList.m_hWnd, GWL_STYLE, lStyle);//����style
	DWORD dwExtendStyle = wndList.GetExtendedStyle();
	
	dwExtendStyle |= LVS_EX_INFOTIP;
	dwExtendStyle |= LVS_OWNERDATA;
	dwExtendStyle |= LVS_EX_DOUBLEBUFFER/*|LVS_EX_FULLROWSELECT|LVS_EX_INFOTIP*/;
	dwExtendStyle |= dwExStyle;

	wndList.SetExtendedStyle(dwExtendStyle); //������չ���
	if (bShowImage)
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

		himlSmall = (HIMAGELIST)SHGetFileInfo ((LPCTSTR)cSysDir,  
			0,  
			&sfi, 
			sizeof(SHFILEINFO),  
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON );

		himlLarge = (HIMAGELIST)SHGetFileInfo((LPCTSTR)cSysDir,  
			0,  
			&sfi,  
			sizeof(SHFILEINFO),  
			SHGFI_SYSICONINDEX | SHGFI_LARGEICON);

		if (himlSmall && himlLarge)
		{
			::SendMessage(wndList.m_hWnd, LVM_SETIMAGELIST,
				(WPARAM)LVSIL_SMALL, (LPARAM)himlSmall);
			::SendMessage(wndList.m_hWnd, LVM_SETIMAGELIST,
				(WPARAM)LVSIL_NORMAL, (LPARAM)himlLarge);
		}
	}

}

void CMFC_Excel_DemoDlg::OnBnClickedButtonOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


    CFileDialog fileDlg(TRUE, NULL ,_T("") , OFN_HIDEREADONLY , _T("����Execl�ļ�|*.xls|�����ļ�|*.*") , NULL);

	if (fileDlg.DoModal() == IDOK)//��������ȷ���͵õ��ļ�·��
	{
		if(!m_baseExcel.Load(fileDlg.GetPathName()))//�õ��ļ�·��
		{
           AfxMessageBox(_T("�����ļ�ʧ�ܣ�������ѡ���ļ�������"));
		   return ;
		}
	}else
	{
		return ;
	}
	m_wndComboBoxSheetNames.ResetContent();
	size_t nSize = m_baseExcel.GetTotalWorkSheets();
	for (int i = 0;i<nSize;i++)
	{
		if (m_baseExcel.GetAnsiSheetName(i) == 0)
		{
			USES_CONVERSION;
			m_wndComboBoxSheetNames.AddString(W2A(m_baseExcel.GetUnicodeSheetName(i)));
		}else
		{ 
            m_wndComboBoxSheetNames.AddString(m_baseExcel.GetAnsiSheetName(i));
		}
		
	}
	m_wndComboBoxSheetNames.SetCurSel(0);
	ChangeSheet(m_wndComboBoxSheetNames.GetCurSel());
}

void CMFC_Excel_DemoDlg::ChangeSheet(int nIndex)
{
	CString strName,str;
	m_wndComboBoxSheetNames.GetLBText(nIndex,strName);
   m_pBaseWorksheet = m_baseExcel.GetWorksheet(nIndex);
   if (m_pBaseWorksheet == NULL)
   {
	   ResetList(0);
	   str.Format(_T("ѡ���˱�[%s]���ñ������ݡ�"),strName);
	   UpdateTips(str);
	   return ;
   } 
   
   ResetList(m_pBaseWorksheet->GetTotalCols());
   m_wndListData.SetItemCount(m_pBaseWorksheet->GetTotalRows());
   
   str.Format(_T("ѡ���˱�[%s]���ñ� %d * %d �����ݡ�"),strName,
	   m_pBaseWorksheet->GetTotalRows(),
	   m_pBaseWorksheet->GetTotalCols());
   UpdateTips(str);

}

void CMFC_Excel_DemoDlg::ResetList(int nTotalCols)
{
	CString str;
	int nCount = m_wndListData.GetHeaderCtrl()->GetItemCount();
	for(int nIndex=0; nIndex<nCount; nIndex++)   
	{
		m_wndListData.DeleteColumn(0);//ɾ��������
	}
	CRect rectList;
	int nAverageWidth = 100;
    GetDlgItem(IDC_LIST_DATA)->GetWindowRect(rectList);
	if (nTotalCols != 0)
	{
		nAverageWidth = (rectList.Width() - 20) / nTotalCols;
	}
   for (int i = 0; i < nTotalCols; i++)
   {
	   str.Format(_T("�� %d ��"),i);
	   m_wndListData.InsertColumn(i,str,LVCFMT_LEFT,nAverageWidth);
   }
}

CString CMFC_Excel_DemoDlg::GetData(int nRow,int nCol)
{
   CString strData;
   if (m_pBaseWorksheet != NULL)
   {
	   BasicExcelCell* cell = m_pBaseWorksheet->Cell(nRow,nCol);
	   switch (cell->Type())
	   {
	   case BasicExcelCell::UNDEFINED:
		  strData = _T("");
		   break;
	   case BasicExcelCell::INT:
		   strData.Format(_T("%d"), cell->GetInteger());
		   break;
	   case BasicExcelCell::DOUBLE:
		   strData.Format(_T("%lf"), cell->GetDouble());
		   break;
	   case BasicExcelCell::STRING:
		   strData.Format(_T("%s"), cell->GetString());
		   break;
	   case BasicExcelCell::WSTRING:
           USES_CONVERSION;
		   strData.Format(_T("%s"), W2A(cell->GetWString()));
		   break;
	   }
   }
   return strData;
}
void CMFC_Excel_DemoDlg::OnLvnGetdispinfoListData(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString strData;
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

// 	if (pDispInfo->item.mask & LVIF_COLUMNS)
// 	{
// 	}
// 	if (pDispInfo->item.mask & LVIF_DI_SETITEM)
// 	{
// 	}
// 	if (pDispInfo->item.mask & LVIF_GROUPID)
// 	{
// 	}
// 	if (pDispInfo->item.mask & LVIF_IMAGE)
// 	{
// 	}
// 	if (pDispInfo->item.mask & LVIF_INDENT)
// 	{
// 	}
// 	if (pDispInfo->item.mask & LVIF_NORECOMPUTE)
// 	{
// 	}
// 	if (pDispInfo->item.mask & LVIF_PARAM)
// 	{
// 	}
// 	if (pDispInfo->item.mask & LVIF_STATE)
// 	{
// 	}
	if (pDispInfo->item.mask & LVIF_TEXT)
	{
		if (pDispInfo->item.iItem != -1
			&& pDispInfo->item.iSubItem != -1)
		{
			strData = GetData(pDispInfo->item.iItem,
				pDispInfo->item.iSubItem);
			lstrcpyn(pDispInfo->item.pszText, strData, pDispInfo->item.cchTextMax);
			strData.Empty();
		}
	}
	*pResult = 0;
}

void CMFC_Excel_DemoDlg::OnCbnSelchangeComboSheetname()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ChangeSheet(m_wndComboBoxSheetNames.GetCurSel());
}

void CMFC_Excel_DemoDlg::ReLayout()
{
	if (!GetSafeHwnd())
	{
		return ;
	}
	CRect rectClient,rectMove,rectControl;
	GetClientRect(rectClient);
	rectClient.DeflateRect(5,5,5,5);

	GetDlgItem(IDC_BUTTON_OPEN)->GetWindowRect(rectControl);
	rectMove = rectClient;
	rectMove.bottom = rectMove.top + rectControl.Height();
	rectMove.right = rectMove.left + rectControl.Width();
	GetDlgItem(IDC_BUTTON_OPEN)->MoveWindow(rectMove);
    rectClient.top = rectMove.bottom + 5;

	GetDlgItem(IDC_COMBO_SHEETNAME)->GetWindowRect(rectControl);
	rectMove.top = rectMove.CenterPoint().y - rectControl.Height()/2;
	rectMove.bottom = rectMove.top + rectControl.Height();
	rectMove.left = rectMove.right + 5;
	rectMove.right = rectMove.left + rectControl.Width();
	GetDlgItem(IDC_COMBO_SHEETNAME)->MoveWindow(rectMove);

	GetDlgItem(IDC_STATIC_TIPS)->GetWindowRect(rectControl);
	rectMove = rectClient;
	rectMove.top = rectMove.bottom - rectControl.Height();
	GetDlgItem(IDC_STATIC_TIPS)->MoveWindow(rectMove);
	rectClient.bottom = rectMove.top - 5;

	GetDlgItem(IDC_LIST_DATA)->MoveWindow(rectClient);
}

void CMFC_Excel_DemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	ReLayout();
}

void CMFC_Excel_DemoDlg::UpdateTips(CString strText)
{
	SetDlgItemText(IDC_STATIC_TIPS,strText);
}
