// NewsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ShowDesktop.h"
#include "NewsDlg.h"
#include "MyControlSite.h"

// CNewsDlg 对话框

IMPLEMENT_DYNCREATE(CNewsDlg, CDHtmlDialog)

CNewsDlg::CNewsDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CNewsDlg::IDD, CNewsDlg::IDH, pParent)
{

}

CNewsDlg::~CNewsDlg()
{
}

void CNewsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CNewsDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	Navigate(/*_T("res\\html_new.htm")*/_T("http://www.leavemealone.cn/test.html"),navNoHistory|navNoReadFromCache|navNoWriteToCache);
	SetExternalDispatch(GetIDispatch(TRUE));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CNewsDlg, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CNewsDlg)
// 	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
// 	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CNewsDlg 消息处理程序

// HRESULT CNewsDlg::OnButtonOK(IHTMLElement* /*pElement*/)
// {
// 	OnOK();
// 	return S_OK;
// }
// 
// HRESULT CNewsDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
// {
// 	OnCancel();
// 	return S_OK;
// }

STDMETHODIMP CNewsDlg::GetHostInfo(DOCHOSTUIINFO* pInfo)
{
	//pInfo->dwFlags = DOCHOSTUIFLAG_SCROLL_NO ;//DOCHOSTUIFLAG_THEME;
	//pInfo->dwFlags |= ( 0x08 /*DOCHOSTUIFLAG_SCROLL_NO*/ | 0x04 /*DOCHOSTUIFLAG_NO3DBORDER*/ );
	pInfo->dwFlags |= DOCHOSTUIFLAG_SCROLL_NO;
	pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER;
	//pInfo->dwFlags |= DOCHOSTUIFLAG_THEME;//设置该值，有可能导致在XP系统中出现按钮太长变形的问题
	pInfo->dwFlags |= DOCHOSTUIFLAG_DIALOG;
	//pInfo->dwFlags |= DOCHOSTUIFLAG_OPENNEWWIN;
	pInfo->dwFlags |= DOCHOSTUIFLAG_ENABLE_REDIRECT_NOTIFICATION;
	//pInfo->dwFlags |= DOCHOSTUIFLAG_ACTIVATE_CLIENTHIT_ONLY;


	return S_OK;
}

STDMETHODIMP CNewsDlg::ShowContextMenu(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved)
{
	return S_OK;
}

BOOL CNewsDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_TAB:
		case VK_BACK:
		case VK_F5:
		case VK_F12:
		case VK_RETURN:
		case VK_ESCAPE:
		case VK_CONTROL:
			return TRUE;
		default:
			if (GetAsyncKeyState(VK_CONTROL))
			{
				return TRUE;
			}
			break;
		}
		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_MOUSEWHEEL:
		return TRUE;
	default:
		break;
	}
	return CDHtmlDialog::PreTranslateMessage(pMsg);
}

BOOL CNewsDlg::CreateControlSite(COleControlContainer* pContainer, COleControlSite** ppSite, UINT  nID , REFCLSID  clsid )
{
	if(ppSite == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CMyControlSite *pBrowserSite = new CMyControlSite (pContainer, this);

	if (!pBrowserSite)
		return FALSE;

	*ppSite = pBrowserSite;
	return TRUE;
}