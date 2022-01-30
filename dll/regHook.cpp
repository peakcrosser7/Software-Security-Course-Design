#include "pch.h"
#include "apiHook.h"

using namespace std;


//创建注册表
LSTATUS(APIENTRY* OldRegCreateKey)(
	_In_ HKEY hKey,
	_In_opt_ LPCWSTR lpSubKey,
	_Out_ PHKEY phkResult
	) = RegCreateKey;
//关闭注册表
LSTATUS(APIENTRY* OldRegCloseKey)(
	_In_ HKEY hKey
	) = RegCloseKey;
//删除注册表
LSTATUS(APIENTRY* OldRegDeleteKey)(
	_In_ HKEY hKey,
	_In_ LPCWSTR lpSubKey
	) = RegDeleteKey;
//打开注册表
LSTATUS (APIENTRY* OldRegOpenKey)(
	_In_ HKEY hKey,
	_In_opt_ LPCWSTR lpSubKey,
	_Out_ PHKEY phkResult
) = RegOpenKey;
//设置注册表键值
LSTATUS(APIENTRY* OldRegSetKeyValue)(
	_In_ HKEY hKey,
	_In_opt_ LPCWSTR lpSubKey,
	_In_opt_ LPCWSTR lpValueName,
	_In_ DWORD dwType,
	_In_reads_bytes_opt_(cbData) LPCVOID lpData,
	_In_ DWORD cbData
	) = RegSetKeyValue;
//获取注册表键值
LSTATUS(APIENTRY* OldRegGetValue)(
	_In_ HKEY hkey,
	_In_opt_ LPCWSTR lpSubKey,
	_In_opt_ LPCWSTR lpValue,
	_In_ DWORD dwFlags,
	_Out_opt_ LPDWORD pdwType,
	_Out_writes_bytes_to_opt_(*pcbData, *pcbData) PVOID pvData,
	_Inout_opt_ LPDWORD pcbData
	) = RegGetValue;

extern "C" {
	__declspec(dllexport) LSTATUS APIENTRY NewRegCreateKey(
		_In_ HKEY hKey,
		_In_opt_ LPCWSTR lpSubKey,
		_Out_ PHKEY phkResult
	) {
		LSTATUS res = OldRegCreateKey(hKey, lpSubKey, phkResult);

		printf("Hook API: RegCreateKey\n");
		
		printTime();
		printf("hKey: %#p\n", hKey);
		printf("lpSubKey: %ls\n", lpSubKey);
		printf("phkResult: %#p\n", phkResult);
		printf("$HOOKEND$");

		
		return res;
	}

	__declspec(dllexport) LSTATUS APIENTRY NewRegCloseKey(
		_In_ HKEY hKey
	) {
		printf("Hook API: RegCloseKey\n");
		
		printTime();
		printf("hKey: %#p\n", hKey);
		printf("$HOOKEND$");
		return OldRegCloseKey(hKey);
	}

	__declspec(dllexport) LSTATUS APIENTRY NewRegDeleteKey(
		_In_ HKEY hKey,
		_In_ LPCWSTR lpSubKey
	) {

		printf("Hook API: RegDeleteKey\n");	
		printTime();
		printf("hKey: %#p\n", hKey);
		printf("lpSubKey: %ls\n", lpSubKey);
		printf("$HOOKEND$");

		return OldRegDeleteKey(hKey, lpSubKey);
	}

	__declspec(dllexport) LSTATUS APIENTRY NewRegOpenKey(
		_In_ HKEY hKey,
		_In_opt_ LPCWSTR lpSubKey,
		_Out_ PHKEY phkResult
	) {
		LSTATUS ret = OldRegOpenKey(hKey, lpSubKey, phkResult);
		printf("Hook API: RegSetKeyValue\n");
		printTime();
		printf("hKey: %#p\n", hKey);
		printf("lpSubKey: %ls\n", lpSubKey);
		printf("phkResult: %#p\n", phkResult);
		if (hKey == HKEY_CURRENT_USER && wcscmp(lpSubKey,
			L"Software\\Microsoft\\Windows\\CurrentVersion\\Run")
			== 0) {
			printf("Open Self-starting Execution file item! :     ");
		}
		printf("$HOOKEND$");
		return ret;
	}

	__declspec(dllexport) LSTATUS APIENTRY NewRegSetKeyValue(
		_In_ HKEY hKey,
		_In_opt_ LPCWSTR lpSubKey,
		_In_opt_ LPCWSTR lpValueName,
		_In_ DWORD dwType,
		_In_reads_bytes_opt_(cbData) LPCVOID lpData,
		_In_ DWORD cbData
	) {

		printf("Hook API: RegSetKeyValue\n");
		
		printTime();
		printf("hKey: %#p\n", hKey);
		printf("lpSubKey: %ls\n", lpSubKey);
		printf("lpValueName: %ls\n", lpValueName);
		printf("dwType: %lu\n", dwType);
		printf("lpData: %#p\n", lpData);
		printf("cbData: %lu\n", cbData);
		if (hKey == HKEY_CURRENT_USER && wcscmp(lpSubKey,
			L"Software\\Microsoft\\Windows\\CurrentVersion\\Run")
			== 0) {
			printf("Add Self-starting Execution file item! :      ");
		}
		printf("$HOOKEND$");
		return OldRegSetKeyValue(hKey, lpSubKey, lpValueName, dwType,
			lpData, cbData);
	}

	__declspec(dllexport) LSTATUS APIENTRY NewRegGetValue(
		_In_ HKEY hkey,
		_In_opt_ LPCWSTR lpSubKey,
		_In_opt_ LPCWSTR lpValue,
		_In_ DWORD dwFlags,
		_Out_opt_ LPDWORD pdwType,
		_Out_writes_bytes_to_opt_(*pcbData, *pcbData) PVOID pvData,
		_Inout_opt_ LPDWORD pcbData
	) {
		LSTATUS ret = OldRegGetValue(hkey, lpSubKey, lpValue, dwFlags,
			pdwType, pvData, pcbData);
		printf("Hook API: RegGetKeyValue\n");
		
		printTime();
		printf("hKey: %#p\n", hkey);
		printf("lpSubKey: %ls\n", lpSubKey);
		printf("lpValueName: %ls\n", lpValue);
		printf("dwType: %lu\n", dwFlags);
		printf("pdwType: %#p\n", pdwType);
		if (pdwType) 
			printf("dwType: %lu\n", *pdwType);

		printf("pcbData: %#p\n", pcbData);
		if (pcbData)
			printf("cbData: %lu\n", *pcbData);
		printf("$HOOKEND$");

		return ret;
	}
}
