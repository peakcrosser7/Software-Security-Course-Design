#include <windows.h>

int main() {
	MessageBoxW(nullptr, L"MsgBoxW", L"hello", NULL);
	MessageBoxA(nullptr, "MsgBoxA", "hi", NULL);
	return 0;
}