#pragma once
#include "afxocc.h"
//参考http://www.cnblogs.com/thinkingfor/archive/2010/11/10/1873689.html#2638976
/*
用法：
对应的对话框程序中重写：
对话框声明中添加：
virtual BOOL CreateControlSite(COleControlContainer* pContainer,
COleControlSite** ppSite, UINT  nID , REFCLSID  clsid ); 
对话框实现中添加：
BOOL CXXXXDlg::CreateControlSite(COleControlContainer* pContainer, 
COleControlSite** ppSite, UINT  nID , REFCLSID  clsid )
{
if(ppSite == NULL)
{
ASSERT(FALSE);
return FALSE;
}

CMyControlSite *pBrowserSite = 
new CMyControlSite (pContainer, this);

if (!pBrowserSite)
return FALSE;

*ppSite = pBrowserSite;
return TRUE;
}

*/
class CMyControlSite :
	public COleControlSite
{
public:  
	CMyControlSite(COleControlContainer *pCntr):COleControlSite(pCntr) {}

protected:  
	DECLARE_INTERFACE_MAP()  
	BEGIN_INTERFACE_PART(OleCommandTarget, IOleCommandTarget)  
		STDMETHOD(QueryStatus)(const GUID *pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT *pCmdText);  
		STDMETHOD(Exec)(const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANTARG* pvaIn, VARIANTARG* pvaOut);  
	END_INTERFACE_PART(OleCommandTarget) 

public: 
	CMyControlSite(COleControlContainer *pCnt, CDHtmlDialog *pHandler):COleControlSite(pCnt),m_pHandler(pHandler) {} 

protected: 
	CDHtmlDialog *m_pHandler; 

	BEGIN_INTERFACE_PART(DocHostUIHandler, IDocHostUIHandler)
		STDMETHOD(ShowContextMenu)(DWORD, LPPOINT, LPUNKNOWN, LPDISPATCH);
		STDMETHOD(GetHostInfo)(DOCHOSTUIINFO*);
		STDMETHOD(ShowUI)(DWORD, LPOLEINPLACEACTIVEOBJECT,LPOLECOMMANDTARGET, LPOLEINPLACEFRAME, LPOLEINPLACEUIWINDOW);
		STDMETHOD(HideUI)(void);
		STDMETHOD(UpdateUI)(void);
		STDMETHOD(EnableModeless)(BOOL);
		STDMETHOD(OnDocWindowActivate)(BOOL);
		STDMETHOD(OnFrameWindowActivate)(BOOL);
		STDMETHOD(ResizeBorder)(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL);
		STDMETHOD(TranslateAccelerator)(LPMSG, const GUID*, DWORD);
		STDMETHOD(GetOptionKeyPath)(OLECHAR **, DWORD);
		STDMETHOD(GetDropTarget)(LPDROPTARGET, LPDROPTARGET*);
		STDMETHOD(GetExternal)(LPDISPATCH*);
		STDMETHOD(TranslateUrl)(DWORD, OLECHAR*, OLECHAR **);
		STDMETHOD(FilterDataObject)(LPDATAOBJECT , LPDATAOBJECT*);
	END_INTERFACE_PART(DocHostUIHandler) 
};
