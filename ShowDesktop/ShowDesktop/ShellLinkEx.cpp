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
		//只检测可以获取路径字符串的快捷方式文件。
		/*if (wcslen(filePath)==0)
		{
			filePath=GetWorkingDirectory();
		}*/
		//跳过网络地址
		if (filePath[0]==L'\\')
		{
			return TRUE;
		}
		return PathFileExists(filePath);
	}else{
		return TRUE;
	}
}
