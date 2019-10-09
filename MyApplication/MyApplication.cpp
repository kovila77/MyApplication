#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <windowsx.h>
#include <random>
#include <fstream>
#include <sstream>
#include <string>
#define COLOR_ELLIPS RGB(255, 0, 0)
#define COLOR_RAND RGB(rand() % 255, rand() % 255, rand() % 255)
#define TYPE_STREAM 1
#define TYPE_MAPPING 2
#define TYPE_WINAPI 3
#define TYPE_FDOING 4
#define LENGTH_WAY_TO_ICON 512

const int TYPE_OF_IO = TYPE_FDOING;
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
	int szXWND = 320;
	int szYWND = 240;
	COLORREF colorBack = RGB(0, 0, 255);
	COLORREF colorLine = RGB(255, 0, 0);
	char nameIcon[LENGTH_WAY_TO_ICON];
} DataF;
bool** haveEll;


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
	fread(buff, 1, fileSize, stream);

	std::stringstream sstr;

	sstr << buff;
	sstr >> DataF.N >> DataF.szXWND >> DataF.szYWND >> DataF.colorBack
		>> DataF.colorLine >> DataF.nameIcon;

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

	char* buff = new char[dwFileSize+1];
	ZeroMemory(buff, dwFileSize+1);
	//buff[dwFileSize] = '\0';
	std::stringstream sstr;

	if (!ReadFile(hFile, buff, dwFileSize, NULL, NULL)) {
		std::cout << "ReadFile error" << std::endl;
		CloseHandle(hFile);
		return false;
	}

	sstr << buff;
	sstr >> DataF.N >> DataF.szXWND >> DataF.szYWND >> DataF.colorBack
		>> DataF.colorLine >> DataF.nameIcon;

	CloseHandle(hFile);
	delete []buff;
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

	sstr >> DataF.N >> DataF.szXWND >> DataF.szYWND >> DataF.colorBack
		>> DataF.colorLine >> DataF.nameIcon;

	UnmapViewOfFile(hViewOfFile);
	CloseHandle(hMapping);
	CloseHandle(hFile);
	return 1;
}

bool ReadParamStream() {
	std::ifstream fin(fname);
	if (fin >> DataF.N) {
		fin >> DataF.szXWND >> DataF.szYWND >> DataF.colorBack
			>> DataF.colorLine >> DataF.nameIcon;
		fin.close();
	}
	return 1;
}

bool ReadParam() {
	switch (TYPE_OF_IO)
	{
	case TYPE_MAPPING: {
		return ReadParamMapping();
	}
	case TYPE_WINAPI: {
		return ReadParamWinAPI();
	}
	case TYPE_FDOING: {
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
	sstr << DataF.N << std::endl << DataF.szXWND << std::endl << DataF.szYWND << std::endl << DataF.colorBack
		<< std::endl << DataF.colorLine << std::endl << DataF.nameIcon;
	std::string strStream = sstr.str();

	char* buff = new char[strStream.length() + 1];
	//buff[strStream.length() + 1] = '\0';
	strcpy_s(buff, strStream.length() + 1, strStream.c_str());

	fwrite(buff, 1, strStream.length(), stream);

	delete[] buff;
	fclose(stream);
}

void WriteParamWinAPI() {
	std::stringstream sstr;
	sstr << DataF.N << std::endl << DataF.szXWND << std::endl << DataF.szYWND << std::endl << DataF.colorBack
		<< std::endl << DataF.colorLine << std::endl << DataF.nameIcon;
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

	char* buff = new char[strStream.length() + 1];
	//buff[strStream.length() + 1] = '\0';
	strcpy_s(buff, strStream.length() + 1, strStream.c_str());

	WriteFile(hFile, buff, strStream.length() + 1, NULL, NULL);
	
	CloseHandle(hFile);
	delete[]buff;
}

void WriteParamMapping() {
	std::stringstream sstr;
	sstr << DataF.N << std::endl << DataF.szXWND << std::endl << DataF.szYWND << std::endl << DataF.colorBack
		<< std::endl << DataF.colorLine << std::endl << DataF.nameIcon;
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
	fout << DataF.N << '\n' << DataF.szXWND << '\n'
		<< DataF.szYWND << '\n' << DataF.colorBack << '\n'
		<< DataF.colorLine << '\n' << DataF.nameIcon;
	fout.close();
}

void WriteParam() {
	switch (TYPE_OF_IO)
	{
	case TYPE_WINAPI: {
		WriteParamWinAPI();
		break;
	}
	case TYPE_MAPPING: {
		WriteParamMapping();
		break;
	}
	case TYPE_FDOING:
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
		PostQuitMessage(0); 
		return 0;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &ps.rcPaint);

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

		SelectObject(hdc, tempPen);
		EndPaint(hwnd, &ps);
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
	for (int i = 0; i < strlen(STANDART_ICON_NAME); i++) {
		DataF.nameIcon[i] = STANDART_ICON_NAME[i];
	}
}

int main()
{
	if (!ReadParam()) setStandartIcon();

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
		DataF.szXWND,                 /* The programs width */
		DataF.szYWND,                 /* and height in pixels */
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

	WriteParam();
	DestroyWindow(hwnd);
	for (int i = 0; i < DataF.N + 1; i++) {
		delete[]haveEll[i];
	}
	delete[]haveEll;
	DeleteObject(hBrush);
	DeleteObject(hBrushEll);
	UnregisterClass(szWinClass, hThisInstance);

	return 0;
}