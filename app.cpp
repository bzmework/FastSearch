#include <windows.h>
#include <time.h>
#include "lib/lib.h"

HINSTANCE g_hInstance;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND m_hWnd;

// CPU
Win32Cpu cpuusage;

// Win32文件传统搜索
Win32FileSearch fs;
void CALLBACK fs_OnSearch(Win32FileSearch::FileSearchInfo searchInfo);

// Win32文件快速搜索
Win32FileJournals fj;
void CALLBACK fj_OnLoadProgress(TString* deviceName, INT deviceIndex, INT deviceCount, DWORD fileCount, DOUBLE elapsedTime);// 加载进度事件
void CALLBACK fj_OnLoadComplete(INT deviceCount, DWORD fileCount, DOUBLE elapsedTime);// 加载完成事件
void CALLBACK fj_OnQueryProgress(UsnList* fileList, TString* deviceName, INT deviceIndex, INT deviceCount, DWORD fileCount, DOUBLE elapsedTime); // 查询进度事件
void CALLBACK fj_OnQueryComplete(UsnList* fileList, INT deviceCount, DWORD fileCount, DOUBLE elapsedTime); // 查询完成事件

LONG left = 10;
LONG top = 40;
LONG width = 300;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("HelloWin");
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass;

	g_hInstance = hInstance;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(200, 210, 240));//GetStockObject(WHITE_BRUSH) ;//RGB(38,39,41)
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass)) return 0;

	hwnd = CreateWindow (szAppName,                  // window class name
		TEXT ("鼠标中键传统搜索，鼠标右击快速搜索"), // window caption
		WS_OVERLAPPEDWINDOW,        // window style
		CW_USEDEFAULT,              // initial x position
		CW_USEDEFAULT,              // initial y position
		800,						// initial x size
		600,						// initial y size
		NULL,                       // parent window handle
		NULL,                       // window menu handle
		hInstance,                  // program instance handle
		NULL) ;                     // creation parameters
     
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;
     
     while (GetMessage (&msg, NULL, 0, 0))
     {
		TranslateMessage(&msg) ;
		DispatchMessage(&msg) ;
     }
     return msg.wParam ;
}


// 传统的文件搜索
void CALLBACK fs_OnSearch(Win32FileSearch::FileSearchInfo searchInfo)
{
	TCHAR file[300] = {0};

	if(searchInfo.bIsFinded)
	{
		RECT rc;

		HDC hdc = GetDC(m_hWnd);

		rc.left = left;
		rc.top = top;
		rc.bottom = rc.top + 20;
		rc.right = rc.left + width;
		
		format(file, _T("已找到:%d"), searchInfo.dwFindedCount);
		Win32GDI::DrawArea(hdc, rc, RGB(255,234,23));
		Win32GDI::DrawAText(hdc,file,rc);
		ReleaseDC(m_hWnd, hdc);
	}
	
	if(searchInfo.bIsFinish)
	{
		RECT rc;

		HDC hdc = GetDC(m_hWnd);
		rc.left = left;
		rc.top = top;
		rc.bottom = rc.top + 20;
		rc.right = rc.left + width;
		
		format(file,_T("搜索%d.找到:%d"), searchInfo.dwFindingCount, searchInfo.dwFindedCount);
		Win32GDI::DrawArea(hdc, rc, RGB(255,234,23));
		Win32GDI::DrawAText(hdc,file,rc);
		ReleaseDC(m_hWnd, hdc);
	}
}

// 加载进度事件
void CALLBACK fj_OnLoadProgress(TString* deviceName, INT deviceIndex, INT deviceCount, DWORD fileCount, DOUBLE elapsedTime)
{
	TCHAR file[300] = { 0 };
	RECT rc;

	top += 40;
	HDC hdc = GetDC(m_hWnd);
	rc.left = left;
	rc.top = top;
	rc.bottom = rc.top + 20;
	rc.right = rc.left + width;

	format(file, _T("扫描:%s 找到:%d 消耗:%f 秒"), deviceName->toString(), fileCount, elapsedTime / 1000);
	Win32GDI::DrawArea(hdc, rc, RGB(255, 234, 23));
	Win32GDI::DrawAText(hdc, file, rc);
	ReleaseDC(m_hWnd, hdc);
}

// 加载完成事件
void CALLBACK fj_OnLoadComplete(INT deviceCount, DWORD fileCount, DOUBLE elapsedTime)
{
	TCHAR file[300] = { 0 };
	RECT rc;

	top += 40;
	HDC hdc = GetDC(m_hWnd);
	rc.left = left;
	rc.top = top;
	rc.bottom = rc.top + 20;
	rc.right = rc.left + width;

	format(file, _T("扫描完成，找到:%d, 总计消耗:%f 秒"), fileCount, elapsedTime / 1000);
	Win32GDI::DrawArea(hdc, rc, RGB(255, 34, 23));
	Win32GDI::DrawAText(hdc, file, rc);
	ReleaseDC(m_hWnd, hdc);

	// 扫描完成，进行查询
	top = 40;
	fj.Query("*mp3*", fj_OnQueryProgress, fj_OnQueryComplete);
}

// 查询进度事件
void CALLBACK fj_OnQueryProgress(UsnList* fileList, TString* deviceName, INT deviceIndex, INT deviceCount, DWORD fileCount, DOUBLE elapsedTime)
{
	TCHAR file[300] = { 0 };
	RECT rc;

	top += 40;
	HDC hdc = GetDC(m_hWnd);
	rc.left = left + width + 20;
	rc.top = top;
	rc.bottom = rc.top + 20;
	rc.right = rc.left + width;

	format(file, _T("查找:%s 找到:%d 消耗:%f 秒"), deviceName->toString(), fileCount, elapsedTime / 1000);
	Win32GDI::DrawArea(hdc, rc, RGB(255, 234, 23));
	Win32GDI::DrawAText(hdc, file, rc);
	ReleaseDC(m_hWnd, hdc);
}

// 查询完成事件
void CALLBACK fj_OnQueryComplete(UsnList* fileList, INT deviceCount, DWORD fileCount, DOUBLE elapsedTime)
{
	TCHAR file[300] = { 0 };
	RECT rc;

	top += 40;
	HDC hdc = GetDC(m_hWnd);
	rc.left = left + width + 20;
	rc.top = top;
	rc.bottom = rc.top + 20;
	rc.right = rc.left + width;

	format(file, _T("查找完成，找到:%d, 总计消耗:%f 秒"), fileCount, elapsedTime / 1000);
	Win32GDI::DrawArea(hdc, rc, RGB(255, 34, 23));
	Win32GDI::DrawAText(hdc, file, rc);
	ReleaseDC(m_hWnd, hdc);
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     HDC         hdc ;
     RECT        rect ;

	 switch (message)
	 {
	 case WM_CREATE:

		 m_hWnd = hwnd;
		 SetTimer(hwnd, 0, 800, NULL);
		 return 0;

	 case WM_SHOWWINDOW:
		 break;

	 case WM_MBUTTONUP:
	 {
		 // 传统的搜索测试
		 top = 40;
		 fs.SetSearchPath(_T("c:\\|d:\\|e:\\|f:\\|g:\\"));
		 fs.SetSearchPath(_T("d:\\"));
		 fs.SetSearchFilter(_T("*.wmv|*.mp3|*.wav|*.rmvb|*.rm|*.mid|*.avi"));
		 fs.SetSearchEvent(fs_OnSearch);
		 fs.Start();
		 return 0;
	 }
	 case WM_RBUTTONUP:
	 {
		 // 快速搜索测试
		 top = 40;
		 fj.Refresh(fj_OnLoadProgress, fj_OnLoadComplete);
		 return 0;
	 }

	 case WM_TIMER:
		 
		 hdc = GetDC(hwnd);
		 GetClientRect (hwnd, &rect);
		 rect.left = 10;
		 rect.top = 10;
		 rect.right = rect.left + 130;
		 rect.bottom = rect.top + 24;
		 cpuusage.hDC = hdc;
		 cpuusage.Rect = rect;
		 cpuusage.Draw();
		 ReleaseDC(hwnd,hdc);
		 return 0;

     case WM_DESTROY:
		 KillTimer(hwnd,0);
         PostQuitMessage(0) ;
         return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}