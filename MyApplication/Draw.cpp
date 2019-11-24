#include "MyApplication.h"


void ThreadDraw(MAIN_DATA_STRUCT* MD) {
	WaitForSingleObject(MD->SemCanExit, INFINITE);
	while (MD->workThread)
	{
		PAINTSTRUCT ps;
		//HDC hdc = BeginPaint(MD->hwnd, &ps);
		HDC hdc = GetDC(MD->hwnd);
		GetClientRect(MD->hwnd, &ps.rcPaint);
		//FillRect(hdc, &ps.rcPaint, MD->hBrush);

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
		//std::cout << "1";
		//Sleep(1);
		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	ReleaseSemaphore(MD->SemCanExit, 1, NULL);
}