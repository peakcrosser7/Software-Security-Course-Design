#include <Windows.h>

int main() {
	HKEY hKey;
	auto ret = RegCreateKey(HKEY_CURRENT_USER, L"TEST_KEY", &hKey);
	if (ret != ERROR_SUCCESS) {
		//printf("RegCreate Fail\n");
		return 0;
	}
	//printf("Create Key Success\n");
	DWORD dwValue = 10;
	if (RegSetKeyValue(hKey, L"mySubKey", L"myValueName", REG_DWORD,
		&dwValue, sizeof(dwValue)) == ERROR_SUCCESS) {
		//printf("Set Key Success\n");
	}
	DWORD dwType;
	DWORD data;
	DWORD cbData = sizeof(data);
	if (RegGetValue(hKey, L"mySubKey", L"myValueName", RRF_RT_REG_DWORD,
		&dwType, &data, &cbData) == ERROR_SUCCESS) {
		//printf("Get Key Success\n");
		//cout << "Value: " << data << endl;
	}
	auto res = RegDeleteTree(hKey, L"mySubKey");
	if (RegCloseKey(hKey) != ERROR_SUCCESS) {
		//printf("Close Key Fail\n");
		return 0;
	}
	res = RegDeleteKey(HKEY_CURRENT_USER, L"TEST_KEY");
	if (res == ERROR_SUCCESS) {
		//printf("Delete Key Success\n");
	}

	WCHAR value[] = L"./testReg";
	if (RegSetKeyValue(HKEY_CURRENT_USER,
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"
		, L"Everything", REG_SZ, value, sizeof(value))) {
		//printf("Fail");
		return 0;
	}
	HKEY self;
	RegOpenKey(HKEY_CURRENT_USER,
		L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		&self);
	if (RegDeleteValue(self, L"Everything") != ERROR_SUCCESS) {
		//printf("del fail");
	}
	return 0;
}