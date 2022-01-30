 //dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "apiHook.h"

using namespace std;


SYSTEMTIME st;


void detourAttaches();
void detourDettaches();

extern int (WINAPI* OldMessageBoxA)(
	HWND hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT uType
	);
extern int (WINAPI* OldMessageBoxW)(
	HWND hWnd,
	LPCWSTR lpText,
	LPCWSTR lpCaption,
	UINT uType
	);


extern HANDLE(WINAPI* OldCreateFile)(
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
	);
extern BOOL(WINAPI* OldDeleteFile)(
	LPCTSTR lpFileName
	);
extern BOOL(WINAPI* OldWriteFile)(
	HANDLE hFile,
	LPCVOID lpBuffer,
	DWORD nNumberOfBytesToWrite,
	LPDWORD lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped
	);
extern BOOL(WINAPI* OldReadFile)(
	HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped
	);
extern BOOL(WINAPI* OldCopyFile)(
	LPCWSTR lpExistingFileName,
	LPCWSTR lpNewFileName,
	BOOL bFailIfExists
	);


extern LSTATUS(APIENTRY* OldRegCreateKey)(
	_In_ HKEY hKey,
	_In_opt_ LPCWSTR lpSubKey,
	_Out_ PHKEY phkResult
	);
extern LSTATUS(APIENTRY* OldRegCloseKey)(
	_In_ HKEY hKey
	);
extern LSTATUS(APIENTRY* OldRegDeleteKey)(
	_In_ HKEY hKey,
	_In_ LPCWSTR lpSubKey
	);
extern LSTATUS(APIENTRY* OldRegOpenKey)(
	_In_ HKEY hKey,
	_In_opt_ LPCWSTR lpSubKey,
	_Out_ PHKEY phkResult
	);
extern LSTATUS(APIENTRY* OldRegSetKeyValue)(
	_In_ HKEY hKey,
	_In_opt_ LPCWSTR lpSubKey,
	_In_opt_ LPCWSTR lpValueName,
	_In_ DWORD dwType,
	_In_reads_bytes_opt_(cbData) LPCVOID lpData,
	_In_ DWORD cbData
	);
extern LSTATUS(APIENTRY* OldRegGetValue)(
	_In_ HKEY hkey,
	_In_opt_ LPCWSTR lpSubKey,
	_In_opt_ LPCWSTR lpValue,
	_In_ DWORD dwFlags,
	_Out_opt_ LPDWORD pdwType,
	_Out_writes_bytes_to_opt_(*pcbData, *pcbData) PVOID pvData,
	_Inout_opt_ LPDWORD pcbData
	);


extern HANDLE(WINAPI* OldHeapCreate)(
	DWORD fIOoptions,
	SIZE_T dwInitSize,
	SIZE_T dwMaxSize
	);
extern BOOL(WINAPI* OldHeapDestroy)(HANDLE);
extern LPVOID(WINAPI* OldHeapAlloc)(
	_In_ HANDLE hHeap,
	_In_ DWORD dwFlags,
	_In_ SIZE_T dwBytes
	);
extern BOOL(WINAPI* OldHeapFree)(
	HANDLE                 hHeap,
	DWORD                  dwFlags,
	_Frees_ptr_opt_ LPVOID lpMem
	);


extern SOCKET(WSAAPI* OldSocket)(
	_In_ int af,
	_In_ int type,
	_In_ int protocol
	);
extern int (WSAAPI* OldConnect)(
	_In_ SOCKET s,
	_In_reads_bytes_(namelen) const struct sockaddr FAR* name,
	_In_ int namelen
	);
extern int (WSAAPI* OldSend)(
	_In_ SOCKET s,
	_In_reads_bytes_(len) const char FAR* buf,
	_In_ int len,
	_In_ int flags
	);
extern int (WSAAPI* OldRecv)(
	_In_ SOCKET s,
	_Out_writes_bytes_to_(len, return) __out_data_source(NETWORK) char FAR* buf,
	_In_ int len,
	_In_ int flags
	);

void detourAttaches() {
    //在目标函数上附加一个detour函数
	DetourAttach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);
	DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);

	DetourAttach(&(PVOID&)OldCreateFile, NewCreateFile);
	DetourAttach(&(PVOID&)OldDeleteFile, NewDeleteFile);
	DetourAttach(&(PVOID&)OldWriteFile, NewWriteFile);
	DetourAttach(&(PVOID&)OldReadFile, NewReadFile);
	DetourAttach(&(PVOID&)OldCopyFile, NewCopyFile);

	DetourAttach(&(PVOID&)OldRegCreateKey, NewRegCreateKey);
	DetourAttach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
	DetourAttach(&(PVOID&)OldRegDeleteKey, NewRegDeleteKey);
	DetourAttach(&(PVOID&)OldRegSetKeyValue, NewRegSetKeyValue);
	DetourAttach(&(PVOID&)OldRegGetValue, NewRegGetValue);
	DetourAttach(&(PVOID&)OldRegOpenKey, NewRegOpenKey);

	DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
	DetourAttach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
	//DetourAttach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
	//DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);

	DetourAttach(&(PVOID&)OldSocket, NewSocket);
	DetourAttach(&(PVOID&)OldConnect, NewConnect);
	DetourAttach(&(PVOID&)OldSend, NewSend);
	DetourAttach(&(PVOID&)OldRecv, NewRecv);
}

void detourDettaches() {
	DetourDetach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);
	DetourDetach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);

	DetourDetach(&(PVOID&)OldCreateFile, NewCreateFile);
	DetourDetach(&(PVOID&)OldDeleteFile, NewDeleteFile);
	DetourDetach(&(PVOID&)OldWriteFile, NewWriteFile);
	DetourDetach(&(PVOID&)OldReadFile, NewReadFile);
	DetourDetach(&(PVOID&)OldCopyFile, NewCopyFile);

	DetourDetach(&(PVOID&)OldRegCreateKey, NewRegCreateKey);
	DetourDetach(&(PVOID&)OldRegCloseKey, NewRegCloseKey);
	DetourDetach(&(PVOID&)OldRegDeleteKey, NewRegDeleteKey);
	DetourDetach(&(PVOID&)OldRegSetKeyValue, NewRegSetKeyValue);
	DetourDetach(&(PVOID&)OldRegGetValue, NewRegGetValue);
	DetourDetach(&(PVOID&)OldRegOpenKey, NewRegOpenKey);

	DetourDetach(&(PVOID&)OldHeapCreate, NewHeapCreate);
	DetourDetach(&(PVOID&)OldHeapDestroy, NewHeapDestroy);
	//DetourDetach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
	//DetourDetach(&(PVOID&)OldHeapFree, NewHeapFree);

	DetourDetach(&(PVOID&)OldSocket, NewSocket);
	DetourDetach(&(PVOID&)OldConnect, NewConnect);
	DetourDetach(&(PVOID&)OldSend, NewSend);
	DetourDetach(&(PVOID&)OldRecv, NewRecv);
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:    //进程正在加载DLL
		//    DisableThreadLibraryCalls 禁用指定的DLL的DLL_THREAD_ATTACH
		// 和DLL_THREAD_DETACH通知，可减小某些程序的工作集大小。
		//    该函数在有许多DLL，频繁创建和删除线程，并且DLL不需要线程级消息如
		// DLL_THREAD_ATTACH and DLL_THREAD_DETACH时的多线程应用中是很
		// 有效的优化
		DisableThreadLibraryCalls(hModule);
		DetourTransactionBegin();   //开始一个新的附加或分离detour事务
		DetourUpdateThread(GetCurrentThread()); //在当前事务中登记要更新的线程
		detourAttaches();
		DetourTransactionCommit();  //提交当前事务以附加或分离detour
		break;
	case DLL_THREAD_ATTACH:     //进程正在创建线程
	case DLL_THREAD_DETACH:     //线程正常退出
		break;
	case DLL_PROCESS_DETACH:    //进程卸载DLL
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		detourDettaches();
		DetourTransactionCommit();
		break;
	}
	return TRUE;
}





