#include <windows.h>
#include <iostream>
using namespace std;

int main() {
	HANDLE h = HeapCreate(NULL, 0, 0);
	if (h) HeapDestroy(h);

	PHANDLE ph = (PHANDLE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PHANDLE));
	HeapFree(GetProcessHeap(), NULL, (LPVOID)ph);
	HeapFree(GetProcessHeap(), NULL, (LPVOID)ph);
	return 0;
}