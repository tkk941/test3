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
	//q启动window socket 2.x环境tttt
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//---------------
	//1建立一个socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//2bind绑定用于接受客户端连接的网络端口
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(CLIENT_PORT);//host to nwt unsigned short
	_sin.sin_addr.S_un.S_addr = INADDR_ANY;//inet_addr("CLIENT_ADDR");
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(_sin))) {
		printf("ERROR,绑定用于接受客户端连接的网络端口失败... \n"); 
	} else {
		printf("绑定网络端口成功... \n");
	}
	//3listen监听网络端口
	if (SOCKET_ERROR == listen(_sock, LISTEN_NET_NUM)) {
		printf("ERROR,监听网络端口失败... \n");
	} else {
		printf("监听网络端口成功... \n");
	}
	while (true) {
		//伯克利套接字 BSD socket
		fd_set fdRead;//描述符（socket）集合
		fd_set fdWrite;
		fd_set fdExp;
		//清理集合
		FD_ZERO(&fdRead);
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExp);
		//将描述符（socket）加入集合
		FD_SET(_sock, &fdRead);
		FD_SET(_sock, &fdWrite);
		FD_SET(_sock, &fdExp);

		for (int n = (int)g_clients.size()-1; n >= 0; n--) {
			FD_SET(g_clients[n], &fdRead);
		}
		//nfds 是一个整数值，是指fd_set集合中所有描述符（socket）的范围，而不是数量
		//既是所有文件描述最大值+1，在windows中这个参数可以写0
		timeval t = {1,0};
		int ret = select(_sock + 1, &fdRead, &fdWrite, &fdExp, &t);
		if (ret < 0) {
			printf("select任务结束。 \n");
			break;
		}
		//判断描述符（socket）是否在集合中
		if (FD_ISSET(_sock, &fdRead)) {
			FD_CLR(_sock, &fdRead);
			//4accept等待接收客户端连接
			sockaddr_in clientAddr = { };
			int nAddrLen = sizeof(sockaddr_in);
			SOCKET _cSock = INVALID_SOCKET;
			_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
			if (INVALID_SOCKET == _cSock) {
				printf("ERROR,接收到无效客户端SOCKET... \n");
			} else {
				for (int n = (int)g_clients.size() - 1; n >= 0; n--) {
					NewUserJoin userJoin;
					send(g_clients[n], (const char*)&userJoin, sizeof(NewUserJoin), 0);
				}
				g_clients.push_back(_cSock);
				printf("新客户端加入：socket =%d ,IP =%s  \n", (int)_cSock, inet_ntoa(clientAddr.sin_addr));
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
		printf("空闲时间处理其它业务...\n");
	}
	for (int n =(int) g_clients.size() - 1; n >= 0; n--) {
		closesocket(g_clients[n]);
	}
	//8 关闭套接字closesocket
	closesocket(_sock);
	//---------------
	//清除windows sock环境
	WSACleanup();
	printf("已退出，任务结束。\n");
	return 0;
}