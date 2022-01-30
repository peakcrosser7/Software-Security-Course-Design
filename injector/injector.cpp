#include <cstdio>
#include <iostream>
#include <windows.h>
#include <detours.h>

using namespace std;

#define _CRT_SECURE_NO_WARNINGS	
#pragma comment(lib, "detours.lib")

const WCHAR* DirPath = L"E:\\Computer\\C_Language\\SoftwSec\\Debug";
char DLLPath[MAX_PATH] = "E:\\Computer\\C_Language\\SoftwSec\\Debug\\SoftwSec.dll";
const char* DLLPathWithAlloc = "E:\\Computer\\C_Language\\SoftwSec\\Debug\\SoftwSecWithHeapAlloc.dll";


int main(int argc, char* argv[]) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	WCHAR exePath[MAX_PATH + 1]{ 0 };
	WCHAR exeName[21];

	if (argc == 1) {
		wscanf_s(L"%ls", exeName, 20);
		wsprintf(exePath, L"%s\\%s.exe", DirPath, exeName);
	}
	if (argc >= 2) {
		if (argv[1][0] == '.') {
			wsprintf(exeName, L"%hs", argv[1] + 2);
			wsprintf(exePath, L"%s\\%s", DirPath, exeName);
		}
		else {
			wsprintf(exePath, L"%hs", argv[1]);
		}

	}
	if (argc == 3) {
		if (strcmp(argv[2], "WithAlloc")==0) {
			strcpy_s(DLLPath, DLLPathWithAlloc);
		}
	}


	//创建一个新进程并将DLL加载到其中, 进程是挂起状态下创建
	if (DetourCreateProcessWithDllEx(exePath, NULL, NULL, NULL, TRUE,
		CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DirPath,
		&si, &pi, DLLPath, NULL)) {
		ResumeThread(pi.hThread);	//使线程的挂起时间计数减一
		WaitForSingleObject(pi.hProcess, INFINITE);	//等待进程返回
	}
	else {
		printf_s("Error : %d", GetLastError());
		printf("$HOOKEND$");
	}
	printf("$HOOKFIN$");
	return 0;
}