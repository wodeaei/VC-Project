#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// CNewsDlg 对话框

class CNewsDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CNewsDlg)

public:
	CNewsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CNewsDlg();
// 重写
// 	HRESULT OnButtonOK(IHTMLElement *pElement);
// 	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_DIALOG_NEWS, IDH = IDR_HTML_NEWSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL CreateControlSite(COleControlContainer* pContainer,
		COleControlSite** ppSite, UINT  nID , REFCLSID  clsid ); 
	STDMETHOD(GetHostInfo)(DOCHOSTUIINFO* pInfo);
	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
