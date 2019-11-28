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

void RunCopyMyProgramm() {
	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;

	ZeroMemory(&sInfo, sizeof(STARTUPINFO));

	CreateProcess(_T("MyApplication.exe"),
		NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
	CloseHandle(pInfo.hProcess);
	CloseHandle(pInfo.hThread);
}

void clearArea() {
	for (int i = 0; i < MD.DataF.N + 1; i++) {
		for (int j = 0; j < MD.DataF.N + 1; j++) {
			MD.ellHelp.haveEll[i][j] = false;
			//ellHelp.TypeEll[i][j] = 0;
		}
	}
	*MD.lastWasCross = false;
	*MD.countClick = 0;
	MessageBoxA(MD.hwnd, "Cross`s turn", "Begining", MB_OK);
}

void SayWhoWinAndExit(const char* msg) {
	MessageBoxA(MD.hwnd, msg, "THE END OF GAME!", MB_OK);
	//DialogBoxA()
	//SendMessage(HWND_BROADCAST, MD.WM_EXIT, NULL, NULL);
}

void checkWin() {
	bool winCW;
	bool winCH;
	bool winCD;
	bool winZW;
	bool winZH;
	bool winZD;
	bool winCOD;
	bool winZOD;
	for (int i = 0; i < MD.DataF.N + 1; i++) {
		winCW = true;
		winZW = true;
		for (int j = 0; (winCW || winZW) && (j < MD.DataF.N + 1); j++) {
			if (!(MD.ellHelp.haveEll[i][j] && MD.ellHelp.TypeEll[i][j] == 0))
				winCW = false;
			if (!(MD.ellHelp.haveEll[i][j] && MD.ellHelp.TypeEll[i][j] == 1))
				winZW = false;
		}
		if (winCW) {
			//SayWhoWinAndExit("Win Cross!!!");
			PostMessage(HWND_BROADCAST, MD.WM_EXIT, 'c', NULL);
			return;
		}
		if (winZW) {
			//SayWhoWinAndExit("Win Zeros!!!");
			PostMessage(HWND_BROADCAST, MD.WM_EXIT, 'z', NULL);
			return;
		}
	}

	for (int i = 0; i < MD.DataF.N + 1; i++) {
		winCH = true;
		winZH = true;
		for (int j = 0; (winCH || winZH) && (j < MD.DataF.N + 1); j++) {
			if (!(MD.ellHelp.haveEll[j][i] && MD.ellHelp.TypeEll[j][i] == 0))
				winCH = false;
			if (!(MD.ellHelp.haveEll[j][i] && MD.ellHelp.TypeEll[j][i] == 1))
				winZH = false;
		}
		if (winCH) {
			//SayWhoWinAndExit("Win Cross!!!");
			PostMessage(HWND_BROADCAST, MD.WM_EXIT, 'c', NULL);
			return;
		}
		if (winZH) {
			//SayWhoWinAndExit("Win Zeros!!!");
			PostMessage(HWND_BROADCAST, MD.WM_EXIT, 'z', NULL);
			return;
		}
	}

	winZD = true;
	winCD = true;
	for (int i = 0; (winZD || winCD) && (i < MD.DataF.N + 1); i++) {
		if (!(MD.ellHelp.haveEll[i][i] && MD.ellHelp.TypeEll[i][i] == 0))
			winCD = false;
		if (!(MD.ellHelp.haveEll[i][i] && MD.ellHelp.TypeEll[i][i] == 1))
			winZD = false;
	}
	if (winCD) {
		//SayWhoWinAndExit("Win Cross!!!");
		PostMessage(HWND_BROADCAST, MD.WM_EXIT, 'c', NULL);
		return;
	}
	if (winZD) {
		//SayWhoWinAndExit("Win Zeros!!!");
		PostMessage(HWND_BROADCAST, MD.WM_EXIT, 'z', NULL);
		return;
	}

	winCOD = true;
	winZOD = true;
	for (int i = 0; (winZOD || winCOD) && (i < MD.DataF.N + 1); i++) {
		if (!(MD.ellHelp.haveEll[MD.DataF.N - i][i] && MD.ellHelp.TypeEll[MD.DataF.N - i][i] == 0))
			winCOD = false;
		if (!(MD.ellHelp.haveEll[MD.DataF.N - i][i] && MD.ellHelp.TypeEll[MD.DataF.N - i][i] == 1))
			winZOD = false;
	}
	if (winCOD) {
		//SayWhoWinAndExit("Win Cross!!!");
		PostMessage(HWND_BROADCAST, MD.WM_EXIT, 'c', NULL);
		return;
	}
	if (winZOD) {
		//SayWhoWinAndExit("Win Zeros!!!");
		PostMessage(HWND_BROADCAST, MD.WM_EXIT, 'z', NULL);
		return;
	}
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == MD.WM_EXIT) {
		if (wParam != 'n')
			if (wParam == 'c')
				if (MD.imACross)
					SayWhoWinAndExit("You win!!! (Cross win)");
				else
					SayWhoWinAndExit("You lose... (Cross win)");
			else
				if (MD.imACross)
					SayWhoWinAndExit("You lose... (Zeros win)");
				else
					SayWhoWinAndExit("You win!!! (Zeros win)");
		else
			MessageBoxA(MD.hwnd, "No one won.", "...", MB_OK);
		PostMessageW(MD.hwnd, WM_QUIT, 0, 0);
	}
	switch (message) {
	case WM_DESTROY: {
		if (MD.imACross) *MD.haveCross = false;
		else *MD.haveZero = false;

		MD.workThread = false;
		ReleaseSemaphore(MD.stopSem, 1, NULL);
		//WaitForSingleObject(MD.SemCanExit, INFINITE);
		MD.ptrThread->join();

		RECT tmpWR = { 0 };
		GetWindowRect(hwnd, &tmpWR);
		MD.DataF.szXWNDCreated = tmpWR.right - tmpWR.left;
		MD.DataF.szYWNDCreated = tmpWR.bottom - tmpWR.top;

		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_LBUTTONDOWN: {
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
			if (MD.ellHelp.haveEll[i][j]) break;
			if (MD.imACross) {
				if (*MD.lastWasCross) {
					MessageBoxA(MD.hwnd, "Its Zeros turn!", "Not your turn!", MB_OK);
					break;
				}
				*MD.lastWasCross = true;
			}
			else {
				if (!(*MD.lastWasCross)) {
					MessageBoxA(MD.hwnd, "Its Crosss turn!", "Not your turn!", MB_OK);
					break;
				}
				*MD.lastWasCross = false;
			}

			(*MD.countClick)++;
			MD.ellHelp.haveEll[i][j] = true;//!MD.ellHelp.haveEll[i][j];

			if (MD.DataF.RCountIcon != 0) {
				if (MD.imACross)
					MD.ellHelp.TypeEll[i][j] = 0;
				else
					MD.ellHelp.TypeEll[i][j] = 1;
			}
			MD.xElipse = MD.yElipse = -1;
		}

		checkWin();
		if (*MD.countClick >= (MD.DataF.N + 1) * (MD.DataF.N + 1)) {
			//MessageBoxA(MD.hwnd, "No one won.", "...", MB_OK);
			PostMessage(HWND_BROADCAST, MD.WM_EXIT, 'n', NULL);
			//*MD.countClick = 0;
			//clearArea();
		}
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
					/*case WM_TIMER:
					{
						std::cout << "notT";
						return 0;
					}*/
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_SPACE: {
			if (WaitForSingleObject(MD.stopSem, 50) == WAIT_TIMEOUT)
				ReleaseSemaphore(MD.stopSem, 1, NULL);
			break;
		}
		case '1':
		{
			bool k = SetThreadPriority(MD.ptrThread->native_handle(), THREAD_PRIORITY_IDLE);
			std::cout << k;
			break;
		}
		case '2':
		{
			bool k = SetThreadPriority(MD.ptrThread->native_handle(), THREAD_PRIORITY_LOWEST);
			std::cout << k;
			break;
		}
		case '3':
		{
			bool k = SetThreadPriority(MD.ptrThread->native_handle(), THREAD_PRIORITY_BELOW_NORMAL);
			std::cout << k;
			break;
		}
		case '4':
		{
			bool k = SetThreadPriority(MD.ptrThread->native_handle(), THREAD_PRIORITY_NORMAL);
			std::cout << k;
			break;
		}
		case '5':
		{
			bool k = SetThreadPriority(MD.ptrThread->native_handle(), THREAD_PRIORITY_ABOVE_NORMAL);
			std::cout << k;
			break;
		}
		case '6':
		{
			bool k = SetThreadPriority(MD.ptrThread->native_handle(), THREAD_PRIORITY_HIGHEST);
			std::cout << k;
			break;
		}
		case '7':
		{
			bool k = SetThreadPriority(MD.ptrThread->native_handle(), THREAD_PRIORITY_TIME_CRITICAL);
			std::cout << k;
			break;
		}
		default: break;
		}
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

int main(int argc, char* argv[])
{
	if (argc > 1)
		setTypeIO(argv[1]);

	ReadParam(&MD);

	MD.WM_EXIT = RegisterWindowMessage((LPCWSTR)MD.NAME_EXIT_EVENT);
	if (MD.WM_EXIT == 0) {
		for (int i = 0; i < MD.DataF.RCountIcon; i++) {
			DeleteObject(MD.myImages[i].bm);
		}
		delete[] MD.myImages;
		FreeDataF(&MD);
		std::cout << "cant register window message" << std::endl;
		return 0;
	}
	MD.hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		MD.szName);               // name of mapping object
	if (!MD.hMapFile) {
		MD.imACross = true;
		MD.imFirst = true;
		MD.hMapFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // use paging file
			NULL,                    // default security
			PAGE_READWRITE,          // read/write access
			0,                       // maximum object size (high-order DWORD)
			((sizeof(bool)) * (MD.DataF.N + 1) * (MD.DataF.N + 1) + (sizeof(int)) * (MD.DataF.N + 1) * (MD.DataF.N + 1) + 3 * sizeof(bool) + sizeof(int)),                // maximum object size (low-order DWORD)
			MD.szName);                 // name of mapping object

		if (MD.hMapFile == NULL)
		{
			_tprintf(TEXT("Could not create file mapping object (%d).\n"),
				GetLastError());
			for (int i = 0; i < MD.DataF.RCountIcon; i++) {
				DeleteObject(MD.myImages[i].bm);
			}
			delete[] MD.myImages;
			FreeDataF(&MD);
			return 0;
		}
	}
	MD.pBuf = (LPTSTR)MapViewOfFile(MD.hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		((sizeof(bool)) * (MD.DataF.N + 1) * (MD.DataF.N + 1) + (sizeof(int)) * (MD.DataF.N + 1) * (MD.DataF.N + 1) + 3 * sizeof(bool)) + sizeof(int));
	if (MD.pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());
		CloseHandle(MD.hMapFile);
		for (int i = 0; i < MD.DataF.RCountIcon; i++) {
			DeleteObject(MD.myImages[i].bm);
		}
		delete[] MD.myImages;
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
	MD.haveCross = (bool*)tmp2;
	MD.lastWasCross = MD.haveCross + 1;
	MD.haveZero = MD.lastWasCross + 1;
	MD.countClick = (int*)(MD.haveZero + 1);

	if (MD.imFirst) {
		//*MD.countClick = 0;
		//*MD.lastWasCross = false; 
		*MD.haveZero = false;
		clearArea();
		RunCopyMyProgramm();
	}
	if (MD.imACross) { *MD.haveCross = true; }
	else
		if (!(*MD.haveCross)) {
			MD.imACross = true;
			*MD.haveCross = true;
		}
		else {
			if (!(*MD.haveZero)) {
				*MD.haveZero = true;
			}
			else {
				MessageBoxA(MD.hwnd, "Too many copy of programm", "Close...", MB_OK);

				for (int i = 0; i < MD.DataF.RCountIcon; i++) {
					DeleteObject(MD.myImages[i].bm);
				}
				delete[] MD.myImages;
				FreeDataF(&MD);

				delete[]MD.ellHelp.haveEll;
				delete[]MD.ellHelp.TypeEll;
				DeleteObject(MD.hBrush);
				DeleteObject(MD.hBrushEll);

				UnmapViewOfFile(MD.pBuf);
				CloseHandle(MD.hMapFile);
				return 0;
			}
		}

	//for (int i = 0; i < MD.DataF.N + 1; i++) {
	//	for (int j = 0; j < MD.DataF.N + 1; j++) {
	//		MD.ellHelp.haveEll[i][j] = false;
	//		//ellHelp.TypeEll[i][j] = 0;
	//	}
	//}

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
	if (MD.imACross)
		MD.hwnd = CreateWindowEx(
			NULL,
			MD.szWinClass,          /* Classname */
			MD.szWinNameCross,       /* Title Text */
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
	else MD.hwnd = CreateWindowEx(
		NULL,
		MD.szWinClass,          /* Classname */
		MD.szWinNameZero,       /* Title Text */
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

	MD.stopSem = CreateSemaphore(NULL, 1, 1, NULL);
	MD.SemCanExit = CreateSemaphore(NULL, 1, 1, NULL);
	MD.ptrThread = new std::thread(ThreadDraw, &MD);
	//MD.ptrThread->detach();

	ShowWindow(MD.hwnd, SW_SHOW);
	//SetTimer(hwnd, NULL, TIMER_INTERVAL, (TIMERPROC)SendMessage_WM_TIMER);
	//SendMessage(HWND_BROADCAST, MD.WM_EXIT, NULL, NULL);
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

	delete[]MD.ellHelp.haveEll;
	delete[]MD.ellHelp.TypeEll;
	DeleteObject(MD.hBrush);
	DeleteObject(MD.hBrushEll);

	UnmapViewOfFile(MD.pBuf);
	CloseHandle(MD.hMapFile);
	CloseHandle(MD.SemCanExit);
	CloseHandle(MD.stopSem);

	UnregisterClass(MD.szWinClass, hThisInstance);

	return 0;
}