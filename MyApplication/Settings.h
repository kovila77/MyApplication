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

struct MAIN_DATA_STRUCT
{
	const char* NAME_MY_DLL = "WWI.dll";
	const char* NAME_MY_EVENT = "WM_UPDATEDATA";

	int TYPE_OF_IO = 1; //TPStream
	const wchar_t* fname = _T("Param.dat");

	const TCHAR* szWinClass = _T("Win32SampleApp");
	const TCHAR* szWinName = _T("Win32SampleWindow");
	const TCHAR* szName = TEXT("MyFileMappingObject");
	HWND hwnd;
	HPEN lPen;
	HBRUSH hBrush;
	HBRUSH hBrushEll;
	int xElipse = -1;
	int yElipse = -1;
	int WM_UPDATEDATA = 0;

	HANDLE hMapFile;
	LPCTSTR pBuf;

	struct loadData {
		int N = 3;
		int szXWNDCreated = 320;
		int szYWNDCreated = 240;
		COLORREF colorBack = RGB(0, 0, 255);
		COLORREF colorLine = RGB(255, 0, 0);
		int countIcon = 0;
		int RCountIcon = 0;
		int* lenIcon;
		char** nameIcons;
	} DataF;

	struct tabaleHelp {
		bool** haveEll;
		int** TypeEll;
	} ellHelp;
};