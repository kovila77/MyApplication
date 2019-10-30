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
#define TPSTREAM 1
#define TMAPPING 2
#define TWINAPI 3
#define TSTREAMIO 4
#define NAME_MY_DLL "WWI.dll"
#define NAME_MY_EVENT "WM_UPDATEDATA"

int TYPE_OF_IO = TPSTREAM;
const wchar_t* fname = _T("Param.dat");

const int HOTKEY__SHIFT_C__NOTEPAD = 15;
const int HOTKEY__RETURN__CHANGE_COLOR = 16;
const int HOTKEY__ESC__EXIT = 17;
const int HOTKEY__CONTROL_Q__EXIT = 18;
const TCHAR szWinClass[] = _T("Win32SampleApp");
const TCHAR szWinName[] = _T("Win32SampleWindow");
TCHAR szName[] = TEXT("MyFileMappingObject");
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

struct img {
	HBITMAP bm;
	int width;
	int height;
};
img* myImages;

//struct myImg {
//	unsigned char* buff;
//	int width;
//	int height;
//} myImage;


void FreeDataF() {
	delete[] DataF.lenIcon;
	for (int i = 0; i < DataF.countIcon; i++) {
		delete[] DataF.nameIcons[i];
	}
	delete[] DataF.nameIcons;
}


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
		>> DataF.colorLine >> DataF.countIcon;
	DataF.nameIcons = new char* [DataF.countIcon];
	DataF.lenIcon = new int[DataF.countIcon];
	for (int i = 0; i < DataF.countIcon; i++) {
		sstr >> DataF.lenIcon[i];
		DataF.nameIcons[i] = new char[DataF.lenIcon[i] + 1];
		ZeroMemory(DataF.nameIcons[i], DataF.lenIcon[i] + 1);
		sstr >> DataF.nameIcons[i];
	}/*DataF.lenIcon;
	DataF.nameIcons = new char[DataF.lenIcon + 1];
	ZeroMemory(DataF.nameIcons, DataF.lenIcon + 1);
	sstr >> DataF.nameIcons;*/

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
		>> DataF.colorLine >> DataF.countIcon;
	DataF.nameIcons = new char* [DataF.countIcon];
	DataF.lenIcon = new int[DataF.countIcon];
	for (int i = 0; i < DataF.countIcon; i++) {
		sstr >> DataF.lenIcon[i];
		DataF.nameIcons[i] = new char[DataF.lenIcon[i] + 1];
		ZeroMemory(DataF.nameIcons[i], DataF.lenIcon[i] + 1);
		sstr >> DataF.nameIcons[i];
	} /*DataF.lenIcon;
	DataF.nameIcons = new char[DataF.lenIcon + 1];
	ZeroMemory(DataF.nameIcons, DataF.lenIcon + 1);
	sstr >> DataF.nameIcons;*/

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
		>> DataF.colorLine >> DataF.countIcon;
	DataF.nameIcons = new char* [DataF.countIcon];
	DataF.lenIcon = new int[DataF.countIcon];
	for (int i = 0; i < DataF.countIcon; i++) {
		sstr >> DataF.lenIcon[i];
		DataF.nameIcons[i] = new char[DataF.lenIcon[i] + 1];
		ZeroMemory(DataF.nameIcons[i], DataF.lenIcon[i] + 1);
		sstr >> DataF.nameIcons[i];
	}
	/*DataF.nameIcons = new char[DataF.lenIcon + 1];
	ZeroMemory(DataF.nameIcons, DataF.lenIcon + 1);
	sstr >> DataF.nameIcons;*/

	UnmapViewOfFile(hViewOfFile);
	CloseHandle(hMapping);
	CloseHandle(hFile);
	return 1;
}

bool ReadParamStream() {
	std::ifstream fin(fname);
	if (!fin.is_open()) return false;
	fin >> DataF.N >> DataF.szXWNDCreated >> DataF.szYWNDCreated >> DataF.colorBack
		>> DataF.colorLine >> DataF.countIcon;
	DataF.nameIcons = new char* [DataF.countIcon];
	DataF.lenIcon = new int[DataF.countIcon];
	for (int i = 0; i < DataF.countIcon; i++) {
		fin >> DataF.lenIcon[i];
		DataF.nameIcons[i] = new char[DataF.lenIcon[i] + 1];
		ZeroMemory(DataF.nameIcons[i], DataF.lenIcon[i] + 1);
		fin >> DataF.nameIcons[i];
	}
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
		<< std::endl << DataF.countIcon;
	for (int i = 0; i < DataF.countIcon; i++) {
		sstr << std::endl << strlen(DataF.nameIcons[i])
			<< std::endl << DataF.nameIcons[i];
	}
	FreeDataF();
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
		<< std::endl << DataF.countIcon;
	for (int i = 0; i < DataF.countIcon; i++) {
		sstr << std::endl << strlen(DataF.nameIcons[i])
			<< std::endl << DataF.nameIcons[i];
	}
	FreeDataF();
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
		<< std::endl << DataF.countIcon;
	for (int i = 0; i < DataF.countIcon; i++) {
		sstr << std::endl << strlen(DataF.nameIcons[i])
			<< std::endl << DataF.nameIcons[i];
	}
	FreeDataF();
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
		<< std::endl << DataF.countIcon;
	for (int i = 0; i < DataF.countIcon; i++) {
		fout << std::endl << strlen(DataF.nameIcons[i])
			<< std::endl << DataF.nameIcons[i];
	}
	FreeDataF();
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
	std::cout << message << std::endl;
	if (message == WM_UPDATEDATA) {
		//int* dataPtr = (int*)pBuf;
		//xElipse = dataPtr[0];
		//yElipse = dataPtr[1];
		bool* dataPtr1 = (bool*)pBuf;
		int tmp = 0;
		for (int i = 0; i < DataF.N + 1; i++) {
			for (int j = 0; j < DataF.N + 1; j++) {
				ellHelp.haveEll[i][j] = dataPtr1[tmp++];
			}
		}
		int* dataPtr2 = (int*)pBuf;
		for (int i = 0; i < DataF.N + 1; i++) {
			for (int j = 0; j < DataF.N + 1; j++) {
				ellHelp.TypeEll[i][j] = dataPtr2[tmp++];
			}
		}
		//SendMessage(hwnd, WM_PAINT, NULL, NULL);
		InvalidateRect(hwnd, NULL, true);
		//char* dataPtr = (char*)pBuf;
		//std::cout << "Oleg" << dataPtr[0] <<  std::endl;
	}
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

		HGDIOBJ tempPen = SelectObject(hdc, (HGDIOBJ)lPen);

		double stepX = ps.rcPaint.right / (double)(DataF.N + 1);
		double positionX = stepX;
		double stepY = ps.rcPaint.bottom / (double)(DataF.N + 1);
		double positionY = stepY;


		//if (xElipse > 0 && stepX != 0 && stepY != 0) {
		//	int i = (int)(xElipse / stepX);
		//	int j = (int)(yElipse / stepY);
		//	ellHelp.haveEll[i][j] = !ellHelp.haveEll[i][j];
		//	if (DataF.RCountIcon != 0) {
		//		ellHelp.TypeEll[i][j] = rand() % DataF.RCountIcon;

		//	}
		//	xElipse = yElipse = -1;
		//	//SendMessage(HWND_BROADCAST, WM_UPDATEDATA, NULL, NULL);
		//}

		if (DataF.RCountIcon == 0) {
			SelectObject(hdc, hBrushEll);
			for (int i = 0; i < DataF.N + 1; i++)
				for (int j = 0; j < DataF.N + 1; j++)
					if (ellHelp.haveEll[i][j] && stepX != 0 && stepY != 0) {
						Ellipse(hdc, stepX * i, stepY * j, stepX * i + stepX, stepY * j + stepY);
					}
			SelectObject(hdc, hBrush);
		}
		else {
			//PatBlt(hdc, 0, 0, 10, 10, WHITENESS);
			//PatBlt(hdc, xDest, yDest, xWidth, yHeight, dwROP);
			//HDC hdcTMP = CreateDC(pszDriver, pszDevice, pszOutput, pData);

			//SetMapMode(hdcTemp, GetMapMode(hdc));

			for (int i = 0; i < DataF.N + 1; i++)
				for (int j = 0; j < DataF.N + 1; j++)
					if (ellHelp.haveEll[i][j] && stepX != 0 && stepY != 0) {
						//Ellipse(hdc, stepX * i, stepY * j, stepX * i + stepX, stepY * j + stepY);
						HDC hdcTemp = CreateCompatibleDC(hdc);
						SelectObject(hdcTemp, myImages[ellHelp.TypeEll[i][j]].bm);
						/*BitBlt(hdc,
							stepX * i, stepY * j,
							stepX * i + stepX - stepX * i, stepY * j + stepY - stepY * j,
							hdcTemp,
							0, 0,
							SRCCOPY);*/
						BLENDFUNCTION b = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
						/*TransparentBlt(
							hdc,
							stepX * i, stepY * j,
							stepX * i + stepX - stepX * i, stepY * j + stepY - stepY * j,
							hdcTemp,
							0, 0,
							myImages[ellHelp.TypeEll[i][j]].width, myImages[ellHelp.TypeEll[i][j]].height,
							RGB(0, 0, 0));*/
						AlphaBlend(
							hdc,
							stepX * i, stepY * j,
							stepX * i + stepX - stepX * i, stepY * j + stepY - stepY * j,
							hdcTemp,
							0, 0,
							myImages[ellHelp.TypeEll[i][j]].width, myImages[ellHelp.TypeEll[i][j]].height,
							b);
						DeleteDC(hdcTemp);
					}

		}

		for (int i = 0; i < DataF.N; i++) {
			MoveToEx(hdc, positionX, 0, NULL);
			LineTo(hdc, positionX, ps.rcPaint.bottom);
			MoveToEx(hdc, 0, positionY, NULL);
			LineTo(hdc, ps.rcPaint.right, positionY);
			positionX += stepX;
			positionY += stepY;
		}

		SelectObject(hdc, tempPen);
		EndPaint(hwnd, &ps);
		//DeleteObject(hBitmapImage);
		break;
	}
	case WM_LBUTTONDOWN: {
		//int* dataPtr = (int*)pBuf;
		xElipse = GET_X_LPARAM(lParam);
		yElipse = GET_Y_LPARAM(lParam);

		RECT mR;

		GetClientRect(hwnd, &mR);

		double stepX = mR.right / (double)(DataF.N + 1);
		double positionX = stepX;
		double stepY = mR.bottom / (double)(DataF.N + 1);
		double positionY = stepY;


		if (xElipse > 0 && stepX != 0 && stepY != 0) {
			int i = (int)(xElipse / stepX);
			int j = (int)(yElipse / stepY);
			ellHelp.haveEll[i][j] = !ellHelp.haveEll[i][j];
			if (DataF.RCountIcon != 0) {
				ellHelp.TypeEll[i][j] = rand() % DataF.RCountIcon;

			}
			xElipse = yElipse = -1;
			
		}

		bool* dataPtr1 = (bool*)pBuf;
		int tmp = 0;
		for (int i = 0; i < DataF.N + 1; i++) {
			for (int j = 0; j < DataF.N + 1; j++) {
				dataPtr1[tmp++] = ellHelp.haveEll[i][j];
			}
		}
		int* dataPtr2 = (int*)pBuf;
		for (int i = 0; i < DataF.N + 1; i++) {
			for (int j = 0; j < DataF.N + 1; j++) {
				dataPtr2[tmp++] = ellHelp.TypeEll[i][j];
			}
		}

		//FlushViewOfFile(pBuf, (sizeof(bool))* (DataF.N + 1)* (DataF.N + 1) + (sizeof(int)) * (DataF.N + 1) * (DataF.N + 1));
		//dataPtr[0] = xElipse = GET_X_LPARAM(lParam);
		//dataPtr[1] = yElipse = GET_Y_LPARAM(lParam);
		InvalidateRect(hwnd, NULL, true);
		SendMessage(HWND_BROADCAST, WM_UPDATEDATA, NULL, NULL);
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
	myImages = new img[DataF.countIcon];
	hLib = LoadLibraryA(NAME_MY_DLL);
	if (hLib == NULL) { std::cout << "cant open LIB" << std::endl; return; }

	unsigned char* buff;
	int width;
	int height;

	unsigned char* (*load_image)(const char* filename, int& width, int& height);
	(FARPROC&)load_image = GetProcAddress(hLib, "load_image");

	for (int i = 0; i < DataF.countIcon; i++) {
		buff = (*load_image)(DataF.nameIcons[i], width, height);
		if (buff != NULL) {
			myImages[DataF.RCountIcon].bm = CreateBitmap(width, height, 1, 32, buff);
			myImages[DataF.RCountIcon].width = width;
			myImages[DataF.RCountIcon].height = height;
			DataF.RCountIcon++;
			delete buff;
		}
	}

	FreeLibrary(hLib);
}

//void SendMessage_WM_TIMER() {
//	SendMessage(hwnd, WM_TIMER, NULL, NULL);
//}

//
//bool ReadSlot()
//{
//	DWORD cbMessage, cMessage, cbRead;
//	BOOL fResult;
//	LPTSTR lpszBuffer;
//	TCHAR achID[80];
//	DWORD cAllMessages;
//	HANDLE hEvent;
//	OVERLAPPED ov;
//
//	cbMessage = cMessage = cbRead = 0;
//
//	hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ExampleSlot"));
//	if (NULL == hEvent)
//		return FALSE;
//	ov.Offset = 0;
//	ov.OffsetHigh = 0;
//	ov.hEvent = hEvent;
//
//	fResult = GetMailslotInfo(hSlot, // mailslot handle 
//		(LPDWORD)NULL,               // no maximum message size 
//		&cbMessage,                   // size of next message 
//		&cMessage,                    // number of messages 
//		(LPDWORD)NULL);              // no read time-out 
//
//	if (!fResult)
//	{
//		printf("GetMailslotInfo failed with %d.\n", GetLastError());
//		return FALSE;
//	}
//
//	if (cbMessage == MAILSLOT_NO_MESSAGE)
//	{
//		printf("Waiting for a message...\n");
//		return TRUE;
//	}
//
//	cAllMessages = cMessage;
//
//	while (cMessage != 0)  // retrieve all messages
//	{
//		// Create a message-number string. 
//
//		StringCchPrintf((LPTSTR)achID,
//			80,
//			TEXT("\nMessage #%d of %d\n"),
//			cAllMessages - cMessage + 1,
//			cAllMessages);
//
//		// Allocate memory for the message. 
//
//		lpszBuffer = (LPTSTR)GlobalAlloc(GPTR,
//			lstrlen((LPTSTR)achID) * sizeof(TCHAR) + cbMessage);
//		if (NULL == lpszBuffer)
//			return FALSE;
//		lpszBuffer[0] = '\0';
//
//		fResult = ReadFile(hSlot,
//			lpszBuffer,
//			cbMessage,
//			&cbRead,
//			&ov);
//
//		if (!fResult)
//		{
//			printf("ReadFile failed with %d.\n", GetLastError());
//			GlobalFree((HGLOBAL)lpszBuffer);
//			return FALSE;
//		}
//
//		// Concatenate the message and the message-number string. 
//
//		StringCbCat(lpszBuffer,
//			lstrlen((LPTSTR)achID) * sizeof(TCHAR) + cbMessage,
//			(LPTSTR)achID);
//
//		// Display the message. 
//
//		_tprintf(TEXT("Contents of the mailslot: %s\n"), lpszBuffer);
//
//		GlobalFree((HGLOBAL)lpszBuffer);
//
//		fResult = GetMailslotInfo(hSlot,  // mailslot handle 
//			(LPDWORD)NULL,               // no maximum message size 
//			&cbMessage,                   // size of next message 
//			&cMessage,                    // number of messages 
//			(LPDWORD)NULL);              // no read time-out 
//
//		if (!fResult)
//		{
//			printf("GetMailslotInfo failed (%d)\n", GetLastError());
//			return FALSE;
//		}
//	}
//	CloseHandle(hEvent);
//	return TRUE;
//}

int main(int argc, char* argv[])
{
	if (argc > 1)
		setTypeIO(argv[1]);

	ReadParam();

	WM_UPDATEDATA = RegisterWindowMessage((LPCWSTR)NAME_MY_EVENT);
	if (WM_UPDATEDATA == 0) {
		std::cout << "cant register window message" << std::endl; return 1;
	}
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		szName);               // name of mapping object
	if (!hMapFile) {
		hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // use paging file
			NULL,                    // default security
			PAGE_READWRITE,          // read/write access
			0,                       // maximum object size (high-order DWORD)
			(sizeof(bool))*(DataF.N + 1) * (DataF.N + 1) + (sizeof(int))*(DataF.N + 1) * (DataF.N + 1),                // maximum object size (low-order DWORD)
			szName);                 // name of mapping object

		if (hMapFile == NULL)
		{
			_tprintf(TEXT("Could not create file mapping object (%d).\n"),
				GetLastError());
			FreeDataF();
			return 0;
		}
	}
	pBuf = (LPTSTR)MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		(sizeof(bool)) * (DataF.N + 1) * (DataF.N + 1) + (sizeof(int)) * (DataF.N + 1) * (DataF.N + 1));
	if (pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());
		CloseHandle(hMapFile);
		FreeDataF();
		return 0;
	}

	loadImage();

	BOOL bMessageOk;
	MSG message;
	WNDCLASSEX wincl = { 0 };
	HINSTANCE hThisInstance = GetModuleHandle(NULL);

	hBrush = CreateSolidBrush(DataF.colorBack);
	hBrushEll = CreateSolidBrush(COLOR_ELLIPS);
	lPen = CreatePen(PS_SOLID, 3, DataF.colorLine);

	ellHelp.haveEll = new bool* [DataF.N + 1];
	ellHelp.TypeEll = new int* [DataF.N + 1];
	for (int i = 0; i < DataF.N + 1; i++) {
		ellHelp.haveEll[i] = new bool[DataF.N + 1];
		ellHelp.TypeEll[i] = new int[DataF.N + 1];
	}
	for (int i = 0; i < DataF.N + 1; i++) {
		for (int j = 0; j < DataF.N + 1; j++) {
			ellHelp.haveEll[i][j] = false;
			//ellHelp.TypeEll[i][j] = 0;
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
	//SetTimer(hwnd, NULL, TIMER_INTERVAL, (TIMERPROC)SendMessage_WM_TIMER);
	SendMessage(HWND_BROADCAST, WM_UPDATEDATA, NULL, NULL);
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

	for (int i = 0; i < DataF.RCountIcon; i++) {
		DeleteObject(myImages[i].bm);
	}
	delete[] myImages;

	for (int i = 0; i < DataF.N + 1; i++) {
		delete[]ellHelp.haveEll[i];
		delete[]ellHelp.TypeEll[i];
	}
	delete[]ellHelp.haveEll;
	delete[]ellHelp.TypeEll;
	//delete[]DataF.nameIcons;
	DeleteObject(hBrush);
	DeleteObject(hBrushEll);

	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);

	UnregisterClass(szWinClass, hThisInstance);

	return 0;
}