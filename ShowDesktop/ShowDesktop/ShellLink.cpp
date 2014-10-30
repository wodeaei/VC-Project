#include "StdAfx.h"
#include "shlwapi.h"
#include "ShellLink.h"

ShellLink::ShellLink(void)
: m_pISL(NULL)
, m_pIPF(NULL)
, m_fileBuffer(NULL)
, m_filePath(NULL)
, m_targetPath(NULL)
, m_workingDirectory(NULL)
, m_arguments(NULL)			//maximum length win2000 MAX_PATH or later INFOTIPSIZE
, m_description(NULL)		//maximum length win2000 MAX_PATH or later INFOTIPSIZE
, m_IconLocation(NULL)
, m_IconIndex(0)
, m_fileBufferLeng(0)
, m_filePathLeng(0)
, m_targetPathLeng(0)
, m_workingDirectoryLeng(0)
, m_argumentsLeng(0)
, m_descriptionLeng(0)
, m_IconLocationLeng(0)
{
	//��ʼ��COM�⡣
	CoInitialize(NULL);
	//��ʼ���ӿ�
	HRESULT hr = CoCreateInstance ( CLSID_ShellLink,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IShellLink,
		(void**) &m_pISL );
	if ( SUCCEEDED(hr))
	{
		// ��ȡ�������ĵڶ����ӿ�(IPersistFile)��
		hr = m_pISL->QueryInterface ( IID_IPersistFile, (void**) & m_pIPF );
		if ( SUCCEEDED(hr) )
		{
		}else
		{
			m_pIPF=NULL;
			m_pISL->Release();
			m_pISL=NULL;
		}
	}else{
		m_pISL=NULL;
	}
}

ShellLink::~ShellLink(void)
{
	if(m_filePath!=NULL)
	{
		ReleaseAll();
	}
	m_pIPF->Release();
	m_pIPF=NULL;
	m_pISL->Release();
	m_pISL=NULL;
	//����COM�⡣
	CoUninitialize();
}
/************************************
�������ƣ�ReleaseAll
�������ܣ��ͷŶ�����������Դ��
*************************************/
void ShellLink::ReleaseAll( void )
{
	if(m_IconLocation!=NULL)
	{
		delete  [] m_IconLocation;
		m_IconLocation=NULL;
		m_IconLocationLeng=0;
		m_IconIndex=0;
	}
	if (m_description!=NULL)
	{
		delete [] m_description;
		m_description=NULL;
		m_descriptionLeng=0;
	}
	if (m_arguments!=NULL)
	{
		delete [] m_arguments;
		m_arguments=NULL;
		m_argumentsLeng=0;
	}
	if (m_workingDirectory!=NULL)
	{
		delete [] m_workingDirectory;
		m_workingDirectory=NULL;
		m_workingDirectoryLeng=0;
	}
	if (m_targetPath!=NULL)
	{
		delete [] m_targetPath;
		m_targetPath=NULL;
		m_targetPathLeng=0;
	}
	if (m_filePath!=NULL)
	{
		delete [] m_filePath;
		m_filePath=NULL;
		m_filePath=0;
	}
	if (m_fileBuffer!=NULL)
	{
		delete [] m_fileBuffer;
		m_fileBuffer=NULL;
		m_fileBufferLeng=0;
	}
	/*if(m_pIPF!=NULL)
	{
		m_pIPF->Release();
	}
	if (m_pISL!=NULL)
	{
		m_pISL->Release();
	}*/
}
/************************************
�������ƣ�Load
�������ܣ����ؿ�ݷ�ʽ�ļ���
�����б�
	WCHAR * filePath: ��ݷ�ʽ�ļ�·����
����ֵ��
	TRUE ���سɹ���FALSE ����ʧ�ܣ��ļ����ɶ����ļ���ʽ����ȷ��

*************************************/
BOOL ShellLink::Load( WCHAR * filePath )
{
	if (m_pISL==NULL)
	{
		return FALSE;
	}
	if (m_filePath!=NULL)
	{
		ReleaseAll();
	}
	HRESULT hr;
	if (m_pIPF==NULL)
	{
		hr=m_pISL->QueryInterface ( IID_IPersistFile, (void**) & m_pIPF );
		if (!SUCCEEDED(hr))
		{
			return FALSE;
		}
	}
	hr = m_pIPF->Load(filePath,FALSE);
	if ( SUCCEEDED(hr))
	{
		m_filePathLeng=wcslen(filePath)+1;
		m_filePath=new WCHAR[m_filePathLeng];
		if (m_filePath==NULL)
		{
			return NULL;
		}
		wcscpy_s(m_filePath,m_filePathLeng,filePath);
		return TRUE;
	}
	return FALSE;
}

/************************************
�������ƣ�Save
�������ܣ������ݷ�ʽ�ļ���
�����б�
WCHAR * filePath: ��Ҫ����Ŀ�ݷ�ʽ�ļ�·����NULL ��ʾʹ����Loadʱ���ļ�·����
����ֵ��
TRUE ����ɹ���FALSE ����ʧ�ܡ�

*************************************/
BOOL ShellLink::Save( WCHAR * filePath/*=NULL*/ )
{
	WCHAR * savePath=NULL;
	if (filePath==NULL)
	{
		savePath=m_filePath;
	}else{
		savePath=filePath;
	}
	if (savePath==NULL)
	{
		return FALSE;
	}
	if (m_pISL==NULL)
	{
		return FALSE;
	}
	
	HRESULT hr;
	if (m_pIPF==NULL)
	{
		hr=m_pISL->QueryInterface ( IID_IPersistFile, (void**) & m_pIPF );
		if (!SUCCEEDED(hr))
		{
			return FALSE;
		}
	}
	hr = m_pIPF->Save(filePath,FALSE);
	if ( SUCCEEDED(hr))
	{
		return TRUE;
	}
	return FALSE;
}
/************************************
�������ƣ�GetShellLinkPath
�������ܣ���ȡ��ݷ�ʽ�ļ�·����
�����б�
 
����ֵ��
	const WCHAR * : ָ���ݷ�ʽ�ļ�·���ַ�����ָ��,���·�������ڷ���NULL��
*************************************/
const WCHAR * ShellLink::GetShellLinkPath( void )
{
	return m_filePath;
}
/************************************
�������ƣ�GetArguments
�������ܣ���ȡ��ݷ�ʽ�Ĳ�����Ϣ��   Gets the command-line arguments associated with a Shell link object.
�����б�

����ֵ��
const WCHAR * : ָ���ݷ�ʽ�ļ�������Ϣ�ַ�����ָ��,���û�в�����Ϣ����NULL��
*************************************/
const WCHAR * ShellLink::GetArguments( void )
{
	HRESULT hr;
	
	if (m_pISL!=NULL)
	{
		
		if (m_arguments==NULL)
		{
			m_argumentsLeng=INFOTIPSIZE;
			m_arguments=new WCHAR[m_argumentsLeng];
			if (m_arguments==NULL)
			{
				return NULL;
			}
			hr= m_pISL->GetArguments(m_arguments,m_argumentsLeng);
			if ( !SUCCEEDED(hr))
			{
				delete m_arguments;
				m_arguments=NULL;
				m_argumentsLeng=0;
				return NULL;
			}
		}
		if (wcslen(m_arguments)==0)
		{
			return NULL;
		}else{
			return m_arguments;
		}
		
		//wcschr()
		//memchr()
	}
	return NULL;

}
/************************************
�������ƣ�GetDescription
�������ܣ���ȡ��ݷ�ʽ��������Ϣ��  Gets the description string for a Shell link object.
�����б�

����ֵ��
const WCHAR * : ָ���ݷ�ʽ�ļ�������Ϣ�ַ�����ָ��,���û��������Ϣ����NULL��
*************************************/
const WCHAR * ShellLink::GetDescription( void )
{
	HRESULT hr;

	if (m_pISL!=NULL)
	{

		if (m_description==NULL)
		{
			m_descriptionLeng=INFOTIPSIZE;
			m_description=new WCHAR[m_descriptionLeng];
			if (m_description==NULL)
			{
				return NULL;
			}
			hr= m_pISL->GetDescription(m_description,m_descriptionLeng);
			if ( !SUCCEEDED(hr))
			{
				delete m_description;
				m_description=NULL;
				m_descriptionLeng=0;
				return NULL;
			}
		}
		if (wcslen(m_description)==0)
		{
			return NULL;
		}else{
			return m_description;
		}

		//wcschr()
		//memchr()
	}
	return NULL;

}
/************************************
�������ƣ�GetHotkey
�������ܣ���ȡ��ݷ�ʽ���ȼ���  Gets the hot key for a Shell link object.
�����б�

����ֵ��
WORD : ���ؿ�ݷ�ʽ�ļ������õ��ȼ�ֵ,û���ȼ��򷵻�0��

The address of the hot key. The virtual key code is in the low-order byte, and the modifier flags are in the high-order byte. The modifier flags can be a combination of the following values.
HOTKEYF_ALT
ALT key 
HOTKEYF_CONTROL
CTRL key 
HOTKEYF_EXT
Extended key 
HOTKEYF_SHIFT
SHIFT key 

*************************************/
WORD ShellLink::GetHotkey()
{
	HRESULT hr;
	WORD reHotKey;

	if (m_pISL!=NULL)
	{
		hr= m_pISL->GetHotkey(&reHotKey);

		if (SUCCEEDED(hr))
		{
			return reHotKey;
		}
	}
	return 0;

}
/************************************
�������ƣ�GetIconLocation
�������ܣ���ȡ��ݷ�ʽ��ͼ�ꡣ  Gets the location (path and index) of the icon for a Shell link object. 
�����б�
int * piIcon : ���ָ����������ͼ���������

����ֵ��
const WCHAR * : ���ؿ�ݷ�ʽ����ͼ����ļ�·��,���û�з���NULL��

*************************************/
const WCHAR * ShellLink::GetIconLocation( int *piIcon )
{
	HRESULT hr;

	if (m_pISL!=NULL)
	{

		if (m_IconLocation==NULL)
		{
			m_IconLocationLeng=INFOTIPSIZE;
			m_IconLocation=new WCHAR[m_IconLocationLeng];
			if (m_IconLocation==NULL)
			{
				return NULL;
			}
			hr= m_pISL->GetIconLocation(m_IconLocation,m_IconLocationLeng,&m_IconIndex);
			if ( !SUCCEEDED(hr))
			{
				delete m_IconLocation;
				m_IconLocation=NULL;
				m_IconLocationLeng=0;
				m_IconIndex=0;
				return NULL;
			}
		}
		if (wcslen(m_IconLocation)==0)
		{
			return NULL;
		}else{
			*piIcon=m_IconIndex;
			return m_IconLocation;
		}
	}
	return NULL;

}
/************************************
�������ƣ�GetTargetPath
�������ܣ���ȡ��ݷ�ʽ��Ŀ���ļ�·����  Gets the path and file name of a Shell link object.
�����б�

����ֵ��
const WCHAR * : ���ؿ�ݷ�ʽ��Ŀ���ļ�·��,���û�з���NULL��

*************************************/
const WCHAR * ShellLink::GetTargetPath( void )
{
	HRESULT hr;
	
//	LPITEMIDLIST  PIDList;
//	LPCITEMIDLIST pItemChild;
//	IShellFolder * spShellFolder;
//	STRRET strDispName;
	if (m_pISL!=NULL)
	{

		//m_pISL->Resolve(NULL,0);
		if (m_targetPath==NULL)
		{
			m_targetPathLeng=INFOTIPSIZE;
			m_targetPath=new WCHAR[m_targetPathLeng];
			if (m_targetPath==NULL)
			{
				return NULL;
			}
			hr= m_pISL->GetPath(m_targetPath,m_targetPathLeng,NULL,NULL);
			//m_pISL->GetWorkingDirectory()
			if ( !SUCCEEDED(hr))
			{
				delete m_targetPath;
				m_targetPath=NULL;
				m_targetPathLeng=0;
				return NULL;
			}
			//��ȡURL ·��
			//if (wcslen(m_targetPath)==0)
			//{
			//	hr=m_pISL->GetIDList(&PIDList);
			//	if ( !SUCCEEDED(hr))
			//	{
			//		delete m_targetPath;
			//		m_targetPath=NULL;
			//		m_targetPathLeng=0;
			//		return NULL;
			//	}
			//	//PIDList->mkid.cb=1;
			//	hr= SHBindToParent(PIDList,IID_IShellFolder,(LPVOID * ) &spShellFolder,&pItemChild);
			//	if (S_OK!=hr||NULL==spShellFolder)
			//	{
			//		delete m_targetPath;
			//		m_targetPath=NULL;
			//		m_targetPathLeng=0;
			//		return NULL;
			//	}

			//	hr =spShellFolder->GetDisplayNameOf(pItemChild,SHGDN_FORPARSING|SHGDN_INFOLDER ,&strDispName);
			//	if (hr!= S_OK)
			//	{
			//		delete m_targetPath;
			//		m_targetPath=NULL;
			//		m_targetPathLeng=0;
			//		return NULL;
			//	}
			//	hr= StrRetToBufW(&strDispName,PIDList,m_targetPath,m_targetPathLeng);

			//}
		}
		if (wcslen(m_targetPath)==0)
		{
			return NULL;
		}else{
			return m_targetPath;
		}
	}
	return NULL;

}
/************************************
�������ƣ�GetShowCmd
�������ܣ���ȡ�򿪿�ݷ�ʽʱʹ�õĴ������͡�  Gets the show command for a Shell link object..
�����б�

����ֵ��
int : ���ش򿪿�ݷ�ʽʱʹ�õĴ�������,���û�з���0��

A pointer to the command. The following commands are supported.
SW_SHOWNORMAL
Activates and displays a window. If the window is minimized or maximized, the system restores it to its original size and position. An application should specify this flag when displaying the window for the first time.
SW_SHOWMAXIMIZED
Activates the window and displays it as a maximized window.
SW_SHOWMINIMIZED
Activates the window and displays it as a minimized window.

*************************************/
int ShellLink::GetShowCmd()
{
	HRESULT hr;
	int reShowCmd;

	if (m_pISL!=NULL)
	{
		hr= m_pISL->GetShowCmd(&reShowCmd);

		if (SUCCEEDED(hr))
		{
			return reShowCmd;
		}
	}
	return 0;
}
/************************************
�������ƣ�GetWorkingDirectory
�������ܣ���ȡ��ݷ�ʽ�Ĺ���Ŀ¼·���� Gets the name of the working directory for a Shell link object. 
�����б�

����ֵ��
const WCHAR * : ���ؿ�ݷ�ʽ�Ĺ���Ŀ¼·��,���û�з���NULL��

*************************************/
const WCHAR * ShellLink::GetWorkingDirectory( void )
{
	HRESULT hr;

	if (m_pISL!=NULL)
	{

		if (m_workingDirectory==NULL)
		{
			m_workingDirectoryLeng=INFOTIPSIZE;
			m_workingDirectory=new WCHAR[m_workingDirectoryLeng];
			if (m_workingDirectory==NULL)
			{
				return NULL;
			}
			hr= m_pISL->GetWorkingDirectory(m_workingDirectory,m_workingDirectoryLeng);
			if ( !SUCCEEDED(hr))
			{
				delete m_workingDirectory;
				m_workingDirectory=NULL;
				m_workingDirectoryLeng=0;
				return NULL;
			}
		}
		if (wcslen(m_workingDirectory)==0)
		{
			return NULL;
		}else{
			return m_workingDirectory;
		}
	}
	return NULL;

}
/************************************
�������ƣ�SetArguments
�������ܣ����ÿ�ݷ�ʽ�ļ��������в����� Sets the command-line arguments for a Shell link object.
�����б�
WCHAR * arguments:�����в����ַ�����
����ֵ��
���óɹ�����TRUE ��ʧ�ܷ��� FALSE��
*************************************/
BOOL ShellLink::SetArguments( WCHAR * arguments )
{
	HRESULT hr;
	if (arguments!=NULL&&m_pISL!=NULL)
	{
		hr= m_pISL->SetArguments(arguments);

		if (SUCCEEDED(hr))
		{
			if (m_arguments!=NULL)
			{
				delete m_arguments;
			}
			m_argumentsLeng=wcslen(arguments)+1;
			m_arguments=new WCHAR[m_argumentsLeng];
			if (m_arguments!=NULL)
			{
				wcscpy_s(m_arguments,m_argumentsLeng,arguments);
			}
			
			return TRUE;
		}
	}
	return FALSE;

}
/************************************
�������ƣ�SetDescription
�������ܣ����ÿ�ݷ�ʽ�ļ���������Ϣ�� Sets the description for a Shell link object. The description can be any application-defined string.
�����б�
WCHAR * description:��ݷ�ʽ�ļ��������ַ�����
����ֵ��
���óɹ�����TRUE ��ʧ�ܷ��� FALSE��
*************************************/
BOOL ShellLink::SetDescription( WCHAR * description )
{
	HRESULT hr;
	if (description!=NULL&&m_pISL!=NULL)
	{
		hr= m_pISL->SetDescription(description);

		if (SUCCEEDED(hr))
		{
			if (m_description!=NULL)
			{
				delete m_description;
			}
			m_descriptionLeng=wcslen(description)+1;
			m_description=new WCHAR[m_descriptionLeng];
			if (m_description!=NULL)
			{
				wcscpy_s(m_description,m_descriptionLeng,description);
			}

			return TRUE;
		}
	}
	return FALSE;	

}
/************************************
�������ƣ�SetHotkey
�������ܣ����ÿ�ݷ�ʽ�ļ����ȼ���Sets a hot key for a Shell link object.
�����б�
WORD hotkey:�ȼ���ֵ��
����ֵ��
���óɹ�����TRUE ��ʧ�ܷ��� FALSE��
*************************************/
BOOL ShellLink::SetHotkey( WORD hotkey )
{
	HRESULT hr;
	if (m_pISL!=NULL)
	{
		hr= m_pISL->SetHotkey(hotkey);

		if (SUCCEEDED(hr))
		{
			return TRUE;
		}
	}
	return FALSE;	

}
/************************************
�������ƣ�SetIconLocation
�������ܣ����ÿ�ݷ�ʽ�ļ�ʹ�õ�ͼ���ļ�����·����ͼ�������� Sets the location (path and index) of the icon for a Shell link object.
�����б�
WCHAR * iconPath:ͼ������·���ַ�����
int iconIndex:ͼ��������
����ֵ��
���óɹ�����TRUE ��ʧ�ܷ��� FALSE��
*************************************/
BOOL ShellLink::SetIconLocation( WCHAR * iconPath, int iconIndex )
{
	HRESULT hr;
	if (iconPath!=NULL&&m_pISL!=NULL)
	{
		hr= m_pISL->SetIconLocation( iconPath, iconIndex);

		if (SUCCEEDED(hr))
		{
			if (m_IconLocation!=NULL)
			{
				delete m_IconLocation;
			}
			m_IconLocationLeng=wcslen(iconPath)+1;
			m_IconLocation=new WCHAR[m_IconLocationLeng];
			if (m_IconLocation!=NULL)
			{
				wcscpy_s(m_IconLocation,m_IconLocationLeng,iconPath);
				this->m_IconIndex=iconIndex;
			}

			return TRUE;
		}
	}
	return FALSE;	

}
/************************************
�������ƣ�SetTargetPath
�������ܣ����ÿ�ݷ�ʽ�ļ���Ŀ���ļ�·���� Sets the location (path and index) of the icon for a Shell link object.
�����б�
WCHAR * targetPath:Ŀ���ļ�·���ַ�����
����ֵ��
���óɹ�����TRUE ��ʧ�ܷ��� FALSE��
*************************************/
BOOL ShellLink::SetTargetPath( WCHAR * targetPath )
{
	HRESULT hr;
	if (targetPath!=NULL&&m_pISL!=NULL)
	{
		hr= m_pISL->SetPath( targetPath);

		if (SUCCEEDED(hr))
		{
			if (m_filePath!=NULL)
			{
				delete m_filePath;
			}
			m_filePathLeng=wcslen(targetPath)+1;
			m_filePath=new WCHAR[m_filePathLeng];
			if (m_filePath!=NULL)
			{
				wcscpy_s(m_filePath,m_filePathLeng,targetPath);
			}

			return TRUE;
		}
	}
	return FALSE;	

}
/************************************
�������ƣ�SetRelativePath
�������ܣ����ÿ�ݷ�ʽ�ļ���Ŀ������ļ�·���� Sets the relative path to the Shell link object.
�����б�
WCHAR * targetPath:Ŀ���ļ����·���ַ�����
����ֵ��
���óɹ�����TRUE ��ʧ�ܷ��� FALSE��
*************************************/
BOOL ShellLink::SetRelativePath( WCHAR * relativePath)
{
	HRESULT hr;
	if (relativePath!=NULL&&m_pISL!=NULL)
	{
		hr= m_pISL->SetRelativePath( relativePath, 0 );

		if (SUCCEEDED(hr))
		{
			return TRUE;
		}
	}
	return FALSE;	
}
/************************************
�������ƣ�SetShowCmd
�������ܣ����ô򿪿�ݷ�ʽʱʹ�õĴ��ڵ���ʾ���͡� Sets the show command for a Shell link object. The show command sets the initial show state of the window.
�����б�
 int showCmd :�򿪿�ݷ�ʽʱʹ�õĴ��ڵ���ʾ���͡���ϸ���ͼ�GetShowCmd��
����ֵ��
���óɹ�����TRUE ��ʧ�ܷ��� FALSE��
*************************************/
BOOL ShellLink::SetShowCmd( int showCmd )
{
	HRESULT hr;
	if (m_pISL!=NULL)
	{
		hr= m_pISL->SetShowCmd(showCmd);

		if (SUCCEEDED(hr))
		{
			return TRUE;
		}
	}
	return FALSE;

}
/************************************
�������ƣ�SetWorkingDirectory
�������ܣ����ÿ�ݷ�ʽ�Ĺ���Ŀ¼·���� Sets the show command for a Shell link object. The show command sets the initial show state of the window.
�����б�
WCHAR * workingDir :��ݷ�ʽ�Ĺ���Ŀ¼·���ַ�����
����ֵ��
���óɹ�����TRUE ��ʧ�ܷ��� FALSE��
*************************************/
BOOL ShellLink::SetWorkingDirectory( WCHAR * workingDir )
{
	HRESULT hr;
	if (workingDir!=NULL&&m_pISL!=NULL)
	{
		hr= m_pISL->SetPath( workingDir);

		if (SUCCEEDED(hr))
		{
			if (m_workingDirectory!=NULL)
			{
				delete m_workingDirectory;
			}
			m_workingDirectoryLeng=wcslen(workingDir)+1;
			m_workingDirectory=new WCHAR[m_workingDirectoryLeng];
			if (m_workingDirectory!=NULL)
			{
				wcscpy_s(m_workingDirectory,m_workingDirectoryLeng,workingDir);
			}

			return TRUE;
		}
	}
	return FALSE;	
}
/************************************
�������ƣ�TargetPathExists
�������ܣ����Ŀ���ļ��Ƿ���ڡ�
�����б�

����ֵ��
���ڷ���TRUE �����򷵻� FALSE��
*************************************/
BOOL ShellLink::TargetPathExists()
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
	//HRESULT hr;
	//hr=m_pISL->Resolve(NULL,SLR_NO_UI);
	//if (hr!=S_OK )
	//{
	//	return FALSE;
	//}else{
	//	return TRUE;
	//}
	//return FALSE;
	
}
/************************************
�������ƣ�Resolve
�������ܣ�������ݷ�ʽ�ļ���
�����б�
HWND hwnd����Ҫ��ʾ������Ϣʱ��������ھ��������Ϊ�����ڡ�
DWORD fFlags ��������ʽ��

SLR_INVOKE_MSI
Call the Microsoft Windows Installer.
SLR_NOLINKINFO
Disable distributed link tracking. By default, distributed link tracking tracks removable media across multiple devices based on the volume name. It also uses the Universal Naming Convention (UNC) path to track remote file systems whose drive letter has changed. Setting SLR_NOLINKINFO disables both types of tracking.
SLR_NO_UI
Do not display a dialog box if the link cannot be resolved. When SLR_NO_UI is set, the high-order word of fFlags can be set to a time-out value that specifies the maximum amount of time to be spent resolving the link. The function returns if the link cannot be resolved within the time-out duration. If the high-order word is set to zero, the time-out duration will be set to the default value of 3,000 milliseconds (3 seconds). To specify a value, set the high word of fFlags to the desired time-out duration, in milliseconds.
SLR_NOUPDATE
Do not update the link information.
SLR_NOSEARCH
Do not execute the search heuristics.
SLR_NOTRACK
Do not use distributed link tracking.
SLR_UPDATE
If the link object has changed, update its path and list of identifiers. If SLR_UPDATE is set, you do not need to call IPersistFile::IsDirty to determine whether or not the link object has changed.
 
����ֵ��
��ݷ�ʽ��Ч����TRUE �����򷵻� FALSE��
*************************************/
BOOL ShellLink::Resolve( HWND hwnd, DWORD fFlags )
{
	HRESULT hr;
	if (m_pISL!=NULL)
	{
		hr= m_pISL->Resolve(hwnd, fFlags);

		if (hr==S_OK)
		{
			return TRUE;
		}
	}
	return FALSE;

}

int ShellLink::GetIDList( PIDLIST_ABSOLUTE *ppidl )
{

	HRESULT hr;
	if (m_pISL!=NULL)
	{
		hr= m_pISL->GetIDList(ppidl);

		if (SUCCEEDED(hr))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL ShellLink::SetIDList( PCIDLIST_ABSOLUTE ppidl )
{

	HRESULT hr;
	if (m_pISL!=NULL)
	{
		hr= m_pISL->SetIDList(ppidl);

		if (SUCCEEDED(hr))
		{
			return TRUE;
		}
	}
	return FALSE;
}