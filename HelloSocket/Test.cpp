#define WIN32_LEAN_AND_MEAN

#include<windows.h>
#include<winsock2.h>

#pragma comment(lib, "ws2_32.lib")
int main()
{
	//q启动window socket 2.x环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//---------------
	//--用socket API建立简易TCP客户端
	//1建立一个socket
	//2 连接服务器 connect
	//3接收服务器信息recv
	//4关闭套接字closesocket
	//--用Socket API建立简易TCP服务端
	//1建立一个socket
	//2bind绑定用于接受客户端连接的网络端口
	//3listen监听网络端口




	WSACleanup();
	return 0;
}