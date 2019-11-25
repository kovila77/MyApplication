#include "MyApplication.h"

void FreeDataF(MAIN_DATA_STRUCT* MD) {
	delete[] MD->DataF.lenIcon;
	for (int i = 0; i < MD->DataF.countIcon; i++) {
		delete[] MD->DataF.nameIcons[i];
	}
	delete[] MD->DataF.nameIcons;
}

void WriteP(std::ostream* str, MAIN_DATA_STRUCT* MD) {
	*str << MD->DataF.N << std::endl << MD->DataF.szXWNDCreated
		<< std::endl << MD->DataF.szYWNDCreated
		<< std::endl << MD->DataF.colorBack
		<< std::endl << MD->DataF.colorLine
		<< std::endl << MD->DataF.countIcon;
	for (int i = 0; i < MD->DataF.countIcon; i++) {
		*str << std::endl << strlen(MD->DataF.nameIcons[i])
			<< std::endl << MD->DataF.nameIcons[i];
	}
	FreeDataF(MD);
}

void ReadP(std::istream* str, MAIN_DATA_STRUCT* MD) {
	*str >> MD->DataF.N >> MD->DataF.szXWNDCreated >> MD->DataF.szYWNDCreated >> MD->DataF.colorBack
		>> MD->DataF.colorLine >> MD->DataF.countIcon;
	MD->DataF.nameIcons = new char* [MD->DataF.countIcon];
	MD->DataF.lenIcon = new int[MD->DataF.countIcon];
	for (int i = 0; i < MD->DataF.countIcon; i++) {
		*str >> MD->DataF.lenIcon[i];
		MD->DataF.nameIcons[i] = new char[MD->DataF.lenIcon[i] + 1];
		ZeroMemory(MD->DataF.nameIcons[i], MD->DataF.lenIcon[i] + 1);
		*str >> MD->DataF.nameIcons[i];
	}
}

bool ReadParamFdoing(MAIN_DATA_STRUCT* MD) {
	FILE* stream;
	_wfopen_s(&stream, MD->fname, _T("r"));
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
	ReadP(&sstr, MD);
	/*DataF.lenIcon;
	DataF.nameIcons = new char[DataF.lenIcon + 1];
	ZeroMemory(DataF.nameIcons, DataF.lenIcon + 1);
	sstr >> DataF.nameIcons;*/

	delete[] buff;
	fclose(stream);
	return 1;
}

bool ReadParamWinAPI(MAIN_DATA_STRUCT* MD) {
	HANDLE hFile = CreateFileW(
		MD->fname,
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
		delete[]buff;
		CloseHandle(hFile);
		return false;
	}

	sstr << buff;
	ReadP(&sstr, MD);
	/*DataF.lenIcon;
	DataF.nameIcons = new char[DataF.lenIcon + 1];
	ZeroMemory(DataF.nameIcons, DataF.lenIcon + 1);
	sstr >> DataF.nameIcons;*/

	CloseHandle(hFile);
	delete[]buff;
	return 1;
}

bool ReadParamMapping(MAIN_DATA_STRUCT* MD) {
	HANDLE hFile = CreateFileW(
		MD->fname,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hFile == INVALID_HANDLE_VALUE) {
		std::cout << "fileMappingCreate - CreateFile failed, fname = "
			<< MD->fname << std::endl;
		return false;
	}

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	if (dwFileSize == INVALID_FILE_SIZE) {
		std::cout << "fileMappingCreate - GetFileSize failed, fname = "
			<< MD->fname << std::endl;
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
			<< MD->fname << std::endl;
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
			<< MD->fname << std::endl;
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return false;
	}

	std::stringstream sstr;
	sstr << dataPtr;
	ReadP(&sstr, MD);
	/*DataF.nameIcons = new char[DataF.lenIcon + 1];
	ZeroMemory(DataF.nameIcons, DataF.lenIcon + 1);
	sstr >> DataF.nameIcons;*/

	UnmapViewOfFile(hViewOfFile);
	CloseHandle(hMapping);
	CloseHandle(hFile);
	return 1;
}

bool ReadParamStream(MAIN_DATA_STRUCT* MD) {
	std::ifstream fin(MD->fname);
	if (!fin.is_open()) return false;
	ReadP(&fin, MD);

	fin.close();
	return 1;
}

bool ReadParam(MAIN_DATA_STRUCT* MD) {
	switch (MD->TYPE_OF_IO)
	{
	case TMAPPING: {
		return ReadParamMapping(MD);
	}
	case TWINAPI: {
		return ReadParamWinAPI(MD);
	}
	case TSTREAMIO: {
		return ReadParamFdoing(MD);
	}
	default:
		return ReadParamStream(MD);
	}
}


void WriteParamFdoing(MAIN_DATA_STRUCT* MD) {
	FILE* stream;
	_wfopen_s(&stream, MD->fname, _T("w"));
	if (stream == NULL) {
		std::cout << "Error Open file" << std::endl;
		return;
	}

	std::stringstream sstr;
	WriteP(&sstr, MD);
	std::string strStream = sstr.str();

	fwrite(strStream.c_str(), 1, strStream.length(), stream);

	fclose(stream);
}

void WriteParamWinAPI(MAIN_DATA_STRUCT* MD) {
	std::stringstream sstr;
	WriteP(&sstr, MD);
	std::string strStream = sstr.str();

	HANDLE hFile = CreateFileW(
		MD->fname,
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

void WriteParamMapping(MAIN_DATA_STRUCT* MD) {
	std::stringstream sstr;
	WriteP(&sstr, MD);
	std::string strStream = sstr.str();

	HANDLE hFile = CreateFileW(
		MD->fname,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hFile == INVALID_HANDLE_VALUE) {
		std::cout << "fileMappingCreate - CreateFile failed, fname = "
			<< MD->fname << std::endl;
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
			<< MD->fname << std::endl;
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
			<< MD->fname << std::endl;
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return;
	}

	strcpy_s((char*)hViewOfFile, sstr.str().length() + 1, sstr.str().c_str());

	UnmapViewOfFile(hViewOfFile);
	CloseHandle(hMapping);
	CloseHandle(hFile);
}

void WriteParamStream(MAIN_DATA_STRUCT* MD) {
	std::ofstream fout(MD->fname);
	WriteP(&fout, MD);
	fout.close();
}

void WriteParam(MAIN_DATA_STRUCT* MD) {
	switch (MD->TYPE_OF_IO)
	{
	case TWINAPI: {
		WriteParamWinAPI(MD);
		break;
	}
	case TMAPPING: {
		WriteParamMapping(MD);
		break;
	}
	case TSTREAMIO:
		WriteParamFdoing(MD);
		break;
	default:
		WriteParamStream(MD);
		break;
	}
}
