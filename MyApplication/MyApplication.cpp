#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <windowsx.h>
#include <random>
#include <fstream>
//#include <string> // Подключаю стринг
#define COLOR_ELLIPS RGB(255, 0, 0)
#define COLOR_RAND RGB(rand() % 255, rand() % 255, rand() % 255)
#define TYPE_STREAM 1
#define TYPE_MAPPING 2


const int TYPE_OF_IO = TYPE_STREAM;
const wchar_t* fname = _T("Param.dat");

const int HOTKEY__SHIFT_C__NOTEPAD = 15;
const int HOTKEY__RETURN__CHANGE_COLOR = 16;
const int HOTKEY__ESC__EXIT = 17;
const int HOTKEY__CONTROL_Q__EXIT = 18;
//const int STANDART_N = 15; // Не 0!
//const int STANDART_szXWND = 320;
//const int STANDART_szYWND = 240;
const char STANDART_ICON_NAME[] = "icon.ico";
const TCHAR szWinClass[] = _T("Win32SampleApp");
const TCHAR szWinName[] = _T("Win32SampleWindow");
HWND hwnd;               /* This is the handle for our window */
HPEN lPen;
HBRUSH hBrush;           /* Current brush */
HBRUSH hBrushEll;
int xElipse = -1;
int yElipse = -1;


struct loadData {
	int N = 3;
	int szXWND = 320;
	int szYWND = 240;
	COLORREF colorBack = RGB(0, 0, 255);
	COLORREF colorLine = RGB(255, 0, 0);
	char nameIcon[1024];
} DataF;
bool** haveEll;

struct FileMapping {
	HANDLE hFile;
	HANDLE hMapping;
	size_t fsize;
	unsigned char* dataPtr;
};


bool ReadParamMapping() {
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
		std::cout << "fileMappingCreate - CreateFile failed, fname = "
			<< fname << std::endl;
		return NULL;
	}

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == INVALID_FILE_SIZE) {
		std::cout << "fileMappingCreate - GetFileSize failed, fname = "
			<< fname << std::endl;
		CloseHandle(hFile);
		return NULL;
	}

	HANDLE hMapping = CreateFileMappingW(
		hFile,
		NULL,
		PAGE_READONLY, // Changed
		0,
		0,
		NULL
	);
	if (hMapping == NULL) {
		std::cout << "fileMappingCreate - CreateFileMapping failed, fname = "
			<< fname << std::endl;
		CloseHandle(hFile);
		return NULL;
	}



	LPVOID hViewOfFile = MapViewOfFile(
		hMapping,
		FILE_MAP_READ,
		0,
		0,
		dwFileSize
	);
	unsigned char* dataPtr = (unsigned char*)hViewOfFile;
	if (dataPtr == NULL) {
		std::cout << "fileMappingCreate - MapViewOfFile failed, fname = "
			<< fname << std::endl;
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return NULL;
	}

	/*FileMapping* mapping = (FileMapping*)malloc(sizeof(FileMapping));
	if (mapping == NULL) {
		std::cerr << "fileMappingCreate - malloc failed, fname = "
			<< fname << std::endl;
		UnmapViewOfFile(dataPtr);
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return NULL;
	}*/
	DataF.N = dataPtr[0];
	DataF.szXWND = dataPtr[2];
	DataF.szYWND = dataPtr[4];
	DataF.colorBack = dataPtr[6];
	DataF.colorLine = dataPtr[8];
	for (int i = 10; i < dwFileSize; i++) {
		DataF.nameIcon[i] = dataPtr[i];
	}
	//mapping->hFile = hFile;
	//mapping->hMapping = hMapping;
	//mapping->dataPtr = dataPtr;
	//mapping->fsize = (size_t)dwFileSize;
	UnmapViewOfFile(hViewOfFile);
	CloseHandle(hMapping);
	CloseHandle(hFile);
}
// Оптимизировать
bool ReadParamStream() {
	std::ifstream fin(fname);
	char tmpIconName[1024];
	if (fin >> DataF.N) {
		fin >> DataF.szXWND >> DataF.szYWND >> DataF.colorBack
			>> DataF.colorLine >> tmpIconName;
		// ПЕРЕДЕЛАТЬ
		for (int i = 0; i < strlen(tmpIconName); i++) {
			DataF.nameIcon[i] = tmpIconName[i];
		}
		fin.close();
	}
	return 1;
}

bool ReadParam() {
	switch (TYPE_OF_IO)
	{
	case TYPE_MAPPING: {
		ReadParamMapping();
		break;
	}
	default:
		return ReadParamStream();
	}
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
	default:
		return WriteParamStream();
	}
}


void RunNotepad(void)
{
	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;

	ZeroMemory(&sInfo, sizeof(STARTUPINFO));

	//puts("Starting Notepad...");
	CreateProcess(_T("C:\\Windows\\Notepad.exe"),
		NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
	CloseHandle(pInfo.hProcess);
	CloseHandle(pInfo.hThread);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {/*
	case WM_CREATE: {
		return 0;
	}*/
	case WM_DESTROY: {
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
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
		//if (hwnd == GetActiveWindow())
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
	if (!ReadParam()) return 0;
	if (DataF.nameIcon[0] == '\0') setStandartIcon();

	BOOL bMessageOk;
	MSG message;            /* Here message to the application are saved */
	WNDCLASSEX wincl = { 0 };         /* Data structure for the windowclass */
	HINSTANCE hThisInstance = GetModuleHandle(NULL); /* Get handle */

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

	wincl.hInstance = hThisInstance;	/* The Window structure */
	wincl.lpszClassName = szWinClass;
	wincl.style = CS_HREDRAW | CS_VREDRAW;
	wincl.lpfnWndProc = WindowProcedure;      /* This function is called by Windows */
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.cbSize = sizeof(WNDCLASSEX); // ??
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