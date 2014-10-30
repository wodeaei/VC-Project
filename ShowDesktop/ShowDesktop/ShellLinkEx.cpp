#include "StdAfx.h"
#include <shlwapi.h>
#include "ShellLinkEx.h"

ShellLinkEx::ShellLinkEx(void)
{
}

ShellLinkEx::~ShellLinkEx(void)
{
}

BOOL ShellLinkEx::TargetPathExists()
{
	const WCHAR * filePath=GetTargetPath();
	if (filePath!=NULL)
	{
		//ֻ�����Ի�ȡ·���ַ����Ŀ�ݷ�ʽ�ļ���
		/*if (wcslen(filePath)==0)
		{
			filePath=GetWorkingDirectory();
		}*/
		//���������ַ
		if (filePath[0]==L'\\')
		{
			return TRUE;
		}
		return PathFileExists(filePath);
	}else{
		return TRUE;
	}
}
