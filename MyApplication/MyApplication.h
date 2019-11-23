#pragma once
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <windowsx.h>
#include <random>
#include <fstream>
#include <sstream>
#include <string>

#include "Settings.h"

#define COLOR_ELLIPS RGB(255, 0, 0)
#define COLOR_RAND RGB(rand() % 255, rand() % 255, rand() % 255)
#define TPSTREAM 1
#define TMAPPING 2
#define TWINAPI 3
#define TSTREAMIO 4

void FreeDataF(MAIN_DATA_STRUCT* MD);

bool ReadParamFdoing(MAIN_DATA_STRUCT* MD);
bool ReadParamWinAPI(MAIN_DATA_STRUCT* MD);
bool ReadParamMapping(MAIN_DATA_STRUCT* MD);
bool ReadParamStream(MAIN_DATA_STRUCT* MD);
bool ReadParam(MAIN_DATA_STRUCT* MD);

void WriteParamFdoing(MAIN_DATA_STRUCT* MD);
void WriteParamWinAPI(MAIN_DATA_STRUCT* MD);
void WriteParamMapping(MAIN_DATA_STRUCT* MD);
void WriteParamStream(MAIN_DATA_STRUCT* MD);
void WriteParam(MAIN_DATA_STRUCT* MD);