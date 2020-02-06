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
	//q����window socket 2.x����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//1����һ��socket
	SOCKET _sock = socket(AF_INET,SOCK_STREAM, 0);
	if (INVALID_SOCKET == _sock) {
		printf("ERROR,����socketʧ��... \n");
	} else {
		printf("�����ɹ�... \n");
	}
	//2 ���ӷ����� connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(CLIENT_PORT);
	_sin.sin_addr.S_un.S_addr = inet_addr(CLIENT_ADDR);
	int ret = connect(_sock,(sockaddr*)&_sin,sizeof(sockaddr_in));
	if (SOCKET_ERROR == ret) {
		printf("�������ӷ�����ʧ�� \n");
	} else {
		printf("���ӷ������ɹ� \n");
	}
	while (true) {
		fd_set fdReads;
		FD_ZERO(&fdReads);
		FD_SET(_sock, &fdReads);
		timeval t = {1,0};
		int ret = select(_sock, &fdReads, 0, 0, &t);
		if (ret < 0) {
			printf("select�������1\n");
			break;
		}
		if (FD_ISSET(_sock, &fdReads)) {
			FD_CLR(_sock, &fdReads);
			
			if (Processor(_sock) == false) {
				printf("select�������2\n");
				break;
			}
		}
		printf("����ʱ�䴦������ҵ��...\n");
		Login login;
		strcpy(login.userName, "tkk");
		strcpy(login.passWord, "tkkmm");
		send(_sock, (const char*)&login, sizeof(Login), 0);
		//Sleep(1000);
	}
	//7 �ر��׽���closesocket
	closesocket(_sock);
	//���window socket����
	WSACleanup();
	printf("���˳���\n");
	return 0;
}