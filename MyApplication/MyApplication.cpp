#include "MyApplication.h"

//#ifndef HOTKEY__SHIFT_C__NOTEPAD
//#define HOTKEY__SHIFT_C__NOTEPAD 15
//#endif // !HOTKEY__SHIFT_C__NOTEPAD
//#ifndef HOTKEY__RETURN__CHANGE_COLOR
//#define HOTKEY__RETURN__CHANGE_COLOR 16
//#endif // !HOTKEY__RETURN__CHANGE_COLOR
//#ifndef HOTKEY__ESC__EXIT
//#define HOTKEY__ESC__EXIT 17
//#endif // !HOTKEY__ESC__EXIT
//#ifndef HOTKEY__CONTROL_Q__EXIT
//#define HOTKEY__CONTROL_Q__EXIT 18
//#endif // !HOTKEY__CONTROL_Q__EXIT

MAIN_DATA_STRUCT MAIN_DATA;
#define MD MAIN_DATA

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
	//std::cout << message << std::endl;
	if (message == MD.WM_UPDATEDATA) {
		//int* dataPtr = (int*)pBuf;
		//xElipse = dataPtr[0];
		//yElipse = dataPtr[1];
		/*bool* dataPtr1 = (bool*)MD.pBuf;
		int tmp = 0;
		for (int i = 0; i < MD.DataF.N + 1; i++) {
			for (int j = 0; j < MD.DataF.N + 1; j++) {
				MD.ellHelp.haveEll[i][j] = dataPtr1[tmp++];
			}
		}
		int* dataPtr2 = (int*)MD.pBuf;
		for (int i = 0; i < MD.DataF.N + 1; i++) {
			for (int j = 0; j < MD.DataF.N + 1; j++) {
				MD.ellHelp.TypeEll[i][j] = dataPtr2[tmp++];
			}
		}*/
		//SendMessage(hwnd, WM_PAINT, NULL, NULL);
		InvalidateRect(hwnd, NULL, true);
		UpdateWindow(hwnd);
		//char* dataPtr = (char*)pBuf;
		//std::cout << "Oleg" << dataPtr[0] <<  std::endl;
		return 0;
	}
	switch (message) {
	case WM_DESTROY: {
		//Актуализация размеров окна

		MD.workThread = false;
		WaitForSingleObject(MD.SemCanExit, INFINITE);

		RECT tmpWR = { 0 };
		GetWindowRect(hwnd, &tmpWR);
		MD.DataF.szXWNDCreated = tmpWR.right - tmpWR.left;
		MD.DataF.szYWNDCreated = tmpWR.bottom - tmpWR.top;

		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT: {
		//PAINTSTRUCT ps;
		//HDC hdc = BeginPaint(hwnd, &ps);
		/*
		//GetClientRect(hwnd, &ps.rcPaint);

		//HGDIOBJ tempPen = SelectObject(hdc, (HGDIOBJ)MD.lPen);

		//double stepX = ps.rcPaint.right / (double)(MD.DataF.N + 1);
		//double positionX = stepX;
		//double stepY = ps.rcPaint.bottom / (double)(MD.DataF.N + 1);
		//double positionY = stepY;


		////if (xElipse > 0 && stepX != 0 && stepY != 0) {
		////	int i = (int)(xElipse / stepX);
		////	int j = (int)(yElipse / stepY);
		////	ellHelp.haveEll[i][j] = !ellHelp.haveEll[i][j];
		////	if (DataF.RCountIcon != 0) {
		////		ellHelp.TypeEll[i][j] = rand() % DataF.RCountIcon;

		////	}
		////	xElipse = yElipse = -1;
		////	//SendMessage(HWND_BROADCAST, WM_UPDATEDATA, NULL, NULL);
		////}

		//if (MD.DataF.RCountIcon == 0) {
		//	SelectObject(hdc, MD.hBrushEll);
		//	for (int i = 0; i < MD.DataF.N + 1; i++)
		//		for (int j = 0; j < MD.DataF.N + 1; j++)
		//			if (MD.ellHelp.haveEll[i][j] && stepX != 0 && stepY != 0) {
		//				Ellipse(hdc, stepX * i, stepY * j, stepX * i + stepX, stepY * j + stepY);
		//			}
		//	SelectObject(hdc, MD.hBrush);
		//}
		//else {
		//	//PatBlt(hdc, 0, 0, 10, 10, WHITENESS);
		//	//PatBlt(hdc, xDest, yDest, xWidth, yHeight, dwROP);
		//	//HDC hdcTMP = CreateDC(pszDriver, pszDevice, pszOutput, pData);

		//	//SetMapMode(hdcTemp, GetMapMode(hdc));

		//	for (int i = 0; i < MD.DataF.N + 1; i++)
		//		for (int j = 0; j < MD.DataF.N + 1; j++)
		//			if (MD.ellHelp.haveEll[i][j] && stepX != 0 && stepY != 0) {
		//				//Ellipse(hdc, stepX * i, stepY * j, stepX * i + stepX, stepY * j + stepY);
		//				HDC hdcTemp = CreateCompatibleDC(hdc);
		//				SelectObject(hdcTemp, myImages[MD.ellHelp.TypeEll[i][j]].bm);
		//				/*BitBlt(hdc,
		//					stepX * i, stepY * j,
		//					stepX * i + stepX - stepX * i, stepY * j + stepY - stepY * j,
		//					hdcTemp,
		//					0, 0,
		//					SRCCOPY);*/
		//				BLENDFUNCTION b = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		//				/*TransparentBlt(
		//					hdc,
		//					stepX * i, stepY * j,
		//					stepX * i + stepX - stepX * i, stepY * j + stepY - stepY * j,
		//					hdcTemp,
		//					0, 0,
		//					myImages[ellHelp.TypeEll[i][j]].width, myImages[ellHelp.TypeEll[i][j]].height,
		//					RGB(0, 0, 0));*/
		//				AlphaBlend(
		//					hdc,
		//					stepX * i, stepY * j,
		//					stepX * i + stepX - stepX * i, stepY * j + stepY - stepY * j,
		//					hdcTemp,
		//					0, 0,
		//					myImages[MD.ellHelp.TypeEll[i][j]].width, myImages[MD.ellHelp.TypeEll[i][j]].height,
		//					b);
		//				DeleteDC(hdcTemp);
		//			}

		//}

		//for (int i = 0; i < MD.DataF.N; i++) {
		//	MoveToEx(hdc, positionX, 0, NULL);
		//	LineTo(hdc, positionX, ps.rcPaint.bottom);
		//	MoveToEx(hdc, 0, positionY, NULL);
		//	LineTo(hdc, ps.rcPaint.right, positionY);
		//	positionX += stepX;
		//	positionY += stepY;
		//}

		//SelectObject(hdc, tempPen);
		//EndPaint(hwnd, &ps);
		////DeleteObject(hBitmapImage);
		break;
	}
	case WM_LBUTTONDOWN: {
		//int* dataPtr = (int*)pBuf;
		MD.xElipse = GET_X_LPARAM(lParam);
		MD.yElipse = GET_Y_LPARAM(lParam);

		RECT mR;

		GetClientRect(hwnd, &mR);

		double stepX = mR.right / (double)(MD.DataF.N + 1);
		double positionX = stepX;
		double stepY = mR.bottom / (double)(MD.DataF.N + 1);
		double positionY = stepY;

		if (MD.xElipse > 0 && stepX != 0 && stepY != 0) {
			int i = (int)(MD.xElipse / stepX);
			int j = (int)(MD.yElipse / stepY);
			MD.ellHelp.haveEll[i][j] = !MD.ellHelp.haveEll[i][j];
			if (MD.DataF.RCountIcon != 0) {
				MD.ellHelp.TypeEll[i][j] = rand() % MD.DataF.RCountIcon;

			}
			MD.xElipse = MD.yElipse = -1;

		}

		SendMessage(HWND_BROADCAST, MD.WM_UPDATEDATA, NULL, NULL);
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
			MD.DataF.colorBack = COLOR_RAND;
			HBRUSH newhBrush = CreateSolidBrush(MD.DataF.colorBack);
			SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG)newhBrush);
			DeleteObject(MD.hBrush);
			MD.hBrush = newhBrush;
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
		MD.TYPE_OF_IO = TPSTREAM;
		std::cout << "IO set \"standart c++ stream\"" << std::endl;
	}
	else
		if (strcmp(arg, "-tm") == 0) {
			MD.TYPE_OF_IO = TMAPPING;
			std::cout << "IO set \"memory mapping\"" << std::endl;
		}
		else
			if (strcmp(arg, "-tw") == 0) {
				MD.TYPE_OF_IO = TWINAPI;
				std::cout << "IO set \"WIN API method\"" << std::endl;
			}
			else if (strcmp(arg, "-ts") == 0) {
				std::cout << "IO set \"c method\"" << std::endl;
			}
}

void loadImage() {
	HMODULE hLib;
	MD.myImages = new img[MD.DataF.countIcon];
	hLib = LoadLibraryA(MD.NAME_MY_DLL);
	if (hLib == NULL) { std::cout << "cant open LIB" << std::endl; return; }

	unsigned char* buff;
	int width;
	int height;

	unsigned char* (*load_image)(const char* filename, int& width, int& height);
	(FARPROC&)load_image = GetProcAddress(hLib, "load_image");

	for (int i = 0; i < MD.DataF.countIcon; i++) {
		buff = (*load_image)(MD.DataF.nameIcons[i], width, height);
		//std::ofstream fdout("checkImage.txt");
		//int tek=0;
		//fdout.fill('0');
		//for (int r1 = 0; r1 < width*4; r1++) {
		//	for (int r2 = 0; r2 < height; r2++) {
		//		fdout.width(3);
		//		fdout << int(buff[tek++]);
		//		if (tek % 4 == 0) fdout << '.';// else buff[tek] = 0;
		//	}
		//	fdout << std::endl;
		//}
		//fdout.close();
		for (int y = 0; y < height; ++y)
		{
			BYTE* pPixel = (BYTE*)buff + width * 4 * y;
			for (int x = 0; x < width; ++x)
			{
				pPixel[0] = pPixel[0] * pPixel[3] / 255;
				pPixel[1] = pPixel[1] * pPixel[3] / 255;
				pPixel[2] = pPixel[2] * pPixel[3] / 255;
				pPixel += 4;
			}
		}
		if (buff != NULL) {
			MD.myImages[MD.DataF.RCountIcon].bm = CreateBitmap(width, height, 1, 32, buff);
			MD.myImages[MD.DataF.RCountIcon].width = width;
			MD.myImages[MD.DataF.RCountIcon].height = height;
			MD.DataF.RCountIcon++;
			delete buff;
		}
	}

	FreeLibrary(hLib);
}

//void SendMessage_WM_TIMER() {
//	SendMessage(hwnd, WM_TIMER, NULL, NULL);
//}

int main(int argc, char* argv[])
{
	if (argc > 1)
		setTypeIO(argv[1]);

	ReadParam(&MD);

	MD.WM_UPDATEDATA = RegisterWindowMessage((LPCWSTR)MD.NAME_MY_EVENT);
	if (MD.WM_UPDATEDATA == 0) {
		std::cout << "cant register window message" << std::endl; return 1;
	}
	MD.hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		MD.szName);               // name of mapping object
	if (!MD.hMapFile) {
		MD.hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // use paging file
			NULL,                    // default security
			PAGE_READWRITE,          // read/write access
			0,                       // maximum object size (high-order DWORD)
			(sizeof(bool)) * (MD.DataF.N + 1) * (MD.DataF.N + 1) + (sizeof(int)) * (MD.DataF.N + 1) * (MD.DataF.N + 1),                // maximum object size (low-order DWORD)
			MD.szName);                 // name of mapping object

		if (MD.hMapFile == NULL)
		{
			_tprintf(TEXT("Could not create file mapping object (%d).\n"),
				GetLastError());
			FreeDataF(&MD);
			return 0;
		}
	}
	MD.pBuf = (LPTSTR)MapViewOfFile(MD.hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		(sizeof(bool)) * (MD.DataF.N + 1) * (MD.DataF.N + 1) + (sizeof(int)) * (MD.DataF.N + 1) * (MD.DataF.N + 1));
	if (MD.pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());
		CloseHandle(MD.hMapFile);
		FreeDataF(&MD);
		return 0;
	}

	loadImage();

	BOOL bMessageOk;
	MSG message;
	WNDCLASSEX wincl = { 0 };
	HINSTANCE hThisInstance = GetModuleHandle(NULL);

	MD.hBrush = CreateSolidBrush(MD.DataF.colorBack);
	MD.hBrushEll = CreateSolidBrush(COLOR_ELLIPS);
	MD.lPen = CreatePen(PS_SOLID, 3, MD.DataF.colorLine);

	MD.ellHelp.haveEll = new bool* [MD.DataF.N + 1];
	MD.ellHelp.TypeEll = new int* [MD.DataF.N + 1];

	bool* tmp1;
	tmp1 = (bool*)MD.pBuf;
	for (int i = 0; i < MD.DataF.N + 1; i++) {
		MD.ellHelp.haveEll[i] = tmp1;
		tmp1 += (MD.DataF.N + 1);
	}
	int* tmp2;
	tmp2 = (int*)tmp1;
	for (int i = 0; i < (MD.DataF.N + 1); i++) {
		MD.ellHelp.TypeEll[i] = tmp2;
		tmp2 += (MD.DataF.N + 1);
	}
	for (int i = 0; i < MD.DataF.N + 1; i++) {
		for (int j = 0; j < MD.DataF.N + 1; j++) {
			MD.ellHelp.haveEll[i][j] = false;
			//ellHelp.TypeEll[i][j] = 0;
		}
	}

	wincl.hInstance = hThisInstance;
	wincl.lpszClassName = MD.szWinClass;
	wincl.style = CS_HREDRAW | CS_VREDRAW;
	wincl.lpfnWndProc = WindowProcedure;
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.cbSize = sizeof(WNDCLASSEX);
	wincl.hbrBackground = MD.hBrush;

	if (!RegisterClassEx(&wincl))
		return 0;

	MD.hwnd = CreateWindowEx(
		NULL,
		MD.szWinClass,          /* Classname */
		MD.szWinName,       /* Title Text */
		WS_OVERLAPPEDWINDOW, /* default window */
		CW_USEDEFAULT,       /* Windows decides the position */
		CW_USEDEFAULT,       /* where the window ends up on the screen */
		MD.DataF.szXWNDCreated,                 /* The programs width */
		MD.DataF.szYWNDCreated,                 /* and height in pixels */
		HWND_DESKTOP,        /* The window is a child-window to desktop */
		NULL,                /* No menu */
		hThisInstance,       /* Program Instance handler */
		NULL                 /* No Window Creation data */
	);

	if (MD.hwnd == NULL) {
		return 0;
	}

	MD.SemCanExit = CreateSemaphore(NULL, 1, 1, NULL);
	MD.ptrThread = new std::thread(ThreadDraw, &MD);
	MD.ptrThread->detach();

	ShowWindow(MD.hwnd, SW_SHOW);
	//SetTimer(hwnd, NULL, TIMER_INTERVAL, (TIMERPROC)SendMessage_WM_TIMER);
	SendMessage(HWND_BROADCAST, MD.WM_UPDATEDATA, NULL, NULL);
	while ((bMessageOk = GetMessage(&message, NULL, 0, 0)) != 0) {
		if (bMessageOk == -1) {
			puts("Suddenly, GetMessage failed! You can call GetLastError() to see what happend");
			break;
		}

		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	//WriteParam();
	DestroyWindow(MD.hwnd);
	WriteParam(&MD);

	for (int i = 0; i < MD.DataF.RCountIcon; i++) {
		DeleteObject(MD.myImages[i].bm);
	}
	delete[] MD.myImages;

	/*for (int i = 0; i < MD.DataF.N + 1; i++) {
		delete[]MD.ellHelp.haveEll[i];
		delete[]MD.ellHelp.TypeEll[i];
	}*/
	delete[]MD.ellHelp.haveEll;
	delete[]MD.ellHelp.TypeEll;
	//delete[]DataF.nameIcons;
	DeleteObject(MD.hBrush);
	DeleteObject(MD.hBrushEll);

	UnmapViewOfFile(MD.pBuf);
	CloseHandle(MD.hMapFile);

	UnregisterClass(MD.szWinClass, hThisInstance);

	return 0;
}