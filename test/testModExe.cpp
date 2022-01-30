#include <windows.h>

int main() {
	HANDLE hFile = CreateFile(L".\\modexe.exe", GENERIC_READ | GENERIC_WRITE,
		NULL, NULL, OPEN_ALWAYS, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		hFile = NULL;
		return 0;
	}

	//Ð´ÎÄ¼þ
	char buf[128] = "abcd";
	DWORD written;
	WriteFile(hFile, buf, 4, &written, NULL);
	CloseHandle(hFile);
	return 0;
}