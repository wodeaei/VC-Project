#pragma once

#ifdef _WIN32_WCE
#error "Windows CE ��֧�� CDHtmlDialog��"
#endif 

// CNewsDlg �Ի���

class CNewsDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CNewsDlg)

public:
	CNewsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewsDlg();
// ��д
// 	HRESULT OnButtonOK(IHTMLElement *pElement);
// 	HRESULT OnButtonCancel(IHTMLElement *pElement);

// �Ի�������
	enum { IDD = IDD_DIALOG_NEWS, IDH = IDR_HTML_NEWSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
