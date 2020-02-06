#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include<windows.h>
#include<winsock2.h>
#include<stdio.h>
#include"client.h"

#pragma comment(lib, "ws2_32.lib")

bool Processor(SOCKET _cSock);

int main() {
	//q启动window socket 2.x环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//1建立一个socket
	SOCKET _sock = socket(AF_INET,SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock) {
		printf("ERROR,建立socket失败... \n");
	} else {
		printf("建立成功... \n");
	}
	//2 连接服务器 connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(CLIENT_PORT);
	_sin.sin_addr.S_un.S_addr = inet_addr(CLIENT_ADDR);
	int ret = connect(_sock,(sockaddr*)&_sin,sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret) {
		printf("错误，连接服务器失败 \n");
	} else {
		printf("连接服务器成功 \n");
	}
	while (true) {
		fd_set fdReads;
		FD_ZERO(&fdReads);
		FD_SET(_sock, &fdReads);
		timeval t = {1,0};
		int ret = select(_sock, &fdReads, 0, 0, &t);
		if (ret < 0) {
			printf("select任务结束1\n");
			break;
		}
		if (FD_ISSET(_sock, &fdReads)) {
			FD_CLR(_sock, &fdReads);
			
			if (Processor(_sock) == false) {
				printf("select任务结束2\n");
				break;
			}
		}
		printf("空闲时间处理其它业务...\n");
		Login login;
		strcpy(login.userName, "tkk");
		strcpy(login.passWord, "tkkmm");
		send(_sock, (const char*)&login, sizeof(Login), 0);
		//Sleep(1000);
	}
	//7 关闭套接字closesocket
	closesocket(_sock);
	//清除window socket环境
	WSACleanup();
	printf("已退出。\n");
	return 0;
}