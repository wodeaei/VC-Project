// ShowDesktop.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CShowDesktopApp:
// �йش����ʵ�֣������ ShowDesktop.cpp
//

class CShowDesktopApp : public CWinApp
{
public:
	CShowDesktopApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CShowDesktopApp theApp;