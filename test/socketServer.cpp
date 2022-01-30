#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{

	/*初始化套接字库*/
	//WinSock版本
	WORD wVersionRequested = MAKEWORD(2, 2);		//将两个字节组合成1个字节, 表示版本号
	WSADATA wsaData;	//套接字信息
	int err = WSAStartup(wVersionRequested, &wsaData);		//加载套接字库
	if (err)
	{
		//cout << "Load Socket Failed!\n";
		return 0;
	}
	//判断加载的套接字版本是否符合
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return 0;
	}

	/*创建套接字*/
	SOCKET socketServer = socket(AF_INET, SOCK_STREAM, 0);		//服务器套接字
	//SOCK_STREAM表示流式套接字(对应TCP)
	if (socketServer == INVALID_SOCKET)
	{
		//cout << "Socket ERROR!";
		return 0;
	}

	/*绑定套接字到本地地址和端口*/
	//SOCKADDR_IN用于socket定义和赋值；SOCKADDR用于函数参数, 都为存储IP地址的结构体
	SOCKADDR_IN serverAddr;		//服务端地址信息结构
	serverAddr.sin_family = AF_INET;	//IPV4网址
	serverAddr.sin_addr.S_un.S_addr = htons(INADDR_ANY);	//0.0.0.0  任意地址
	serverAddr.sin_port = htons(5555);		//端口号
	if (bind(socketServer, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		//cout << "Bind Failed!\n";
		//cout << GetLastError() << endl;
		closesocket(socketServer);
		return 0;
	}
	else
		//cout << "Bind!\n";

	/*设置套接字为监听状态*/
	if (listen(socketServer, 5) < 0)	//最大等待数目为5
		//cout << "Listen Failed!\n";
	else
		//cout << "Listen!\n";

	/*接受连接*/
	SOCKADDR_IN clientAddr;		//客户端地址信息结构
	int cliAddrLen = sizeof(SOCKADDR);		//表示SOCKADDR大小

	//接受客户端的发送请求, 等待客户端connect请求
	SOCKET socketRecv = accept(socketServer, (SOCKADDR*)&clientAddr, &cliAddrLen);		//用于连接的套接字
	if (socketRecv == SOCKET_ERROR)
		//cout << "Receive Connect Failed!\n";
	else
		//cout << "Connect!\n";

	const int  bufSize = 50;	//缓冲区大小
	char sendBuf[bufSize]="This is the Server Msg";		//发送数据缓冲区
	char recvBuf[bufSize];		//结束数据缓冲区

	/*接收数据*/
		//从面向连接的套接字中接收数据
	//cout << "Waiting for data...\n";
	int recvLen = recv(socketRecv, recvBuf, bufSize, 0);
	if (recvLen <= 0) {
		//cout << "Receive Failed!\n Exit!\n";
	}
	else
		//cout << "Client say:" << recvBuf << endl;
	memset(recvBuf, 0, bufSize);

	/*发送数据*/
	int sendLen = send(socketRecv, sendBuf, bufSize, 0);	//在面向连接方式的套接字间发送数据
	if (sendLen < 0)
		//cout << "Send Failed!";
	else
		//cout << "Send :" << sendBuf;
	memset(sendBuf, 0, bufSize);

	//关闭套接字
	closesocket(socketRecv);
	closesocket(socketServer);
	//关闭链接库
	WSACleanup();

	return 0;
}
