#include "MyApplication.h"
#include <string>

std::string NAME_ANIM_PRE = "Animation\\k1 (";
std::string NAME_ANIM_AFT = ").png";

//unsigned char* gradient;
//int widthG = 100;
//int heightG = 100;
int k = 0;
const int maxCountAnim = 90;
int countAnim = 0;
bool stillwork = true;
img* myAnim;
//HBITMAP bm;
//HANDLE semap;
//std::thread* SecThread;
//HANDLE canCloseTD = CreateMutex(NULL, FALSE, NULL);

//void creatingNewBitG(int widthG, int heightG) {
//	WaitForSingleObject(canCloseTD, 0);
//	while (stillwork) {
//		int tek = 0;
//		for (int i = 0; i < heightG; i++)
//			for (int i = 0; i < widthG; i++) {
//				if (gradient[tek] > 255) {
//					gradient[tek] = 0;
//					tek+=1;
//					gradient[tek] = 0;
//					tek += 1;
//					gradient[tek] = 0;
//					tek += 1;
//					tek++;
//				}
//				else {
//					gradient[tek]++;
//					tek++;
//					gradient[tek]++;
//					tek++;
//					gradient[tek]++;
//					tek++;
//					tek++;
//				}
//			}
//		WaitForSingleObject(semap, INFINITE);
//		bm = CreateBitmap(widthG, heightG, 1, 32, gradient);
//	}
//	ReleaseMutex(canCloseTD);
//}

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
		////fdout.close();
		//for (int y = 0; y < height; ++y)
		//{
		//	BYTE* pPixel = (BYTE*)buff + width * 4 * y;
		//	for (int x = 0; x < width; ++x)
		//	{
		//		pPixel[0] = pPixel[0] * pPixel[3] / 255;
		//		pPixel[1] = pPixel[1] * pPixel[3] / 255;
		//		pPixel[2] = pPixel[2] * pPixel[3] / 255;
		//		pPixel += 4;
		//	}
		//}
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

void ThreadDraw(MAIN_DATA_STRUCT* MD) {
	WaitForSingleObject(MD->SemCanExit, INFINITE);

	//semap = CreateSemaphore(NULL, 1, 1, NULL);
	//SecThread = new std::thread(creatingNewBitG, 1920, 1080);
	//SecThread->detach();

	/*gradient = new unsigned char[4 * 1920 * 1080];
	int tek = 0;
	for (int j = 0; j < 1080; j++)
		for (int i = 0; i < 1920; i++) {
			gradient[tek] = 0;
			tek++;
			gradient[tek] = 0;
			tek++;
			gradient[tek] = 0;
			tek++;
			gradient[tek] = 255;
			tek++;
		}*/
	loadImage1(MD);

	while (MD->workThread)
	{
		//WaitForSingleObject(MD->SemCanExit, INFINITE);
		//ReleaseMutex(MD->SemCanExit);
		WaitForSingleObject(MD->stopSem, INFINITE);
		ReleaseSemaphore(MD->stopSem, 1, NULL);
		//InvalidateRect(MD->hwnd, NULL, true);
		PAINTSTRUCT ps;
		//HDC hdc = BeginPaint(MD->hwnd, &ps);
		HDC hdc = GetDC(MD->hwnd);
		GetClientRect(MD->hwnd, &ps.rcPaint);


		FillRect(hdc, &ps.rcPaint, MD->hBrush);
		HDC hdcTemp2 = CreateCompatibleDC(hdc);
		SelectObject(hdcTemp2, myAnim[k].bm);
		if (k >= countAnim) k = 0;
		BLENDFUNCTION b = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

		//MoveToEx(hdc, k, 0, NULL);
		//LineTo(hdc, k, ps.rcPaint.bottom);
		//k++;
		//if (k > ps.rcPaint.right) k = 0;

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
			//PatBlt(hdc, 0, 0, 10, 10, WHITENESS);
			//PatBlt(hdc, xDest, yDest, xWidth, yHeight, dwROP);
			//HDC hdcTMP = CreateDC(pszDriver, pszDevice, pszOutput, pData);

			//SetMapMode(hdcTemp, GetMapMode(hdc));

			for (int i = 0; i < MD->DataF.N + 1; i++)
				for (int j = 0; j < MD->DataF.N + 1; j++)
					if (MD->ellHelp.haveEll[i][j] && stepX != 0 && stepY != 0) {
						//Ellipse(hdc, stepX * i, stepY * j, stepX * i + stepX, stepY * j + stepY);
						HDC hdcTemp = CreateCompatibleDC(hdc);

						SelectObject(hdcTemp, MD->myImages[MD->ellHelp.TypeEll[i][j]].bm);
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
						AlphaBlend(
							hdc,
							stepX * i, stepY * j,
							stepX * i + stepX - stepX * i, stepY * j + stepY - stepY * j,
							hdcTemp,
							0, 0,
							MD->myImages[MD->ellHelp.TypeEll[i][j]].width, MD->myImages[MD->ellHelp.TypeEll[i][j]].height,
							b);
						DeleteDC(hdcTemp);
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

		SelectObject(hdc, tempPen);
		//EndPaint(MD->hwnd, &ps);
		ReleaseDC(MD->hwnd, hdc);

		DeleteDC(hdcTemp2);
		//DeleteObject(bm);
		//ReleaseSemaphore(semap, 1, NULL);

		//std::cout << "1";
		//ReleaseMutex(MD->stopSem);
		Sleep(50);
		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	//DeleteObject(bm);7
	//stillwork = false;
	//ReleaseSemaphore(semap, 1, NULL);
	//WaitForSingleObject(canCloseTD, 10000);
	//CloseHandle(semap);
	//delete[] gradient;
	ReleaseSemaphore(MD->SemCanExit, 1, NULL);
	delete[] myAnim;
	//CloseHandle(canCloseTD);
}