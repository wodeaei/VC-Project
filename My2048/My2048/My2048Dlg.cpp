// My2048Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "My2048.h"
#include "My2048Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMy2048Dlg 对话框




CMy2048Dlg::CMy2048Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy2048Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nTopGrade = 0;
	m_nGrade = 0;
}

void CMy2048Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMy2048Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_RESTART, &CMy2048Dlg::OnBnClickedButtonRestart)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMy2048Dlg 消息处理程序

BOOL CMy2048Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	m_nTopGrade = GetPrivateProfileInt(_T("Grade"),_T("TopGrad"),0,_T(".\\config.ini"));
    GetFont()->GetLogFont(&m_lf);
	m_lf.lfHeight = 36;//m_TransWindow.GetIdealScaleX(24);
	m_lf.lfWeight = FW_BOLD;
	m_BoldFont.CreateFontIndirect(&m_lf); //创建字体

	InitMap();

	// TODO: 在此添加额外的初始化代码


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMy2048Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy2048Dlg::OnPaint()
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

BOOL CMy2048Dlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ReDraw(pDC);
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy2048Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMy2048Dlg::ReDraw(CDC *pDC)
{
	BOOL bAutoReleaseDC = FALSE;
	if (NULL == pDC)
	{
		pDC = GetDC();
		bAutoReleaseDC = TRUE;
	} 
	CPoint ptPoint(20,20);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	CRect rect,rectAllCell;

	GetClientRect(&rect);
	rectAllCell = rect;
	int nWidth = rect.Width(),nHeight = rect.Height();
	bmp.CreateCompatibleBitmap(pDC,nWidth,nHeight);
	memDC.SelectObject(bmp);
    memDC.FillSolidRect(rect,/*GetSysColor(COLOR_3DFACE)*/RGB(250,248,239));

	rectAllCell.DeflateRect(nOtherMargin,nTopMargin,nOtherMargin,nOtherMargin);
	memDC.FillSolidRect(rectAllCell,colCellbg);
	int bkMode =memDC.GetBkMode();
	memDC.SetBkMode(TRANSPARENT);
	MAP_ALL_CELL::iterator iterItem;
	COLORREF colCell,TextColor;
	tagCellInfo cellInfo;
	CString str;
	colCell = RGB(255,255,255);
	TextColor = RGB(0,0,0);
	CPen hotborderpen;   
	hotborderpen.CreatePen(PS_SOLID, 1, colCellbg);  
	CPen *pOldPen = (CPen *)memDC.SelectObject(&hotborderpen); 

	CFont *pOldFont = (CFont *)memDC.SelectObject(&m_BoldFont);
	for (iterItem = m_allCellInfo.begin();
		iterItem != m_allCellInfo.end();
		++iterItem)
	{
		cellInfo = iterItem->second;
		
 		if (cellInfo.nValue == 0)
 		{
 			colCell = RGB(204,192,179);
 		}else if (cellInfo.nValue == 2)
 		{
 			colCell = RGB(238,238,218);//RGB(217,213,201);
 		}else if (cellInfo.nValue == 4)
 		{
 			colCell = RGB(237,224,200);
 		}else if (cellInfo.nValue == 8)
 		{
 			colCell = RGB(242,177,121);
 		}else if (cellInfo.nValue == 16)
 		{
 			colCell = RGB(245,149,99);
 		}else if (cellInfo.nValue == 32)
 		{
 			colCell = RGB(246,124,95);
 		}else if (cellInfo.nValue == 64)
 		{
 			colCell = RGB(246,94,59);
		}else if (cellInfo.nValue == 128)
		{
			colCell = RGB(246,207,114);
		}else if (cellInfo.nValue == 256)
		{
			colCell = RGB(237,204,97);
		}
		CBrush brushBG(colCell);
		CBrush *pOldBrush = (CBrush *)memDC.SelectObject(&brushBG);
		if (cellInfo.bRedraw)
		{
			CRect rectFill = cellInfo.rect;
			rectFill.InflateRect(2,2,2,2);
           memDC.RoundRect(rectFill,ptPoint);
		   iterItem->second.bRedraw = FALSE;
		}else
		{
			memDC.RoundRect(cellInfo.rect,ptPoint);
		}
		memDC.SelectObject(pOldBrush);
		if (cellInfo.nValue != 0)
		{
			if (cellInfo.nValue <= 4)
			{
				TextColor = RGB(119,110,101);
			}else
			{
				TextColor = RGB(255,255,255);
			}
			str.Format(_T("%d"),cellInfo.nValue);
			memDC.SetTextColor(TextColor);
			memDC.DrawText(str,cellInfo.rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);
		}
		}
	memDC.SetBkMode(bkMode);
	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldPen);
	static BOOL bFirst = TRUE;
	if (bFirst)
	{
		bFirst = FALSE;
		ModifyStyle(0,WS_CLIPCHILDREN);
	}

	::BitBlt(pDC->GetSafeHdc(),0,0,
		nWidth,
		nHeight,
		memDC,0,0,SRCCOPY);
	if (bAutoReleaseDC)
	{
		::ReleaseDC(m_hWnd,pDC->GetSafeHdc());
	}
}

void CMy2048Dlg::RelayoutCell()
{
	CString strKey;
	tagCellInfo newCellInfo;
	MAP_ALL_CELL::iterator iterFind;
	int nWidth = 0,nHeight = 0;
	CRect rectClient,cellrect;
	if (m_allCellInfo.empty())
	{
		return ;
	}
	GetClientRect(rectClient);
	rectClient.DeflateRect(nOtherMargin,nTopMargin,nOtherMargin,nOtherMargin);
	nWidth = (rectClient.Width() - (m_nColumns + 1) *nCellMargin)/m_nColumns;
	nHeight = (rectClient.Height() - (m_nRows + 1) * nCellMargin)/m_nRows;
	for (int i = 0;i < m_nRows;i++)
	{
		for (int j = 0;j < m_nColumns;j++)
		{
			cellrect.left = rectClient.left + (j+1)*nCellMargin + j*nWidth;
			cellrect.top = rectClient.top + (i + 1)*nCellMargin + i*nHeight;
			cellrect.right = cellrect.left + nWidth;
			cellrect.bottom = cellrect.top + nHeight;
			strKey.Format(_T("%d*%d"),i,j);
			iterFind = m_allCellInfo.find(strKey);
			if (iterFind != m_allCellInfo.end())
			{
				iterFind->second.rect = cellrect;
			}
		}
	}
	Invalidate();
}

void CMy2048Dlg::InitMap(int nRow /* = 4 */,int nColumns /* = 4 */)
{
	CString strKey;
	tagCellInfo newCellInfo;
	int nWidth = 0,nHeight = 0;
	m_allCellInfo.clear();
	m_nColumns = nColumns;
	m_nRows = nRow;
	m_nGrade = 0;
	for (int i = 0;i < m_nRows;i++)
	{
		for (int j = 0;j < m_nColumns;j++)
		{
			newCellInfo.nValue = 0;
			newCellInfo.bRedraw = FALSE;
            strKey.Format(_T("%d*%d"),i,j);
			m_allCellInfo[strKey] = newCellInfo;
		}
	}
	m_nGapCell = m_allCellInfo.size();
	RelayoutCell();
	RandValue(TRUE);
	UpdateGrade();
}

void CMy2048Dlg::RandValue(BOOL bInit /* = FALSE */)
{
	int nRow = 0,nColumn = 0;
	int nLoop = 1;
	CString str;
	MAP_ALL_CELL::iterator iterFind;
	
	if (bInit)
	{
		nLoop = 2;
	}
	for (int i = 0;i< nLoop && m_nGapCell > 0;)
	{
		nRow = rand()%m_nRows;
	    nColumn = rand()%m_nColumns;
		str.Format(_T("%d*%d"),nRow,nColumn);
		iterFind = m_allCellInfo.find(str);
		if (iterFind != m_allCellInfo.end())
		{
			if (iterFind->second.nValue == 0)
			{
				iterFind->second.nValue = 2;
				i++;
				m_nGapCell --;
			}
		}
	}
}

int CMy2048Dlg::GetCellValue(int nRow,int nColumn)
{
   CString str;
   MAP_ALL_CELL::iterator iterFind;
   if (m_allCellInfo.empty())
   {
	   return  -1;
   }

   str.Format(_T("%d*%d"),nRow,nColumn);
   iterFind = m_allCellInfo.find(str);
   if (iterFind == m_allCellInfo.end())
   {
	   return -1;
   }
   return iterFind->second.nValue;
}

BOOL CMy2048Dlg::SetCellValue(int nRow,int nColumn,TransData transData)
{
	CString str;
	MAP_ALL_CELL::iterator iterFind;
	if (m_allCellInfo.empty())
	{
		return  FALSE;
	}

	str.Format(_T("%d*%d"),nRow,nColumn);
	iterFind = m_allCellInfo.find(str);
	if (iterFind != m_allCellInfo.end())
	{
		if (iterFind->second.nValue != transData.nValue)
		{
			iterFind->second.nValue = transData.nValue;
			iterFind->second.bRedraw = transData.bRedraw;
			return TRUE;
		}	
	}
	return FALSE;
}

void CMy2048Dlg::CalculateValue(VECTOR_INTEGER &ArrValue,BOOL &bSkip,TransData transData)
{
	if (transData.nValue != 0)
	{
		if (ArrValue.empty())
		{
			ArrValue.push_back(transData);
		}else
		{
			if (!bSkip)
			{
				if (ArrValue[ArrValue.size() - 1].nValue == transData.nValue)
				{
					m_nGrade += 2*transData.nValue;
					ArrValue[ArrValue.size() - 1].nValue = 2*transData.nValue;
					ArrValue[ArrValue.size() - 1].bRedraw = TRUE;
					m_nGapCell ++;
					bSkip = TRUE;
				}else
				{
					bSkip = FALSE;
					ArrValue.push_back(transData);
				}
			}else
			{
				bSkip = FALSE;
				ArrValue.push_back(transData);
			}   
		}
	} 
}

BOOL CMy2048Dlg::IsGameOver()
{
	BOOL bOver = TRUE;
	VECTOR_INTEGER ArrValue;
	int nTempValue = -1;
	int nValue = 0;
	BOOL bSkip = FALSE;
     if (m_nGapCell == 0)
     {
		 for (int i = 0;i < m_nRows;i++)
		 {
			 nTempValue = -1;
			 for (int j = 0;j< m_nColumns; j++)
			 {
				 nValue = GetCellValue(i,j);
				 if(nTempValue == -1)
				 {
					 nTempValue = nValue;
				 }else
				 {
					 if (nTempValue == nValue)
					 {
						 return FALSE;
					 }
					 nTempValue = nValue;
				 }
			 } 
		 }

		 for (int i = 0;i < m_nColumns ;i++)
		 {
			 nTempValue = -1;
			 for (int j = 0;j< m_nRows; j++)
			 {
				 nValue = GetCellValue(j,i);
				 if(nTempValue == -1)
				 {
					 nTempValue = nValue;
				 }else
				 {
					 if (nTempValue == nValue)
					 {
						 return FALSE;
					 }
					 nTempValue = nValue;
				 }
			 } 
		 }
     }else
	 {
		 bOver = FALSE;
	 }
	return bOver;
}

void CMy2048Dlg::MoveCell(int nMoveType/* = TO_LEFT*/)
{
	VECTOR_INTEGER ArrValue;
	BOOL bRedraw = FALSE;
	TransData transData;
	transData.bRedraw = FALSE;
	BOOL bSkip = FALSE;
	BOOL bSetSucced = FALSE;
	BOOL bDoubleRedraw = FALSE;
   switch (nMoveType)
   {
   case TO_LEFT:
	   for (int i = 0;i < m_nRows;i++)
	   {
		   bSkip = FALSE;
		   bSetSucced = FALSE;
		   transData.bRedraw = FALSE;
		   ArrValue.clear();
		   for (int j = 0;j< m_nColumns; j++)
		   {
			   transData.nValue = GetCellValue(i,j);
			   CalculateValue(ArrValue,bSkip,transData);
		   }
           transData.nValue = 0;
		   if (!ArrValue.empty())
		   {
			   for (int j = 0;j< m_nColumns; j++)
			   {
				   if (j < ArrValue.size())
				   {
					   if(ArrValue[j].bRedraw)
					   {
						   bDoubleRedraw = TRUE;
					   }
					    bSetSucced = SetCellValue(i,j,ArrValue[j]);
				   }else
				   {
                        bSetSucced = SetCellValue(i,j,transData);
				   }
				   if (bSetSucced)
				   {
					    bRedraw = TRUE;
				   }
				   
			   }
		   }
		   
	   }
	   break;
   case TO_RIGHT:
	   for (int i = 0;i < m_nRows;i++)
	   {
		   bSkip = FALSE;
		   bSetSucced = FALSE;
		   ArrValue.clear();
		   for (int j = m_nColumns - 1;j >= 0; j--)
		   {
			   transData.nValue = GetCellValue(i,j);
			   CalculateValue(ArrValue,bSkip,transData);
		   }
		   transData.nValue = 0;
		   if (!ArrValue.empty())
		   {
			   for (int j = m_nColumns - 1;j >= 0; j--)
			   {
				   if ((m_nColumns - j - 1)< ArrValue.size())
				   {
					   if(ArrValue[m_nColumns - 1 - j].bRedraw)
					   {
						   bDoubleRedraw = TRUE;
					   }
					   bSetSucced = SetCellValue(i,j,ArrValue[m_nColumns - 1 - j]);
				   }else
				   {
					   bSetSucced = SetCellValue(i,j,transData);
				   }
				   if (bSetSucced)
				   {
					   bRedraw = TRUE;
				   }

			   }
		   }

	   }
	   break;
   case TO_TOP:
	   for (int i = 0;i < m_nColumns;i++)
	   {
		   bSkip = FALSE;
		   bSetSucced = FALSE;
		   ArrValue.clear();
		   for (int j = 0;j< m_nRows; j++)
		   {
			   transData.nValue = GetCellValue(j,i);
			   CalculateValue(ArrValue,bSkip,transData);
		   }
		  transData.nValue = 0;
		   if (!ArrValue.empty())
		   {
			   for (int j = 0;j< m_nRows; j++)
			   {
				   if (j < ArrValue.size())
				   {
					   if(ArrValue[j].bRedraw)
					   {
						   bDoubleRedraw = TRUE;
					   }
					   bSetSucced = SetCellValue(j,i,ArrValue[j]);
				   }else
				   {
					   bSetSucced = SetCellValue(j,i,transData);
				   }
				   if (bSetSucced)
				   {
					   bRedraw = TRUE;
				   }
			   }
		   }

	   }
	   break;
   case TO_BOTTOM:
	   for (int i = 0;i < m_nColumns;i++)
	   {
		   bSkip = FALSE;
		   bSetSucced = FALSE;
		   ArrValue.clear();
		   for (int j = m_nRows - 1;j >= 0; j--)
		   {
			   transData.nValue = GetCellValue(j,i);
			   CalculateValue(ArrValue,bSkip,transData);
		   }
		   transData.nValue = 0;
		   if (!ArrValue.empty())
		   {
			   for (int j = m_nRows - 1;j >= 0; j--)
			   {
				   if ((m_nRows - j - 1)< ArrValue.size())
				   {
					   if(ArrValue[m_nRows - 1 - j].bRedraw)
					   {
						   bDoubleRedraw = TRUE;
					   }
					   bSetSucced = SetCellValue(j,i,ArrValue[m_nRows - 1 - j]);
				   }else
				   {
					   bSetSucced = SetCellValue(j,i,transData);
				   }
				   if (bSetSucced)
				   {
					   bRedraw = TRUE;
				   }

			   }
		   }

	   }
	   break;
   default:
	   break;
   }
   if (bRedraw)
   {
	   RandValue(FALSE);
	   UpdateGrade();
	   Invalidate();
	   if (bDoubleRedraw)
	   {
		   SetTimer(TIMER_REDRAW,80,NULL);
	   }
	   if(IsGameOver())
	   {
		   MessageBox(_T("Game Over !"),_T("Tips"),MB_OK);
	   }
   }
}

void CMy2048Dlg::OnBnClickedButtonRestart()
{
	// TODO: 在此添加控件通知处理程序代码
	InitMap();
	Invalidate();
}

BOOL CMy2048Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
    switch(pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_DOWN:
			MoveCell(TO_BOTTOM);
			break;
		case VK_LEFT:
			MoveCell(TO_LEFT);
			break;
		case VK_RIGHT:
			MoveCell(TO_RIGHT);
			break;
		case VK_UP:
			MoveCell(TO_TOP);
			break;
		default:
			break;
		}
			break;
	default:
		break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMy2048Dlg::UpdateGrade()
{
	CString str;
	if (m_nGrade > m_nTopGrade)
	{
		m_nTopGrade = m_nGrade;
	}
    str.Format(_T("%d"),m_nGrade);
	GetDlgItem(IDC_STATIC_GRADE)->SetWindowText(str);
	 str.Format(_T("%d"),m_nTopGrade);
	GetDlgItem(IDC_STATIC_TOPGRADE)->SetWindowText(str);
}
void CMy2048Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    if(nIDEvent == TIMER_REDRAW)
	{
		KillTimer(TIMER_REDRAW);
		Invalidate();
	}
	CDialog::OnTimer(nIDEvent);
}

void CMy2048Dlg::OnDestroy()
{
	CDialog::OnDestroy();
	CString str;
	str.Format(_T("%d"),m_nTopGrade);
    WritePrivateProfileString(_T("Grade"),_T("TopGrad"),str,_T(".\\config.ini"));
	// TODO: 在此处添加消息处理程序代码
}

void CMy2048Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
    RelayoutCell();
	// TODO: 在此处添加消息处理程序代码
}
