#include "StdAfx.h"
#include "TransparentWindow.h"

CTransparentWindow::CTransparentWindow(void)
{
	m_dLogPixElsx = 96.0;
	m_dLogPixElsy = 96.0;
	m_dScaleX = 1.0;
	m_dScaleY = 1.0;
	m_SourceConstantAlpha = 0;
    InitScaling();
	
}

CTransparentWindow::~CTransparentWindow(void)
{
}

void CTransparentWindow::InitScaling() {
#ifdef AUTO_ADAPTIVE_DISPLAY
	HDC screen = GetDC(0);
	m_dLogPixElsx = GetDeviceCaps(screen, LOGPIXELSX);
	m_dLogPixElsy = GetDeviceCaps(screen, LOGPIXELSY);
	m_dScaleX = m_dLogPixElsx / 96.0;
	m_dScaleY = m_dLogPixElsy / 96.0;
	ReleaseDC(0, screen);
#else
	m_dScaleX = 1.0;
	m_dScaleY = 1.0;
#endif

#ifdef AUTO_FIX_DISPLAY
   if (m_dScaleX > 1.0)
   {
	   m_dScaleX = dFixSize;
   }

   if (m_dScaleY > 1.0)
   {
	   m_dScaleY = dFixSize;
   }
#endif
}

void CTransparentWindow::DoLoadFile(LPCTSTR pFileName,HWND hWnd, BYTE SourceConstantAlpha/*= 255*/)
{
	CWaitCursor wait;

	/*HBITMAP hBitmap;*/
	/*LPBYTE pBitmapBits;*/
	LPVOID pImg= NULL;
	CImage img;
	img.Load(pFileName);

	// first try to load with paintlib....
	// 	if (!::ImgNewDIBFromFile(m_pImgDecoder, pFileName, &pImg))
	// 	{
	// 		::ImgGetHandle(pImg, &hBitmap, (LPVOID *)&pBitmapBits);
	// 
	// 		// get the bitmap info
	// 		BITMAP bmpInfo;
	// 		::GetObject(hBitmap, sizeof(BITMAP), &bmpInfo);
	// 
	// 		if (bmpInfo.bmBitsPixel != 32)
	// 		{
	// 			::ImgDeleteDIBSection(pImg);
	// 			::AfxMessageBox(IDS_ERRORLOADING);
	// 			return;
	// 		}
	// 	}
	// 	else // ops... paitlib fail to load file... maybe is an PSP file?
	// 	{
	// 		if (ERR_NOERROR != ::LoadPSPFileToDIB(pFileName, hBitmap, (LPVOID *)&pBitmapBits))
	// 		{
	// 			::AfxMessageBox(IDS_ERRORLOADING);
	// 			return;
	// 		}
	// 	}
	// 
	// 	::ImgDeleteDIBSection(m_pImg);
	// 	m_pImg= pImg;

	if (img.IsNull())
	{
		return ;
	}
	MakeTranPngImage(img);
	m_bmpDialog.DeleteObject();
	m_bmpDialog.Attach(img.Detach());


	// before using the UpdateLayeredWindow we have to 
	// pre-multiply the RGB channels with the alpha channel
	//::PreMultiplyRGBChannels(m_bmpDialog, pBitmapBits);

	// now update the dummy dialog....
	
	DoUpdateDummyDialog(hWnd, m_bmpDialog, SourceConstantAlpha);
		
}

void CTransparentWindow::DoUpdateDummyDialog( BYTE SourceConstantAlpha /*= 255*/)
{
	return DoUpdateDummyDialog(m_hWnd,m_bmpDialog,SourceConstantAlpha);
}

void CTransparentWindow::DoUpdateDummyDialog(HWND hWnd, BYTE SourceConstantAlpha /*= 255*/)
{
   return DoUpdateDummyDialog(hWnd,m_bmpDialog,SourceConstantAlpha);
}

void CTransparentWindow::DoUpdateDummyDialog(HWND hWnd, CBitmap &bmp, BYTE SourceConstantAlpha/*= 255*/)
{
	// make sure the window has the WS_EX_LAYERED style
	//ModifyStyleEx(hWnd,0, WS_EX_LAYERED);


	// ok... into the per-pixel-alpha bendling....
	
	CDC dcScreen;
	CDC dcMemory;
	// Create/setup the DC's
    if (!IsWindow(hWnd))
    {
		return ;
    }

	dcScreen.Attach(::GetDC(NULL));
	dcMemory.CreateCompatibleDC(&dcScreen);

	CBitmap *pOldBitmap= dcMemory.SelectObject(&bmp);


	// get the bitmap dimensions
	BITMAP bmpInfo;
	bmp.GetBitmap(&bmpInfo);

	// get the window rectangule (we are only interested in the top left position)
	CRect rectDlg;
	::GetWindowRect(hWnd,rectDlg);
	//GetWindowRect(&rectDlg);
	//rectDlg.top += 20;

	// calculate the new window position/size based on the bitmap size
	CPoint ptWindowScreenPosition(rectDlg.TopLeft());
	CSize szWindow(bmpInfo.bmWidth, bmpInfo.bmHeight);

// 	rectDlg.bottom = rectDlg.top + bmpInfo.bmHeight;
// 	rectDlg.right = rectDlg.left + bmpInfo.bmWidth;
// 	wnd.MoveWindow(&rectDlg);

	// Perform the alpha blend

	// setup the blend function
	BLENDFUNCTION blendPixelFunction= { AC_SRC_OVER, 0, SourceConstantAlpha, AC_SRC_ALPHA };
	CPoint ptSrc(0,0); // start point of the copy from dcMemory to dcScreen

	// perform the alpha blend
	BOOL bRet= ::UpdateLayeredWindow(hWnd, dcScreen, &ptWindowScreenPosition, &szWindow, dcMemory,
		&ptSrc, 0, &blendPixelFunction, ULW_ALPHA);

	ASSERT(bRet); // ops... something bad just occured....
    m_SourceConstantAlpha = SourceConstantAlpha;
	// clean up
	dcMemory.SelectObject(pOldBitmap);

}

void  CTransparentWindow::MakeTranPngImage( CImage & image )
{
	if(!image.IsNull() && image.GetBPP() == 32)       
	{                     
		int maxY=image.GetHeight();           
		int maxX=image.GetWidth();           
		//		byte r,g,b;
		//int avg;    
		//		WORD dwHue, dwLum, dwSat;
		for(int x=0;x<maxX;x++)           
		{                
			for(int y=0;y<maxY;y++)                
			{                     

				UCHAR *cr = (UCHAR*) image.GetPixelAddress(x,y);
				cr[0] = cr[0]*cr[3] / 255;
				cr[1] = cr[1]*cr[3] / 255;
				cr[2] = cr[2]*cr[3] / 255;     
			}  
		}    
	}
}

void CTransparentWindow::SetWindowHandle(HWND hWnd)
{
	if (IsWindow(hWnd))
	{
		m_hWnd = hWnd;
	}
}

//设置窗体透明度
int CTransparentWindow::SetWindowTrans(HWND hWnd, COLORREF crkey, BYTE bAlpha, DWORD dwFlags)
{
	//HWND hWnd = GetSafeHwnd(); 
	 LONG lWindowLong = GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED; 
	 ::SetWindowLong(hWnd, GWL_EXSTYLE, lWindowLong); 
	HINSTANCE hInst=LoadLibrary(_T("User32.DLL"));//显示加载DLL
	if(hInst)
	{
		typedef BOOL (WINAPI*MYFUNC)(HWND,COLORREF,BYTE,DWORD);
		MYFUNC fun=NULL;
		//取得SetLayeredWindowAttributes函数指针,该函数控制透明度
		fun=(MYFUNC)GetProcAddress(hInst,"SetLayeredWindowAttributes");
		if(fun)
			fun(hWnd,crkey, bAlpha, dwFlags);
		FreeLibrary(hInst);
	}
	//::SetLayeredWindowAttributes( crkey, bAlpha, dwFlags); 
	//::RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN); 

	return GetLastError();
}

void CTransparentWindow::InitWindowStyle(HWND hWnd,UINT nWidth /*= 786*/,UINT nHeight/* = 556*/,UINT nRectRgn /*= 5*/,UINT nLeftMargin /*= 0*/,UINT nTopMargin /*= 0*/)
{
	if (!IsWindow(hWnd))
	{
		return ;
	}

  	DWORD dwStyle = GetWindowLong(hWnd,GWL_STYLE);//获取旧样式
  	DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE| WS_SYSMENU |WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
  	dwNewStyle&=dwStyle;//按位与将旧样式去掉
  	SetWindowLong(hWnd,GWL_STYLE,dwNewStyle);//设置成新的样式
  	DWORD dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);//获取旧扩展样式
  	DWORD dwNewExStyle = WS_EX_LEFT |WS_EX_LTRREADING |WS_EX_RIGHTSCROLLBAR|WS_EX_ACCEPTFILES /*| WS_EX_LAYERED*/;
  	dwNewExStyle&=dwExStyle;//按位与将旧扩展样式去掉
  	SetWindowLong(hWnd,GWL_EXSTYLE,dwNewExStyle);//设置新的扩展样式

	CRect rect;
	GetWindowRect(hWnd,&rect);
	SetWindowPos(hWnd,HWND_NOTOPMOST,rect.left,rect.top,nWidth,nHeight,/*SWP_NOZORDER|*/SWP_NOMOVE/*|SWP_FRAMECHANGED*/);//告诉windows：我的样式改变了，窗口位置和大小保持原来不变！  

	//圆角处理边角
	HRGN hrgn;
	/*hrgn =CreateRoundRectRgn(nLeftMargin,nTopMargin,nWidth,nHeight,nRectRgn,nRectRgn);*/
	hrgn =CreateRoundRectRgn(nLeftMargin,nTopMargin,nWidth,nHeight,nRectRgn,nRectRgn);
	SetWindowRgn(hWnd,hrgn,TRUE);
}

COLORREF CTransparentWindow::AlphaTrans(COLORREF crSour,COLORREF crDes,int alpha)
{
	int R,G,B;
	R=(alpha*GetRValue(crSour)+(255-alpha)*GetRValue(crDes))/256; 

	G=(alpha*GetGValue(crSour)+(255-alpha)*GetGValue(crDes))/256; 

	B=(alpha*GetBValue(crSour)+(255-alpha)*GetBValue(crDes))/256;
	return RGB(R,G,B);
}