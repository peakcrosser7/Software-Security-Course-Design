#define _CRT_SECURE_NO_WARNINGS	
#include <windows.h>

int main() {
	HANDLE hFile = CreateFile(L".\\hello.txt", GENERIC_READ | GENERIC_WRITE,
		NULL, NULL, OPEN_ALWAYS, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		hFile = NULL;
		//printf("Cannot open file\n");
		return 0;
	}
	//cout << "Open success!\n";

	//写文件
	char buf[128] = "abcd";
	DWORD written;
	if (WriteFile(hFile, buf, 4, &written, NULL) == FALSE) {
		//cout << "WriteFail\n";
	}

	//读文件
	char buffer[128];
	DWORD readSize;
	if (ReadFile(hFile, buffer, 4, &readSize, NULL) == FALSE) {
		//cout << "ReadFail\n";
	}


	//自我复制
	wchar_t src[MAX_PATH];	//缓冲区
	wchar_t dest[MAX_PATH];
	//获取程序本身的路径
	GetModuleFileName(NULL, src, MAX_PATH);
	wchar_t tmp[MAX_PATH];
	int id;
	//提取后缀名
	for (id = wcslen(src) - 1; id >= 0 && src[id] != L'.'; --id);
	wcsncpy(tmp, src + id, MAX_PATH);
	//提取文件名
	wcsncpy(dest, src, id);
	wsprintf(dest + id, L"cp%d%s", 0, tmp);
	CopyFile(src, dest, FALSE);		// 复制文件
	CloseHandle(hFile);


	//删除文件
	DeleteFile(L".\\hello.txt");
	auto err = GetLastError();
	return 0;
}