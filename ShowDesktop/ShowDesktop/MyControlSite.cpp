#include "StdAfx.h"
#include "MyControlSite.h"

BEGIN_INTERFACE_MAP(CMyControlSite, COleControlSite)
	INTERFACE_PART(CMyControlSite, IID_IDocHostUIHandler, DocHostUIHandler)
	INTERFACE_PART(CMyControlSite, IID_IOleCommandTarget, OleCommandTarget)
END_INTERFACE_MAP()

HRESULT CMyControlSite::XOleCommandTarget::Exec(const GUID* pguidCmdGroup, DWORD nCmdID,DWORD nCmdexecopt, VARIANTARG* pvaIn, VARIANTARG* pvaOut )
{
	HRESULT hr = OLECMDERR_E_NOTSUPPORTED;
	//return S_OK;
	if (pguidCmdGroup && IsEqualGUID(*pguidCmdGroup, CGID_DocHostCommandHandler))
	{
		switch (nCmdID)
		{
		case OLECMDID_SHOWSCRIPTERROR:
			{
				IHTMLDocument2* pDoc = NULL;
				IHTMLWindow2* pWindow = NULL;
				IHTMLEventObj* pEventObj = NULL;
				BSTR rgwszNames[5] =
				{
					SysAllocString(L"errorLine"),
					SysAllocString(L"errorCharacter"),
					SysAllocString(L"errorCode"),
					SysAllocString(L"errorMessage"),
					SysAllocString(L"errorUrl")
				};

				DISPID rgDispIDs[5];
				VARIANT rgvaEventInfo[5];
				DISPPARAMS params;
				BOOL fContinueRunningScripts = true;
				params.cArgs = 0;
				params.cNamedArgs = 0;
				hr = pvaIn->punkVal->QueryInterface(IID_IHTMLDocument2, (void **) &pDoc);
				hr = pDoc->get_parentWindow(&pWindow);

				pDoc->Release();
				hr = pWindow->get_event(&pEventObj);

				for (int i = 0; i < 5; i++)
				{
					// Get the property's dispID.
					hr = pEventObj->GetIDsOfNames(IID_NULL, &rgwszNames[i], 1,
						LOCALE_SYSTEM_DEFAULT, &rgDispIDs[i]);
					// Get the value of the property.
					hr = pEventObj->Invoke(rgDispIDs[i], IID_NULL,LOCALE_SYSTEM_DEFAULT,
						DISPATCH_PROPERTYGET, &params, &rgvaEventInfo[i],NULL, NULL);
					//必须使用SysFreeString来释放SysAllocString分配的内存,SysAllocString在分配的内存中记录了字符的长度
					SysFreeString(rgwszNames[i]);
				}
				// At this point, you would normally alert the user with
				// the information about the error, which is now contained
				// in rgvaEventInfo[]. Or, you could just exit silently.
				(*pvaOut).vt = VT_BOOL;
				if (fContinueRunningScripts)
				{
					// 在页面中继续执行脚本
					(*pvaOut).boolVal = VARIANT_TRUE;
				}
				else
				{
					// 停止在页面中执行脚本
					(*pvaOut).boolVal = VARIANT_FALSE;
				}
				break;
			}
		case OLECMDID_SHOWMESSAGE:
			(*pvaOut).boolVal = VARIANT_TRUE;
#ifndef TEST_RELEASE
hr = S_OK;
#endif	
			break;
		default:
			hr =OLECMDERR_E_NOTSUPPORTED;
			break;
		}
	}
	else
	{
		hr = OLECMDERR_E_UNKNOWNGROUP;
	}
	return (hr);
}

ULONG FAR EXPORT CMyControlSite::XOleCommandTarget::AddRef()
{
	METHOD_PROLOGUE(CMyControlSite, OleCommandTarget)
		return pThis->ExternalAddRef();
}

ULONG FAR EXPORT CMyControlSite::XOleCommandTarget::Release()
{
	METHOD_PROLOGUE(CMyControlSite, OleCommandTarget)
		return pThis->ExternalRelease();
}

HRESULT FAR EXPORT CMyControlSite::XOleCommandTarget::QueryInterface(REFIID riid, void **ppvObj)
{
	METHOD_PROLOGUE(CMyControlSite, OleCommandTarget)
		HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&riid, ppvObj);
	return hr;
}

STDMETHODIMP CMyControlSite::XOleCommandTarget::QueryStatus(
	/* [unique][in] */ const GUID __RPC_FAR *pguidCmdGroup,
	/* [in] */ ULONG cCmds,
	/* [out][in][size_is] */ OLECMD __RPC_FAR prgCmds[ ],
	/* [unique][out][in] */ OLECMDTEXT __RPC_FAR *pCmdText)
{
	METHOD_PROLOGUE(CMyControlSite, OleCommandTarget)
		return OLECMDERR_E_NOTSUPPORTED;
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::GetExternal(LPDISPATCH *lppDispatch)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->GetExternal(lppDispatch);
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::ShowContextMenu(DWORD dwID, LPPOINT ppt, LPUNKNOWN pcmdTarget, LPDISPATCH pdispReserved)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->ShowContextMenu(dwID, ppt, pcmdTarget, pdispReserved);
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::GetHostInfo(DOCHOSTUIINFO *pInfo)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->GetHostInfo(pInfo);
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::ShowUI(DWORD dwID, LPOLEINPLACEACTIVEOBJECT pActiveObject,LPOLECOMMANDTARGET pCommandTarget, LPOLEINPLACEFRAME pFrame,LPOLEINPLACEUIWINDOW pDoc)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->ShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::HideUI(void)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->HideUI();
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::UpdateUI(void)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->UpdateUI();
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::EnableModeless(BOOL fEnable)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->EnableModeless(fEnable);
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::OnDocWindowActivate(BOOL fActivate)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->OnDocWindowActivate(fActivate);
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::OnFrameWindowActivate(BOOL fActivate)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->OnFrameWindowActivate(fActivate);
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::ResizeBorder(LPCRECT prcBorder, LPOLEINPLACEUIWINDOW pUIWindow, BOOL fFrameWindow)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->ResizeBorder(prcBorder, pUIWindow, fFrameWindow);
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::TranslateAccelerator(LPMSG lpMsg, const GUID* pguidCmdGroup, DWORD nCmdID)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->TranslateAccelerator(lpMsg, pguidCmdGroup, nCmdID);
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::GetOptionKeyPath(LPOLESTR* pchKey, DWORD dwReserved)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->GetOptionKeyPath(pchKey, dwReserved);
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::GetDropTarget(LPDROPTARGET pDropTarget, LPDROPTARGET* ppDropTarget)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->GetDropTarget(pDropTarget, ppDropTarget);
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::TranslateUrl(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->TranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::FilterDataObject(LPDATAOBJECT pDataObject, LPDATAOBJECT* ppDataObject)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->m_pHandler->FilterDataObject(pDataObject, ppDataObject);
}

STDMETHODIMP_(ULONG) CMyControlSite::XDocHostUIHandler::AddRef()
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CMyControlSite::XDocHostUIHandler::Release()
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->ExternalRelease();
}

STDMETHODIMP CMyControlSite::XDocHostUIHandler::QueryInterface(REFIID iid, LPVOID far* ppvObj)
{
	METHOD_PROLOGUE_EX_(CMyControlSite, DocHostUIHandler)
		return pThis->ExternalQueryInterface(&iid, ppvObj);
}