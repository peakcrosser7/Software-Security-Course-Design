#include "pch.h"
#include "apiHook.h"

using namespace std;


//函数指针
int (WINAPI* OldMessageBoxA)(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_ UINT uType
	) = MessageBoxA;

int (WINAPI* OldMessageBoxW)(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCWSTR lpText,
	_In_opt_ LPCWSTR lpCaption,
	_In_ UINT uType
	) = MessageBoxW;


extern "C" {
	//新的弹框函数
	__declspec(dllexport)int WINAPI NewMessageBoxA(
		HWND hWnd,
		LPCSTR lpText,
		LPCSTR lpCaption,
		UINT uType
	) {
		printf("Hook API: MessageBoxA\n");
		
		printTime();
		printf("hWnd: %#p\n", hWnd);
		printf("lpText: %s\n", lpText);
		printf("lpCaption: %s\n", lpCaption);
		printf("uType: %u\n", uType);
		printf("$HOOKEND$");
		return OldMessageBoxA(hWnd, lpText, lpCaption, uType);
	}


	__declspec(dllexport)int WINAPI NewMessageBoxW(
		HWND hWnd,
		LPCWSTR lpText,
		LPCWSTR lpCaption,
		UINT uType
	) {
		printf("Hook API: MessageBoxW\n");
		printTime();
		printf("hWnd: %#p\n", hWnd);
		printf("lpText: %ls\n", lpText);
		printf("lpCaption: %ls\n", lpCaption);
		printf("uType: %u\n", uType);
		printf("$HOOKEND$");
		return OldMessageBoxW(hWnd, lpText, lpCaption, uType);
	}

}