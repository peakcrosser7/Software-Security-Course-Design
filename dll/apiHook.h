#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS	
#include "framework.h"
#include <stdio.h>
#include <stdarg.h>
#include <detours.h>
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#pragma comment(lib,"detours.lib")
#pragma comment(lib, "ws2_32.lib")
extern SYSTEMTIME st;


std::wstring getFileExt(const std::wstring& filepath);
bool judgeExe(HANDLE hFile);
bool judgeExe(const WCHAR* filepath);


inline void printTime() {
	GetLocalTime(&st);
	printf("Hook Time: %d-%d-%02d %02d:%02d:%02d:%03d\n", st.wYear, st.wMonth,
		st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}


extern "C" {
	__declspec(dllexport)int WINAPI NewMessageBoxA(
		HWND hWnd,
		LPCSTR lpText,
		LPCSTR lpCaption,
		UINT uType
	);
	__declspec(dllexport)int WINAPI NewMessageBoxW(
		HWND hWnd,
		LPCWSTR lpText,
		LPCWSTR lpCaption,
		UINT uType
	);



	__declspec(dllexport) LSTATUS APIENTRY NewRegCreateKey(
		_In_ HKEY hKey,
		_In_opt_ LPCWSTR lpSubKey,
		_Out_ PHKEY phkResult
	);
	__declspec(dllexport) LSTATUS APIENTRY NewRegCloseKey(
		_In_ HKEY hKey
	);
	__declspec(dllexport) LSTATUS APIENTRY NewRegDeleteKey(
		_In_ HKEY hKey,
		_In_ LPCWSTR lpSubKey
	);
	__declspec(dllexport) LSTATUS APIENTRY NewRegOpenKey(
		_In_ HKEY hKey,
		_In_opt_ LPCWSTR lpSubKey,
		_Out_ PHKEY phkResult
	);
	__declspec(dllexport) LSTATUS APIENTRY NewRegSetKeyValue(
		_In_ HKEY hKey,
		_In_opt_ LPCWSTR lpSubKey,
		_In_opt_ LPCWSTR lpValueName,
		_In_ DWORD dwType,
		_In_reads_bytes_opt_(cbData) LPCVOID lpData,
		_In_ DWORD cbData
	);
	__declspec(dllexport) LSTATUS APIENTRY NewRegGetValue(
		_In_ HKEY hkey,
		_In_opt_ LPCWSTR lpSubKey,
		_In_opt_ LPCWSTR lpValue,
		_In_ DWORD dwFlags,
		_Out_opt_ LPDWORD pdwType,
		_Out_writes_bytes_to_opt_(*pcbData, *pcbData) PVOID pvData,
		_Inout_opt_ LPDWORD pcbData
	);


	__declspec(dllexport)HANDLE WINAPI NewHeapCreate(
		DWORD fIOoptions,
		SIZE_T dwInitSize,
		SIZE_T dwMaxSize
	);
	__declspec(dllexport)BOOL WINAPI NewHeapDestroy(
		HANDLE hHeap
	);


	__declspec(dllexport) SOCKET WSAAPI NewSocket(
		_In_ int af,
		_In_ int type,
		_In_ int protocol
	);
	__declspec(dllexport) int WSAAPI NewConnect(
		_In_ SOCKET s,
		_In_reads_bytes_(namelen) const struct sockaddr FAR* name,
		_In_ int namelen
	);
	__declspec(dllexport) int WSAAPI NewSend(
		_In_ SOCKET s,
		_In_reads_bytes_(len) const char FAR* buf,
		_In_ int len,
		_In_ int flags
	);
	__declspec(dllexport) int WSAAPI NewRecv(
		_In_ SOCKET s,
		_Out_writes_bytes_to_(len, return) __out_data_source(NETWORK) char FAR* buf,
		_In_ int len,
		_In_ int flags
	);
}


__declspec(dllexport)HANDLE WINAPI NewCreateFile(
	LPCWSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
);
__declspec(dllexport)BOOL WINAPI NewDeleteFile(
	LPCTSTR lpFileName
);
//__declspec(dllexport) BOOL NewDeleteFile(
//	LPCWSTR lpFileName
//);
__declspec(dllexport)BOOL WINAPI NewWriteFile(
	HANDLE hFile,
	LPCVOID lpBuffer,
	DWORD nNumberOfBytesToWrite,
	LPDWORD lpNumberOfBytesWritten,
	LPOVERLAPPED lpOverlapped
);
__declspec(dllexport)BOOL WINAPI NewReadFile(
	HANDLE hFile,
	LPVOID lpBuffer,
	DWORD nNumberOfBytesToRead,
	LPDWORD lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped
);
__declspec(dllexport)BOOL WINAPI NewCopyFile(
	LPCWSTR lpExistingFileName,
	LPCWSTR lpNewFileName,
	BOOL bFailIfExists
);

__declspec(dllexport)LPVOID WINAPI NewHeapAlloc(
	_In_ HANDLE hHeap,
	_In_ DWORD dwFlags,
	_In_ SIZE_T dwBytes
);
__declspec(dllexport)BOOL WINAPI NewHeapFree(
	HANDLE                 hHeap,
	DWORD                  dwFlags,
	_Frees_ptr_opt_ LPVOID lpMem
);