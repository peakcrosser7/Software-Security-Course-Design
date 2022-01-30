#include "pch.h"
#include "apiHook.h"
using namespace std;

//extern SYSTEMTIME st;
//截获创建堆
HANDLE(WINAPI* OldHeapCreate)(
	DWORD fIOoptions,
	SIZE_T dwInitSize,
	SIZE_T dwMaxSize
	) = HeapCreate;
//截获释放堆
BOOL(WINAPI* OldHeapDestroy)(HANDLE) = HeapDestroy;
LPVOID(WINAPI* OldHeapAlloc)(
	_In_ HANDLE hHeap,
	_In_ DWORD dwFlags,
	_In_ SIZE_T dwBytes
	) = HeapAlloc;
BOOL(WINAPI* OldHeapFree)(
	HANDLE                 hHeap,
	DWORD                  dwFlags,
	_Frees_ptr_opt_ LPVOID lpMem
	) = HeapFree;


BOOL CantPrintAlloc = FALSE;

extern "C" {
	__declspec(dllexport)HANDLE WINAPI NewHeapCreate(
		DWORD fIOoptions,
		SIZE_T dwInitSize,
		SIZE_T dwMaxSize
	) {
		HANDLE hHeap = OldHeapCreate(fIOoptions, dwInitSize, dwMaxSize);
		printf("Hook API: HeapCreate\n");
		printTime();
		printf("fIOoptions: %lu\n", fIOoptions);
		printf("dwInitialSize: %lu\n", dwInitSize);
		printf("dwMaximumSize: %lu\n", dwMaxSize);
		printf("hHeap: 0x%p\n", hHeap);
		printf("$HOOKEND$");
		return hHeap;
	}

	__declspec(dllexport)BOOL WINAPI NewHeapDestroy(
		HANDLE hHeap
	) {
		printf("Hook API: HeapDestroy\n");
		printTime();
		printf("hHeap: 0x%p\n", hHeap);
		printf("$HOOKEND$");
		return OldHeapDestroy(hHeap);
	}

}

unordered_set<HANDLE> lpMems;

__declspec(dllexport)LPVOID WINAPI NewHeapAlloc(
	_In_ HANDLE hHeap,
	_In_ DWORD dwFlags,
	_In_ SIZE_T dwBytes
) {
	if (CantPrintAlloc == TRUE) {
		return OldHeapAlloc(hHeap, dwFlags, dwBytes);
	}
	CantPrintAlloc = TRUE;
	printf("Hook API: HeapAlloc\n");
	printTime();
	printf("hHeap: 0x%p\n", hHeap);
	printf("dwFlags: %lu\n", dwFlags);
	printf("dwBytes: %lu\n", dwBytes);
	printf("$HOOKEND$");
	LPVOID lpMem = OldHeapAlloc(hHeap, dwFlags, dwBytes);
	lpMems.insert(lpMem);
	CantPrintAlloc = FALSE;
	return lpMem;
}

__declspec(dllexport)BOOL WINAPI NewHeapFree(
	HANDLE                 hHeap,
	DWORD                  dwFlags,
	_Frees_ptr_opt_ LPVOID lpMem
) {
	BOOL res = FALSE;
	bool repeat = true;
	if (lpMems.count(hHeap)) {
		lpMems.erase(hHeap);
		repeat = false;
	}
	printf("Hook API: HeapFree\n");
	printTime();
	printf("hHeap: 0x%p\n", hHeap);
	printf("dwFlags: %lu\n", dwFlags);
	printf("lpMem: 0x%p\n", lpMem);
	if (repeat) {
		printf("Heap Free ERROR : maybe heap Reaptedly free or Not heap HANDLE\n");
		res = FALSE;
	}
	printf("$HOOKEND$");
	return res;
}
