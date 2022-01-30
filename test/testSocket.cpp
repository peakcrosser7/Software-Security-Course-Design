#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")


int main() {
	/*初始化套接字库*/
	WORD wVersionRequested = MAKEWORD(2, 2);		//WinSock版本
	WSADATA wsaData;		//套接字信息
	int err = WSAStartup(wVersionRequested, &wsaData);	//加载套接字库
	if (err) {
		return 0;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		WSACleanup();
		return 0;
	}

	/*客户端创建套接字*/
	SOCKET socketSend = socket(AF_INET, SOCK_STREAM, 0);
	//客户端套接字不需要绑定
	if (socketSend == INVALID_SOCKET) {
		//cout << "Socket ERROR!";
		return 0;
	}

	/*连接服务器*/
	//设置服务器地址端口
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(5555);

	if (connect(socketSend, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		return 0;
	}
	const int bufSize = 50;
	char sendBuf[bufSize] = "74126985";		//发送数据缓冲区
	char recvBuf[bufSize];		//接受数据缓冲区

	int sendLen = send(socketSend, sendBuf, bufSize, 0);

	memset(sendBuf, 0, bufSize);

	int recvLen = recv(socketSend, recvBuf, bufSize, 0);
	memset(recvBuf, 0, bufSize);
	
	closesocket(socketSend);
	WSACleanup();
	return 0;
}