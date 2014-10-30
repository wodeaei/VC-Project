#pragma once
#include <atlimage.h>

const double dFixSize = 1.50;//����100ʱ��Ĭ�ϴ�С
const double dDefaultFixSize = 1.00;

#define SCALEX(argX) ((int) ((argX) * scaleX))
#define SCALEY(argY) ((int) ((argY) * scaleY))

class CTransparentWindow
{
private:
	double m_dLogPixElsx;
	double m_dLogPixElsy;
	double m_dScaleX;
	double m_dScaleY;
	CBitmap m_bmpDialog;
	HWND m_hWnd;
	BYTE m_SourceConstantAlpha;
private:
void InitScaling();
	
public:
	CTransparentWindow(void);
	~CTransparentWindow(void);
	//DPI�����Ϣ�Ļ�ȡ
	double GetSysLogPixElsx(){return m_dLogPixElsx;}
	double GetSysLogPixElsy(){return m_dLogPixElsy;}
	double GetScaleX(){return m_dScaleX;}
	double GetScaleY(){return m_dScaleY;}
	int GetIdealScaleX(int nCx){return ((int)((nCx) * m_dScaleX));}
	int GetIdealScaleY(int nCy){return ((int)((nCy) * m_dScaleY));}

	void SetWindowHandle(HWND hWnd);
	void DoUpdateDummyDialog(HWND hWnd, CBitmap &bmp, BYTE SourceConstantAlpha = 255);
	void DoLoadFile(LPCTSTR pFileName,HWND hWnd, BYTE SourceConstantAlpha = 255);
	void DoUpdateDummyDialog(HWND hWnd, BYTE SourceConstantAlpha = 255);
	void DoUpdateDummyDialog( BYTE SourceConstantAlpha = 255);
	BYTE GetConstantAlpha(){return m_SourceConstantAlpha;}

    static void MakeTranPngImage( CImage & image );
	/*
	LWA_ALPHA�����õĻ�,ͨ��bAlpha����͸����.
	LWA_COLORKEY�����õĻ�,��ָ����͸��������ɫΪcrKey,������ɫ��������ʾ.
	*/
	static int SetWindowTrans(HWND hWnd = NULL, COLORREF crkey = 0, BYTE bAlpha = 128, DWORD dwFlags = LWA_ALPHA);
	static void InitWindowStyle(HWND hWnd = NULL,UINT nWidth = 786,UINT nHeight = 556,UINT nRectRgn = 5,UINT nLeftMargin = 0,UINT nTopMargin = 0);
	static COLORREF AlphaTrans(COLORREF crSour,COLORREF crDes,int alpha);
};
