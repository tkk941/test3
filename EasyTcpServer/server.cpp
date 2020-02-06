#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<windows.h>
#include<winsock2.h>
#include<stdio.h>
#include"server.h"
#include<vector>

#pragma comment(lib, "ws2_32.lib")

std::vector<SOCKET> g_clients;

bool Processor(SOCKET _cSock);

int main() {
	//q����window socket 2.x����tttt
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//---------------
	//1����һ��socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//2bind�����ڽ��ܿͻ������ӵ�����˿�
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(CLIENT_PORT);//host to nwt unsigned short
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;//inet_addr("CLIENT_ADDR");
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(_sin))) {
		printf("ERROR,�����ڽ��ܿͻ������ӵ�����˿�ʧ��... \n"); 
	} else {
		printf("������˿ڳɹ�... \n");
	}
	//3listen��������˿�
	if (SOCKET_ERROR == listen(_sock, LISTEN_NET_NUM)) {
		printf("ERROR,��������˿�ʧ��... \n");
	} else {
		printf("��������˿ڳɹ�... \n");
	}
	while (true) {
		//�������׽��� BSD socket
		fd_set fdRead;//��������socket������
		fd_set fdWrite;
		fd_set fdExp;
		//������
		FD_ZERO(&fdRead);
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExp);
		//����������socket�����뼯��
		FD_SET(_sock, &fdRead);
		FD_SET(_sock, &fdWrite);
		FD_SET(_sock, &fdExp);

		for (int n = (int)g_clients.size()-1; n >= 0; n--) {
			FD_SET(g_clients[n], &fdRead);
		}
		//nfds ��һ������ֵ����ָfd_set������������������socket���ķ�Χ������������
		//���������ļ��������ֵ+1����windows�������������д0
		timeval t = {1,0};
		int ret = select(_sock + 1, &fdRead, &fdWrite, &fdExp, &t);
		if (ret < 0) {
			printf("select��������� \n");
			break;
		}
		//�ж���������socket���Ƿ��ڼ�����
		if (FD_ISSET(_sock, &fdRead)) {
			FD_CLR(_sock, &fdRead);
			//4accept�ȴ����տͻ�������
			sockaddr_in clientAddr = { };
			int nAddrLen = sizeof(sockaddr_in);
			SOCKET _cSock = INVALID_SOCKET;
			_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
			if (INVALID_SOCKET == _cSock) {
				printf("ERROR,���յ���Ч�ͻ���SOCKET... \n");
			} else {
				for (int n = (int)g_clients.size() - 1; n >= 0; n--) {
					NewUserJoin userJoin;
					send(g_clients[n], (const char*)&userJoin, sizeof(NewUserJoin), 0);
				}
				g_clients.push_back(_cSock);
				printf("�¿ͻ��˼��룺socket =%d ,IP =%s  \n", (int)_cSock, inet_ntoa(clientAddr.sin_addr));
			}
		}
		for (size_t n = 0; n < fdRead.fd_count; n++) {
	     	if (Processor(fdRead.fd_array[n]) == false) {
				auto iter = find(g_clients.begin(), g_clients.end(), fdRead.fd_array[n]);
				if (iter != g_clients.end()) {
					g_clients.erase(iter);
				}
			}
		}	
		printf("����ʱ�䴦������ҵ��...\n");
	}
	for (int n =(int) g_clients.size() - 1; n >= 0; n--) {
		closesocket(g_clients[n]);
	}
	//8 �ر��׽���closesocket
	closesocket(_sock);
	//---------------
	//���windows sock����
	WSACleanup();
	printf("���˳������������\n");
	return 0;
}