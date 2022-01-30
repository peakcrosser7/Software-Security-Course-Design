#include "pch.h"
#include "apiHook.h"


using namespace std;



HANDLE(WINAPI* OldCreateFile)(
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
	) = CreateFileW;
BOOL(WINAPI* OldDeleteFile)(
	LPCTSTR lpFileName
	) = DeleteFile;
//截获读文件
BOOL(WINAPI* OldWriteFile)(
	HANDLE hFile,
	LPCVOID lpBuffer,
	DWORD nNumberOfBytesToWrite,
	LPDWORD lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped
	) = WriteFile;
BOOL(WINAPI* OldReadFile)(
	HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped
	) = ReadFile;
BOOL(WINAPI* OldCopyFile)(
	LPCWSTR lpExistingFileName,
	LPCWSTR lpNewFileName,
	BOOL bFailIfExists
	) = CopyFile;

//extern "C" {
	//截获打开文件

unordered_map<HANDLE, wstring> fileMap;

	__declspec(dllexport)HANDLE WINAPI NewCreateFile(
		LPCWSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile
	) {
		HANDLE hFile=OldCreateFile(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
			dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
		if (GetFileType(hFile) != FILE_TYPE_DISK) {
			return hFile;
		}
		fileMap[hFile]=wstring(lpFileName);
		
		//waitSem();
		//printf("\n\n***********************\n");
		printf("Hook API: CreateFile\n");
		
		printTime();
		printf("lpFileName: %ls\n", lpFileName);
		printf("dwDesiredAccess: 0x%lx\n", dwDesiredAccess);
		printf("dwShareMode: 0x%lx\n", dwShareMode);
		printf("lpSecurityAttributes: 0x%p\n", lpSecurityAttributes);
		printf("dwCreationDispostion: %#lx\n", dwCreationDisposition);
		printf("dwFlagAndAttributes: %#lx\n", dwFlagsAndAttributes);
		printf("hTemplateFile: 0x%p\n", hTemplateFile);

		printf("$HOOKEND$");
		//releaseSem();
		return hFile;
	}



	__declspec(dllexport)BOOL WINAPI NewDeleteFile(
		LPCTSTR lpFileName 
	) {
		bool isExe = judgeExe(lpFileName);
		//waitSem();
		//printf("\n\n***********************\n");
		printf("Hook API: DeleteFile\n");
		printTime();
		printf("lpFileName: %ls\n", lpFileName);
		if (isExe) printf("Delete executable file!");
		printf("$HOOKEND$");
		//releaseSem();
		return OldDeleteFile(lpFileName);
	}

	__declspec(dllexport)BOOL WINAPI NewReadFile(
		HANDLE hFile,
		LPVOID lpBuffer,
		DWORD nNumberOfBytesToRead,
		LPDWORD lpNumberOfBytesRead,
		LPOVERLAPPED lpOverlapped
	) {
		BOOL ret = OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead,
			lpNumberOfBytesRead, lpOverlapped);
		int len = (lpNumberOfBytesRead ? *lpNumberOfBytesRead : 0);
		//waitSem();
		//printf("\n\n***********************\n");
		printf("Hook API: ReadFile\n");
		
		printTime();
		printf("hFile: 0x%p\n", hFile);
		printf("lpBuffer: 0x%p\n", lpBuffer);
		printf("nNumberOfBytesToRead: %lu\n", nNumberOfBytesToRead);
		printf("NumberOfBytesRead: %lu\n", len);
		printf("lpOverlapped: 0x%p\n", lpOverlapped);
		printf("read data: ");
		for (int i = 0; i < len && ((char*)lpBuffer)[i]; ++i) {
			putchar(((char*)lpBuffer)[i]);
		}
		putchar('\n');
		printf("$HOOKEND$");
		//releaseSem();
		return ret;
	}


	__declspec(dllexport)BOOL WINAPI NewCopyFile(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		BOOL bFailIfExists
	) {
		WCHAR curFile[MAX_PATH];
		WCHAR copiedFile[MAX_PATH];
		GetModuleFileName(NULL, curFile, MAX_PATH);
		//wcscpy_s(copiedFile, MAX_PATH, lpExistingFileName);
		GetFullPathName(lpExistingFileName, MAX_PATH, copiedFile, NULL);

		printf("Hook API: CopyFile\n");
		
		printTime();
		printf("ExistingFileName: %ls\n", lpExistingFileName);
		printf("NewFileName: %ls\n", lpNewFileName);
		printf("bFailIfExist: %d - %s\n", bFailIfExists, bFailIfExists ? "TRUE" : "FALSE");

		printf("CurrentFile: %ls\n", curFile);	
		printf("CopiedFile: %ls\n", copiedFile);

		BOOL ret;
		if (wcscmp(curFile, copiedFile) == 0) {
			printf("File Copies Itself! :      \n");
			ret = FALSE;

		}
		else {
			ret = OldCopyFile(lpExistingFileName, lpNewFileName, bFailIfExists);
		}
		printf("$HOOKEND$");
		return ret;
	}
//}


	__declspec(dllexport)BOOL WINAPI NewWriteFile(
		HANDLE hFile,
		LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten,
		LPOVERLAPPED lpOverlapped
	) {
		BOOL ret = OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite,
			lpNumberOfBytesWritten, lpOverlapped);
		if (GetFileType(hFile) != FILE_TYPE_DISK) {
			return ret;
		}
		bool isExe = judgeExe(hFile);
		int len = (lpNumberOfBytesWritten ? *lpNumberOfBytesWritten : 0);
		printf("Hook API: WriteFile\n");
		printTime();
		printf("hFile: 0x%p\n", hFile);
		printf("lpBuffer: 0x%p\n", lpBuffer);
		printf("nNumberOfBytesToWrite: %lu\n", nNumberOfBytesToWrite);
		printf("NumberOfBytesWritten: %lu\n", len);
		printf("write data: ");
		for (int i = 0; i < len && ((char*)lpBuffer)[i]; ++i) {
			putchar(((char*)lpBuffer)[i]);
		}
		putchar('\n');
		printf("lpOverlapped: 0x%p\n", lpOverlapped);
		if (isExe) printf("Modified executable file: %ls\n",fileMap[hFile].c_str());
		printf("$HOOKEND$");
		return ret;
	}

	wstring getFileExt(const wstring& filepath) {
		auto idx = filepath.find_last_of(L'.');
		if (idx == wstring::npos) return L"";
		return filepath.substr(idx + 1);
	}

	bool judgeExe(HANDLE hFile) {
		if (fileMap.count(hFile) == 0) return false;
		auto ext = getFileExt(fileMap[hFile]);
		return ext == L"exe" || ext == L"dll" || ext == L"ocx";
	}

	bool judgeExe(const WCHAR* filepath) {
		auto ext = getFileExt(filepath);
		return ext == L"exe" || ext == L"dll" || ext == L"ocx";
	}
