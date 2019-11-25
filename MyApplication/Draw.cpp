#include "MyApplication.h"
#include <string>

#define FPS 25
#define TIME_OUT_SEMFPS 1000

std::string NAME_ANIM_PRE = "Animation\\k1 (";
std::string NAME_ANIM_AFT = ").png";

int k = 0;
const int maxCountAnim = 90;
int countAnim = 0;
bool stillwork = true;
img* myAnim;
HANDLE semFPS;

void loadImage1(MAIN_DATA_STRUCT* MD) {
	HMODULE hLib;
	myAnim = new img[maxCountAnim];
	hLib = LoadLibraryA(MD->NAME_MY_DLL);
	if (hLib == NULL) { std::cout << "cant open LIB" << std::endl; return; }

	unsigned char* buff;
	int width;
	int height;

	unsigned char* (*load_image)(const char* filename, int& width, int& height);
	(FARPROC&)load_image = GetProcAddress(hLib, "load_image");

	for (int i = 1; i <= maxCountAnim; i++) {
		std::string tmpname = NAME_ANIM_PRE + std::to_string(i) + NAME_ANIM_AFT;
		buff = (*load_image)(tmpname.c_str(), width, height);
		if (buff != NULL) {
			myAnim[countAnim].bm = CreateBitmap(width, height, 1, 32, buff);
			myAnim[countAnim].width = width;
			myAnim[countAnim].height = height;
			countAnim++;
			delete buff;
		}
		else {
			return;
		}
	}

	FreeLibrary(hLib);
}

void Timer() {
	ReleaseSemaphore(semFPS, 1, NULL);
	//std::cout << "w";
}

void ThreadDraw(MAIN_DATA_STRUCT* MD) {
	loadImage1(MD);
	UINT_PTR timer = SetTimer(MD->hwnd, NULL, 1000/FPS, (TIMERPROC)Timer);
	semFPS = CreateSemaphore(NULL, 1, 1, NULL);
	WaitForSingleObject(semFPS, 1);

	while (MD->workThread)
	{
		WaitForSingleObject(MD->stopSem, INFINITE);
		ReleaseSemaphore(MD->stopSem, 1, NULL);
		PAINTSTRUCT ps;
		HDC hdc = GetDC(MD->hwnd);
		GetClientRect(MD->hwnd, &ps.rcPaint);

		FillRect(hdc, &ps.rcPaint, MD->hBrush);
		HDC hdcTemp2 = CreateCompatibleDC(hdc);
		SelectObject(hdcTemp2, myAnim[k].bm);
		//if (k >= countAnim) k = 0;
		BLENDFUNCTION b = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
		AlphaBlend(
			hdc,
			0, 0,
			ps.rcPaint.right, ps.rcPaint.bottom,
			hdcTemp2,
			0, 0,
			myAnim[k].width, myAnim[k].height,
			b);
		if (++k >= countAnim) k = 0;
		HGDIOBJ tempPen = SelectObject(hdc, (HGDIOBJ)MD->lPen);

		HDC hdcTemp3 = CreateCompatibleDC(hdc);
		if (MD->imACross) {
			SelectObject(hdcTemp3, MD->myImages[0].bm);
			AlphaBlend(
				hdc,
				2, 2,
				22, 22,
				hdcTemp3,
				0, 0,
				MD->myImages[0].width, MD->myImages[0].height,
				b);
		}
		else {
			SelectObject(hdcTemp3, MD->myImages[1].bm);
			AlphaBlend(
				hdc,
				2, 2,
				22, 22,
				hdcTemp3,
				0, 0,
				MD->myImages[1].width, MD->myImages[1].height,
				b);
		}
		DeleteDC(hdcTemp3);

		HDC hdcTempC = CreateCompatibleDC(hdc);
		HANDLE tmp1 = SelectObject(hdcTempC, MD->myImages[0].bm);
		HDC hdcTempZ = CreateCompatibleDC(hdc);
		HANDLE tmp2 = SelectObject(hdcTempZ, MD->myImages[1].bm);

		double stepX = ps.rcPaint.right / (double)(MD->DataF.N + 1);
		double positionX = stepX;
		double stepY = ps.rcPaint.bottom / (double)(MD->DataF.N + 1);
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

		if (MD->DataF.RCountIcon == 0) {
			SelectObject(hdc, MD->hBrushEll);
			for (int i = 0; i < MD->DataF.N + 1; i++)
				for (int j = 0; j < MD->DataF.N + 1; j++)
					if (MD->ellHelp.haveEll[i][j] && stepX != 0 && stepY != 0) {
						Ellipse(hdc, stepX * i, stepY * j, stepX * i + stepX, stepY * j + stepY);
					}
			SelectObject(hdc, MD->hBrush);
		}
		else {
			for (int i = 0; i < MD->DataF.N + 1; i++)
				for (int j = 0; j < MD->DataF.N + 1; j++)
					if (MD->ellHelp.haveEll[i][j] && stepX != 0 && stepY != 0) {
						//Ellipse(hdc, stepX * i, stepY * j, stepX * i + stepX, stepY * j + stepY);
						//HDC hdcTemp = CreateCompatibleDC(hdc);

						//SelectObject(hdcTemp, MD->myImages[MD->ellHelp.TypeEll[i][j]].bm);
						/*BitBlt(hdc,
							stepX * i, stepY * j,
							stepX * i + stepX - stepX * i, stepY * j + stepY - stepY * j,
							hdcTemp,
							0, 0,
							SRCCOPY);*/
							//BLENDFUNCTION b = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
							/*TransparentBlt(
								hdc,
								stepX * i, stepY * j,
								stepX * i + stepX - stepX * i, stepY * j + stepY - stepY * j,
								hdcTemp,
								0, 0,
								myImages[ellHelp.TypeEll[i][j]].width, myImages[ellHelp.TypeEll[i][j]].height,
								RGB(0, 0, 0));*/
						if (MD->ellHelp.TypeEll[i][j] == 0)
							AlphaBlend(
								hdc,
								stepX * i, stepY * j,
								stepX * i + stepX - stepX * i, stepY * j + stepY - stepY * j,
								hdcTempC,
								0, 0,
								MD->myImages[MD->ellHelp.TypeEll[i][j]].width, MD->myImages[MD->ellHelp.TypeEll[i][j]].height,
								b);
						else
							AlphaBlend(
								hdc,
								stepX * i, stepY * j,
								stepX * i + stepX - stepX * i, stepY * j + stepY - stepY * j,
								hdcTempZ,
								0, 0,
								MD->myImages[MD->ellHelp.TypeEll[i][j]].width, MD->myImages[MD->ellHelp.TypeEll[i][j]].height,
								b);
						//DeleteDC(hdcTemp);
					}

		}

		for (int i = 0; i < MD->DataF.N; i++) {
			MoveToEx(hdc, positionX, 0, NULL);
			LineTo(hdc, positionX, ps.rcPaint.bottom);
			MoveToEx(hdc, 0, positionY, NULL);
			LineTo(hdc, ps.rcPaint.right, positionY);
			positionX += stepX;
			positionY += stepY;
		}

		SelectObject(hdcTempC, tmp1);
		SelectObject(hdcTempZ, tmp2);

		DeleteDC(hdcTempC);
		DeleteDC(hdcTempZ);

		SelectObject(hdc, tempPen);
		//EndPaint(MD->hwnd, &ps);
		WaitForSingleObject(semFPS, TIME_OUT_SEMFPS);
		ReleaseDC(MD->hwnd, hdc);

		DeleteDC(hdcTemp2);
		//Sleep(40);
	}
	// При уничтожении окна таймер уничтожается
	//KillTimer(MD->hwnd, timer);
	delete[] myAnim; 
	CloseHandle(semFPS);
}