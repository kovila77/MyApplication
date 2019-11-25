#pragma once
#include "MyApplication.h"

#ifndef HOTKEY__SHIFT_C__NOTEPAD
#define HOTKEY__SHIFT_C__NOTEPAD 15
#endif // !HOTKEY__SHIFT_C__NOTEPAD
#ifndef HOTKEY__RETURN__CHANGE_COLOR
#define HOTKEY__RETURN__CHANGE_COLOR 16
#endif // !HOTKEY__RETURN__CHANGE_COLOR
#ifndef HOTKEY__ESC__EXIT
#define HOTKEY__ESC__EXIT 17
#endif // !HOTKEY__ESC__EXIT
#ifndef HOTKEY__CONTROL_Q__EXIT
#define HOTKEY__CONTROL_Q__EXIT 18
#endif // !HOTKEY__CONTROL_Q__EXIT

struct img {
	HBITMAP bm;
	int width;
	int height;
};

struct MAIN_DATA_STRUCT
{
	bool imFirst = false;
	bool* haveCross;
	bool* haveZero;
	bool imACross = false;
	bool* lastWasCross;

	const char* NAME_MY_DLL = "WWI.dll";
	const char* NAME_EXIT_EVENT = "WM_UPDATEDATA";
	//const char* NAME_MY_EVENT = "WM_UPDATEDATA";

	bool workThread = true;
	HANDLE SemCanExit;

	int TYPE_OF_IO = 1; //TPStream
	const wchar_t* fname = _T("Param.dat");

	const TCHAR* szWinClass = _T("Win32SampleApp");
	const TCHAR* szWinNameCross = _T("Cross Window");
	const TCHAR* szWinNameZero = _T("Zero Window");
	const TCHAR* szName = TEXT("MyFileMappingObject");
	HWND hwnd;
	HPEN lPen;
	HBRUSH hBrush;
	HBRUSH hBrushEll;
	int xElipse = -1;
	int yElipse = -1;
	//int WM_UPDATEDATA = 0;
	int WM_EXIT = 0;
	int* countClick;

	HANDLE hMapFile;
	LPCTSTR pBuf;

	struct loadData {
		int N = 3;
		int szXWNDCreated = 320;
		int szYWNDCreated = 240;
		COLORREF colorBack = RGB(0, 0, 255);
		//COLORREF colorLine = RGB(255, 0, 0);
		COLORREF colorLine = RGB(0, 0, 0);
		int countIcon = 0;
		int RCountIcon = 0;
		int* lenIcon;
		char** nameIcons;
	} DataF;

	struct tabaleHelp {
		bool** haveEll;
		int** TypeEll;
	} ellHelp;

	img* myImages;

	std::thread* ptrThread;

	HANDLE stopSem;
};