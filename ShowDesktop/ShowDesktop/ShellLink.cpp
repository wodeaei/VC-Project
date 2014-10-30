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
	//初始化COM库。
	CoInitialize(NULL);
	//初始化接口
	HRESULT hr = CoCreateInstance ( CLSID_ShellLink,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IShellLink,
		(void**) &m_pISL );
	if ( SUCCEEDED(hr))
	{
		// 获取这个对象的第二个接口(IPersistFile)。
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
	//回收COM库。
	CoUninitialize();
}
/************************************
函数名称：ReleaseAll
函数功能：释放对象中所有资源。
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
函数名称：Load
函数功能：加载快捷方式文件。
参数列表：
	WCHAR * filePath: 快捷方式文件路径。
返回值：
	TRUE 加载成功，FALSE 加载失败，文件不可读或文件格式不正确。

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
函数名称：Save
函数功能：保存快捷方式文件。
参数列表：
WCHAR * filePath: 将要保存的快捷方式文件路径，NULL 表示使用在Load时的文件路径。
返回值：
TRUE 保存成功，FALSE 保存失败。

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
函数名称：GetShellLinkPath
函数功能：获取快捷方式文件路径。
参数列表：
 
返回值：
	const WCHAR * : 指向快捷方式文件路径字符串的指针,如果路径不存在返回NULL。
*************************************/
const WCHAR * ShellLink::GetShellLinkPath( void )
{
	return m_filePath;
}
/************************************
函数名称：GetArguments
函数功能：获取快捷方式的参数信息。   Gets the command-line arguments associated with a Shell link object.
参数列表：

返回值：
const WCHAR * : 指向快捷方式文件参数信息字符串的指针,如果没有参数信息返回NULL。
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
函数名称：GetDescription
函数功能：获取快捷方式的描述信息。  Gets the description string for a Shell link object.
参数列表：

返回值：
const WCHAR * : 指向快捷方式文件描述信息字符串的指针,如果没有描述信息返回NULL。
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
函数名称：GetHotkey
函数功能：获取快捷方式的热键。  Gets the hot key for a Shell link object.
参数列表：

返回值：
WORD : 返回快捷方式文件中设置的热键值,没有热键则返回0。

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
函数名称：GetIconLocation
函数功能：获取快捷方式的图标。  Gets the location (path and index) of the icon for a Shell link object. 
参数列表：
int * piIcon : 这个指针用来返回图标的索引。

返回值：
const WCHAR * : 返回快捷方式容纳图标的文件路径,如果没有返回NULL。

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
函数名称：GetTargetPath
函数功能：获取快捷方式的目标文件路径。  Gets the path and file name of a Shell link object.
参数列表：

返回值：
const WCHAR * : 返回快捷方式的目标文件路径,如果没有返回NULL。

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
			//获取URL 路径
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
函数名称：GetShowCmd
函数功能：获取打开快捷方式时使用的窗口类型。  Gets the show command for a Shell link object..
参数列表：

返回值：
int : 返回打开快捷方式时使用的窗口类型,如果没有返回0。

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
函数名称：GetWorkingDirectory
函数功能：获取快捷方式的工作目录路径。 Gets the name of the working directory for a Shell link object. 
参数列表：

返回值：
const WCHAR * : 返回快捷方式的工作目录路径,如果没有返回NULL。

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
函数名称：SetArguments
函数功能：设置快捷方式文件的命令行参数。 Sets the command-line arguments for a Shell link object.
参数列表：
WCHAR * arguments:命令行参数字符串。
返回值：
设置成功返回TRUE ，失败返回 FALSE。
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
函数名称：SetDescription
函数功能：设置快捷方式文件的描述信息。 Sets the description for a Shell link object. The description can be any application-defined string.
参数列表：
WCHAR * description:快捷方式文件的描述字符串。
返回值：
设置成功返回TRUE ，失败返回 FALSE。
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
函数名称：SetHotkey
函数功能：设置快捷方式文件的热键。Sets a hot key for a Shell link object.
参数列表：
WORD hotkey:热键键值。
返回值：
设置成功返回TRUE ，失败返回 FALSE。
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
函数名称：SetIconLocation
函数功能：设置快捷方式文件使用的图标文件所在路径和图标索引。 Sets the location (path and index) of the icon for a Shell link object.
参数列表：
WCHAR * iconPath:图标所在路径字符串。
int iconIndex:图标索引。
返回值：
设置成功返回TRUE ，失败返回 FALSE。
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
函数名称：SetTargetPath
函数功能：设置快捷方式文件的目标文件路径。 Sets the location (path and index) of the icon for a Shell link object.
参数列表：
WCHAR * targetPath:目标文件路径字符串。
返回值：
设置成功返回TRUE ，失败返回 FALSE。
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
函数名称：SetRelativePath
函数功能：设置快捷方式文件的目标相对文件路径。 Sets the relative path to the Shell link object.
参数列表：
WCHAR * targetPath:目标文件相对路径字符串。
返回值：
设置成功返回TRUE ，失败返回 FALSE。
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
函数名称：SetShowCmd
函数功能：设置打开快捷方式时使用的窗口的显示类型。 Sets the show command for a Shell link object. The show command sets the initial show state of the window.
参数列表：
 int showCmd :打开快捷方式时使用的窗口的显示类型。详细类型见GetShowCmd。
返回值：
设置成功返回TRUE ，失败返回 FALSE。
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
函数名称：SetWorkingDirectory
函数功能：设置快捷方式的工作目录路径。 Sets the show command for a Shell link object. The show command sets the initial show state of the window.
参数列表：
WCHAR * workingDir :快捷方式的工作目录路径字符串。
返回值：
设置成功返回TRUE ，失败返回 FALSE。
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
函数名称：TargetPathExists
函数功能：检查目标文件是否存在。
参数列表：

返回值：
存在返回TRUE ，否则返回 FALSE。
*************************************/
BOOL ShellLink::TargetPathExists()
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
函数名称：Resolve
函数功能：解析快捷方式文件。
参数列表：
HWND hwnd：需要显示更多信息时，这个窗口句柄用来作为父窗口。
DWORD fFlags ：以下样式。

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
 
返回值：
快捷方式有效返回TRUE ，否则返回 FALSE。
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