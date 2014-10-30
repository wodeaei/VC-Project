#pragma once

class ShellLinkEx
{
public:
	ShellLinkEx(void);
	virtual ~ShellLinkEx(void);

	virtual BOOL Load(WCHAR * filePath) = 0;
	virtual BOOL Save(WCHAR * filePath=NULL) = 0;

	virtual const WCHAR * GetShellLinkPath(void) = 0;
	virtual const WCHAR *  GetArguments(void) = 0;
	virtual const WCHAR *  GetDescription(void) = 0;
	virtual WORD GetHotkey() = 0;
	virtual const WCHAR *  GetIconLocation(int *piIcon) = 0;
	virtual int GetIDList(PIDLIST_ABSOLUTE *ppidl) = 0;
	virtual const WCHAR *  GetTargetPath(void) = 0;
	virtual int GetShowCmd() = 0;
	virtual const WCHAR *  GetWorkingDirectory(void) = 0;
	//int GetAdditionalInfo();

	virtual BOOL SetArguments(WCHAR * arguments) = 0;
	virtual BOOL SetDescription(WCHAR * description) = 0;
	virtual BOOL SetHotkey(WORD hotkey) = 0;
	virtual BOOL SetIconLocation(WCHAR * iconPath, int iconIndex) = 0;
	virtual BOOL SetIDList(PCIDLIST_ABSOLUTE  ppidl) = 0;
	virtual BOOL SetTargetPath(WCHAR * targetPath) = 0;
	virtual BOOL SetRelativePath(WCHAR * relativePath) = 0;
	virtual BOOL SetShowCmd(int showCmd) = 0;
	virtual BOOL SetWorkingDirectory(WCHAR * workingDir) = 0;

	virtual BOOL Resolve(HWND hwnd, DWORD fFlags) = 0;

	virtual BOOL TargetPathExists();
};
