// MFC_Excel_Demo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFC_Excel_DemoApp:
// �йش����ʵ�֣������ MFC_Excel_Demo.cpp
//

class CMFC_Excel_DemoApp : public CWinApp
{
public:
	CMFC_Excel_DemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CMFC_Excel_DemoApp theApp;