#pragma once
/****************************
�����ƣ�ShellLink
�๦�ܣ���ݷ�ʽ�ļ���װ��
����������ȡ�����ÿ�ݷ�ʽ�ļ��еĸ������ԡ�
�ӿڣ�

	Load();
	Save();

	GetShellLinkPath();
	GetArguments();
	GetDescription();
	GetHotkey();
	GetIconLocation();
	GetIDList(); //��ʱδ��
	GetTargetPath();
	GetShowCmd();
	GetWorkingDirectory();
	GetAdditionalInfo();

	SetShellLinkPath();
	SetArguments();
	SetDescription();
	SetHotkey();
	SetIconLocation();
	SetIDList(); //��ʱδ��
	SetPath();
	SetShowCmd();
	SetWorkingDirectory();

*****************************/
#include "ShellLinkEx.h"
#include <shobjidl.h>


class ShellLink : public ShellLinkEx
{
protected:
	IShellLink*   m_pISL;
	IPersistFile* m_pIPF;
	char * m_fileBuffer;
	WCHAR * m_filePath;
	WCHAR * m_targetPath;
	WCHAR * m_workingDirectory;
	WCHAR * m_arguments;			//maximum length win2000 MAX_PATH or later INFOTIPSIZE
	WCHAR * m_description;			//maximum length win2000 MAX_PATH or later INFOTIPSIZE
	WCHAR * m_IconLocation;
	int m_IconIndex;
	DWORD m_fileBufferLeng;
	DWORD m_filePathLeng;
	DWORD m_targetPathLeng;
	DWORD m_workingDirectoryLeng;
	DWORD m_argumentsLeng;
	DWORD m_descriptionLeng;
	DWORD m_IconLocationLeng;

	void ReleaseAll(void);
	
public:
	ShellLink(void);
	virtual ~ShellLink(void);

	virtual BOOL Load(WCHAR * filePath);
	virtual BOOL Save(WCHAR * filePath=NULL);

	virtual const WCHAR * GetShellLinkPath(void);
	virtual const WCHAR *  GetArguments(void);
	virtual const WCHAR *  GetDescription(void);
	virtual WORD GetHotkey();
	virtual const WCHAR *  GetIconLocation(int *piIcon);
	virtual int GetIDList(PIDLIST_ABSOLUTE *ppidl);
	virtual const WCHAR *  GetTargetPath(void);
	virtual int GetShowCmd();
	virtual const WCHAR *  GetWorkingDirectory(void);
	//int GetAdditionalInfo();

	virtual BOOL SetArguments(WCHAR * arguments);
	virtual BOOL SetDescription(WCHAR * description);
	virtual BOOL SetHotkey(WORD hotkey);
	virtual BOOL SetIconLocation(WCHAR * iconPath, int iconIndex);
	virtual BOOL SetIDList(PCIDLIST_ABSOLUTE  ppidl);
	virtual BOOL SetTargetPath(WCHAR * targetPath);
	virtual BOOL SetRelativePath(WCHAR * relativePath);
	virtual BOOL SetShowCmd(int showCmd);
	virtual BOOL SetWorkingDirectory(WCHAR * workingDir);

	virtual BOOL Resolve(HWND hwnd, DWORD fFlags);

	virtual BOOL TargetPathExists();

};
