﻿#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <windowsx.h>
#include <random>
#include <fstream>
#include <sstream>
#include <string>
#define COLOR_ELLIPS RGB(255, 0, 0)
#define COLOR_RAND RGB(rand() % 255, rand() % 255, rand() % 255)
#define TPSTREAM 1
#define TMAPPING 2
#define TWINAPI 3
#define TSTREAMIO 4
#define NAME_MY_DLL "WWI.dll"

int TYPE_OF_IO = TPSTREAM;
const wchar_t* fname = _T("Param.dat");

const int HOTKEY__SHIFT_C__NOTEPAD = 15;
const int HOTKEY__RETURN__CHANGE_COLOR = 16;
const int HOTKEY__ESC__EXIT = 17;
const int HOTKEY__CONTROL_Q__EXIT = 18;
const char STANDART_ICON_NAME[] = "icon.ico";
const TCHAR szWinClass[] = _T("Win32SampleApp");
const TCHAR szWinName[] = _T("Win32SampleWindow");
HWND hwnd;
HPEN lPen;
HBRUSH hBrush;
HBRUSH hBrushEll;
int xElipse = -1;
int yElipse = -1;

struct loadData {
	int N = 3;
	int szXWNDCreated = 320;
	int szYWNDCreated = 240;
	COLORREF colorBack = RGB(0, 0, 255);
	COLORREF colorLine = RGB(255, 0, 0);
	int lenIcon;
	char* nameIcon;
} DataF;
bool** haveEll;

struct myImg {
	unsigned char* buff;
	int width;
	int height;
} myImage;


bool ReadParamFdoing() {
	FILE* stream;
	_wfopen_s(&stream, fname, _T("r"));
	if (stream == NULL) {
		std::cout << "Error Open file" << std::endl;
		return false;
	}

	fseek(stream, 0, SEEK_END);
	int fileSize = ftell(stream);
	fseek(stream, 0, SEEK_SET);

	char* buff = new char[fileSize + 1];
	ZeroMemory(buff, fileSize + 1);
	//buff[fileSize] = '\0';
	fread(buff, sizeof(char), fileSize, stream);

	std::stringstream sstr;

	sstr << buff;
	sstr >> DataF.N >> DataF.szXWNDCreated >> DataF.szYWNDCreated >> DataF.colorBack
		>> DataF.colorLine >> DataF.lenIcon;
	DataF.nameIcon = new char[DataF.lenIcon + 1];
	ZeroMemory(DataF.nameIcon, DataF.lenIcon + 1);
	sstr >> DataF.nameIcon;

	delete[] buff;
	fclose(stream);
	return 1;
}

bool ReadParamWinAPI() {
	HANDLE hFile = CreateFileW(
		fname,
		GENERIC_READ,// Changed
		0,
		NULL,
		OPEN_EXISTING, // Changed
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hFile == INVALID_HANDLE_VALUE) {
		std::cout << "fileMappingCreate - CreateFile failed, fname = " << std::endl;
		return false;
	}

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == INVALID_FILE_SIZE) {
		std::cerr << "fileMappingCreate - GetFileSize failed " << std::endl;
		CloseHandle(hFile);
		return NULL;
	}

	char* buff = new char[dwFileSize + 1];
	ZeroMemory(buff, dwFileSize + 1);
	//buff[dwFileSize] = '\0';
	std::stringstream sstr;

	if (!ReadFile(hFile, buff, dwFileSize, NULL, NULL)) {
		std::cout << "ReadFile error" << std::endl;
		CloseHandle(hFile);
		return false;
	}

	sstr << buff;
	sstr >> DataF.N >> DataF.szXWNDCreated >> DataF.szYWNDCreated >> DataF.colorBack
		>> DataF.colorLine >> DataF.lenIcon;
	DataF.nameIcon = new char[DataF.lenIcon + 1];
	ZeroMemory(DataF.nameIcon, DataF.lenIcon + 1);
	sstr >> DataF.nameIcon;

	CloseHandle(hFile);
	delete[]buff;
	return 1;
}

bool ReadParamMapping() {
	HANDLE hFile = CreateFileW(
		fname,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hFile == INVALID_HANDLE_VALUE) {
		std::cout << "fileMappingCreate - CreateFile failed, fname = "
			<< fname << std::endl;
		return false;
	}

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == INVALID_FILE_SIZE) {
		std::cout << "fileMappingCreate - GetFileSize failed, fname = "
			<< fname << std::endl;
		CloseHandle(hFile);
		return false;
	}

	HANDLE hMapping = CreateFileMappingW(
		hFile,
		NULL,
		PAGE_READONLY,
		0,
		0,
		NULL
	);
	if (hMapping == NULL) {
		std::cout << "fileMappingCreate - CreateFileMapping failed, fname = "
			<< fname << std::endl;
		CloseHandle(hFile);
		return false;
	}

	LPVOID hViewOfFile = MapViewOfFile(
		hMapping,
		FILE_MAP_READ,
		0,
		0,
		dwFileSize
	);
	char* dataPtr = (char*)hViewOfFile;
	if (dataPtr == NULL) {
		std::cout << "fileMappingCreate - MapViewOfFile failed, fname = "
			<< fname << std::endl;
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return false;
	}

	std::stringstream sstr;
	sstr << dataPtr;

	sstr >> DataF.N >> DataF.szXWNDCreated >> DataF.szYWNDCreated >> DataF.colorBack
		>> DataF.colorLine >> DataF.lenIcon;
	DataF.nameIcon = new char[DataF.lenIcon + 1];
	ZeroMemory(DataF.nameIcon, DataF.lenIcon + 1);
	sstr >> DataF.nameIcon;

	UnmapViewOfFile(hViewOfFile);
	CloseHandle(hMapping);
	CloseHandle(hFile);
	return 1;
}

bool ReadParamStream() {
	std::ifstream fin(fname);
	if (!fin.is_open()) return false;
	fin >> DataF.N >> DataF.szXWNDCreated >> DataF.szYWNDCreated >> DataF.colorBack
		>> DataF.colorLine >> DataF.lenIcon;
	DataF.nameIcon = new char[DataF.lenIcon + 1];
	ZeroMemory(DataF.nameIcon, DataF.lenIcon + 1);
	fin >> DataF.nameIcon;
	fin.close();
	return 1;
}

bool ReadParam() {
	switch (TYPE_OF_IO)
	{
	case TMAPPING: {
		return ReadParamMapping();
	}
	case TWINAPI: {
		return ReadParamWinAPI();
	}
	case TSTREAMIO: {
		return ReadParamFdoing();
	}
	default:
		return ReadParamStream();
	}
}


void WriteParamFdoing() {
	FILE* stream;
	_wfopen_s(&stream, fname, _T("w"));
	if (stream == NULL) {
		std::cout << "Error Open file" << std::endl;
		return;
	}

	std::stringstream sstr;
	sstr << DataF.N << std::endl << DataF.szXWNDCreated
		<< std::endl << DataF.szYWNDCreated
		<< std::endl << DataF.colorBack
		<< std::endl << DataF.colorLine
		<< std::endl << strlen(DataF.nameIcon)
		<< std::endl << DataF.nameIcon;
	std::string strStream = sstr.str();

	fwrite(strStream.c_str(), 1, strStream.length(), stream);

	fclose(stream);
}

void WriteParamWinAPI() {
	std::stringstream sstr;
	sstr << DataF.N << std::endl << DataF.szXWNDCreated
		<< std::endl << DataF.szYWNDCreated
		<< std::endl << DataF.colorBack
		<< std::endl << DataF.colorLine
		<< std::endl << strlen(DataF.nameIcon)
		<< std::endl << DataF.nameIcon;
	std::string strStream = sstr.str();

	HANDLE hFile = CreateFileW(
		fname,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hFile == INVALID_HANDLE_VALUE) {
		std::cout << "fileMappingCreate - CreateFile failed, fname = " << std::endl;
		return;
	}

	WriteFile(hFile, strStream.c_str(), strStream.length(), NULL, NULL);

	CloseHandle(hFile);
}

void WriteParamMapping() {
	std::stringstream sstr;
	sstr << DataF.N << std::endl << DataF.szXWNDCreated
		<< std::endl << DataF.szYWNDCreated
		<< std::endl << DataF.colorBack
		<< std::endl << DataF.colorLine
		<< std::endl << strlen(DataF.nameIcon)
		<< std::endl << DataF.nameIcon;
	std::string strStream = sstr.str();

	HANDLE hFile = CreateFileW(
		fname,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hFile == INVALID_HANDLE_VALUE) {
		std::cout << "fileMappingCreate - CreateFile failed, fname = "
			<< fname << std::endl;
		return;
	}

	HANDLE hMapping = CreateFileMappingW(
		hFile,
		NULL,
		PAGE_READWRITE, // Changed
		0,
		strStream.length(),
		NULL
	);
	if (hMapping == NULL) {
		std::cout << "fileMappingCreate - CreateFileMapping failed, fname = "
			<< fname << std::endl;
		CloseHandle(hFile);
		return;
	}

	LPVOID hViewOfFile = MapViewOfFile(
		hMapping,
		FILE_MAP_WRITE,
		0,
		0,
		strStream.length()
	);
	char* dataPtr = (char*)hViewOfFile;
	if (dataPtr == NULL) {
		std::cout << "fileMappingCreate - MapViewOfFile failed, fname = "
			<< fname << std::endl;
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return;
	}

	strcpy_s((char*)hViewOfFile, sstr.str().length() + 1, sstr.str().c_str());

	UnmapViewOfFile(hViewOfFile);
	CloseHandle(hMapping);
	CloseHandle(hFile);
}

void WriteParamStream() {
	std::ofstream fout(fname);
	fout << DataF.N << std::endl << DataF.szXWNDCreated
		<< std::endl << DataF.szYWNDCreated
		<< std::endl << DataF.colorBack
		<< std::endl << DataF.colorLine
		<< std::endl << strlen(DataF.nameIcon)
		<< std::endl << DataF.nameIcon;
	fout.close();
}

void WriteParam() {
	switch (TYPE_OF_IO)
	{
	case TWINAPI: {
		WriteParamWinAPI();
		break;
	}
	case TMAPPING: {
		WriteParamMapping();
		break;
	}
	case TSTREAMIO:
		WriteParamFdoing();
		break;
	default:
		WriteParamStream();
		break;
	}
}


void RunNotepad(void)
{
	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;

	ZeroMemory(&sInfo, sizeof(STARTUPINFO));

	CreateProcess(_T("C:\\Windows\\Notepad.exe"),
		NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
	CloseHandle(pInfo.hProcess);
	CloseHandle(pInfo.hThread);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY: {
		//Актуализация размеров окна
		RECT tmpWR = { 0 };
		GetWindowRect(hwnd, &tmpWR);
		DataF.szXWNDCreated = tmpWR.right - tmpWR.left;
		DataF.szYWNDCreated = tmpWR.bottom - tmpWR.top;

		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &ps.rcPaint);
		HBITMAP hBitmap = CreateBitmap(10, 10, 1, 32, (myImage.buff));

		HGDIOBJ tempPen = SelectObject(hdc, (HGDIOBJ)lPen);

		double stepX = ps.rcPaint.right / (double)(DataF.N + 1);
		double positionX = stepX;
		double stepY = ps.rcPaint.bottom / (double)(DataF.N + 1);
		double positionY = stepY;
		for (int i = 0; i < DataF.N; i++) {
			MoveToEx(hdc, positionX, 0, NULL);
			LineTo(hdc, positionX, ps.rcPaint.bottom);
			MoveToEx(hdc, 0, positionY, NULL);
			LineTo(hdc, ps.rcPaint.right, positionY);
			positionX += stepX;
			positionY += stepY;
		}

		if (xElipse > 0 && stepX != 0 && stepY != 0) {
			haveEll[(int)(xElipse / stepX)][(int)(yElipse / stepY)] = !haveEll[(int)(xElipse / stepX)][(int)(yElipse / stepY)];
			xElipse = yElipse = -1;
		}

		SelectObject(hdc, hBrushEll);
		for (int i = 0; i < DataF.N + 1; i++)
			for (int j = 0; j < DataF.N + 1; j++)
				if (haveEll[i][j] && stepX != 0 && stepY != 0) {
					Ellipse(hdc, stepX * i, stepY * j, stepX * i + stepX, stepY * j + stepY);
				}

		//PatBlt(hdc, 0, 0, 10, 10, WHITENESS);
		//PatBlt(hdc, xDest, yDest, xWidth, yHeight, dwROP);
		//HDC hdcTMP = CreateDC(pszDriver, pszDevice, pszOutput, pData);
		HDC hdcTMP = CreateCompatibleDC(hdc);
		SelectObject(hdcTMP, hBitmap);
		//BitBlt(hdc, 0, 0, 100, 100, hdcTMP, 0, 0, SRCCOPY);
		TransparentBlt(hdc, 0, 0, 10, 10, hdcTMP, 0, 0, 10, 10, RGB(0, 0, 0));
		DeleteDC(hdcTMP);

		SelectObject(hdc, tempPen);
		EndPaint(hwnd, &ps);
		DeleteObject(hBitmap);
		break;
	}
	case WM_LBUTTONDOWN: {
		xElipse = GET_X_LPARAM(lParam);
		yElipse = GET_Y_LPARAM(lParam);
		InvalidateRect(hwnd, NULL, true);
		break;
	}
	case WM_HOTKEY: {
		switch (wParam) {
		case HOTKEY__ESC__EXIT: {
			DestroyWindow(hwnd);
			break;
		}
		case HOTKEY__CONTROL_Q__EXIT: {
			DestroyWindow(hwnd);
			break;
		}
		case HOTKEY__SHIFT_C__NOTEPAD: {
			RunNotepad();
			break;
		}
		case HOTKEY__RETURN__CHANGE_COLOR: {
			DataF.colorBack = COLOR_RAND;
			HBRUSH newhBrush = CreateSolidBrush(DataF.colorBack);
			SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG)newhBrush);
			DeleteObject(hBrush);
			hBrush = newhBrush;
			InvalidateRect(hwnd, NULL, true);
			break;
		}
		default:
			break;
		}
		break;
	}
	case WM_KILLFOCUS: {
		UnregisterHotKey(hwnd, HOTKEY__SHIFT_C__NOTEPAD);
		UnregisterHotKey(hwnd, HOTKEY__RETURN__CHANGE_COLOR);
		UnregisterHotKey(hwnd, HOTKEY__ESC__EXIT);
		UnregisterHotKey(hwnd, HOTKEY__CONTROL_Q__EXIT);
		break;
	}
	case WM_SETFOCUS: {
		RegisterHotKey(hwnd, HOTKEY__SHIFT_C__NOTEPAD, MOD_SHIFT | MOD_NOREPEAT, 'C');
		RegisterHotKey(hwnd, HOTKEY__RETURN__CHANGE_COLOR, NULL, VK_RETURN);
		RegisterHotKey(hwnd, HOTKEY__ESC__EXIT, MOD_NOREPEAT, VK_ESCAPE);
		RegisterHotKey(hwnd, HOTKEY__CONTROL_Q__EXIT, MOD_CONTROL | MOD_NOREPEAT, 'Q');
		break;
	}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

void setStandartIcon() {
	if (DataF.nameIcon != NULL) delete[] DataF.nameIcon;
	DataF.lenIcon = strlen(STANDART_ICON_NAME);
	DataF.nameIcon = new char[DataF.lenIcon + 1];
	for (int i = 0; i < DataF.lenIcon + 1; i++) {
		DataF.nameIcon[i] = STANDART_ICON_NAME[i];
	}
}

void setTypeIO(const char* arg) {
	if (strcmp(arg, "-tp") == 0) {
		TYPE_OF_IO = TPSTREAM;
		std::cout << "IO set \"standart c++ stream\"" << std::endl;
	}
	else
		if (strcmp(arg, "-tm") == 0) {
			TYPE_OF_IO = TMAPPING;
			std::cout << "IO set \"memory mapping\"" << std::endl;
		}
		else
			if (strcmp(arg, "-tw") == 0) {
				TYPE_OF_IO = TWINAPI;
				std::cout << "IO set \"WIN API method\"" << std::endl;
			}
			else if (strcmp(arg, "-ts") == 0) {
				std::cout << "IO set \"c method\"" << std::endl;
			}
}

void loadImage() {
	HMODULE hLib;
	hLib = LoadLibraryA(NAME_MY_DLL);
	if (hLib == NULL) return;

	unsigned char* (*load_image)(const char* filename, int& width, int& height);
	(FARPROC&)load_image = GetProcAddress(hLib, "load_image");
	myImage.buff = (*load_image)("221.jpg", myImage.width, myImage.height); 

	FreeLibrary(hLib);
}

int main(int argc, char* argv[])
{
	if (argc > 1)
		setTypeIO(argv[1]);

	if (!ReadParam()) setStandartIcon();

	loadImage();

	BOOL bMessageOk;
	MSG message;
	WNDCLASSEX wincl = { 0 };
	HINSTANCE hThisInstance = GetModuleHandle(NULL);

	hBrush = CreateSolidBrush(DataF.colorBack);
	hBrushEll = CreateSolidBrush(COLOR_ELLIPS);
	lPen = CreatePen(PS_SOLID, 1, DataF.colorLine);

	haveEll = new bool* [DataF.N + 1];
	for (int i = 0; i < DataF.N + 1; i++) {
		haveEll[i] = new bool[DataF.N + 1];
	}
	for (int i = 0; i < DataF.N + 1; i++) {
		for (int j = 0; j < DataF.N + 1; j++) {
			haveEll[i][j] = false;
		}
	}

	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = szWinClass;
	wincl.style = CS_HREDRAW | CS_VREDRAW;
	wincl.lpfnWndProc = WindowProcedure;
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.cbSize = sizeof(WNDCLASSEX);
	wincl.hbrBackground = hBrush;

	if (!RegisterClassEx(&wincl))
		return 0;

	hwnd = CreateWindowEx(
		NULL,
		szWinClass,          /* Classname */
		szWinName,       /* Title Text */
		WS_OVERLAPPEDWINDOW, /* default window */
		CW_USEDEFAULT,       /* Windows decides the position */
		CW_USEDEFAULT,       /* where the window ends up on the screen */
		DataF.szXWNDCreated,                 /* The programs width */
		DataF.szYWNDCreated,                 /* and height in pixels */
		HWND_DESKTOP,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hThisInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
	);

	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, SW_SHOW);

	while ((bMessageOk = GetMessage(&message, NULL, 0, 0)) != 0) {
		if (bMessageOk == -1) {
			puts("Suddenly, GetMessage failed! You can call GetLastError() to see what happend");
			break;
		}

		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	//WriteParam();
	DestroyWindow(hwnd);
	WriteParam();
	for (int i = 0; i < DataF.N + 1; i++) {
		delete[]haveEll[i];
	}
	delete[]haveEll;
	delete[]DataF.nameIcon;
	DeleteObject(hBrush);
	DeleteObject(hBrushEll);
	UnregisterClass(szWinClass, hThisInstance);
	delete myImage.buff;

	return 0;
}