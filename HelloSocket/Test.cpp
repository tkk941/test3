#define WIN32_LEAN_AND_MEAN

#include<windows.h>
#include<winsock2.h>

#pragma comment(lib, "ws2_32.lib")
int main()
{
	//q����window socket 2.x����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	WSAStartup(ver, &dat);
	//---------------
	//--��socket API��������TCP�ͻ���
	//1����һ��socket
	//2 ���ӷ����� connect
	//3���շ�������Ϣrecv
	//4�ر��׽���closesocket
	//--��Socket API��������TCP�����
	//1����һ��socket
	//2bind�����ڽ��ܿͻ������ӵ�����˿�
	//3listen��������˿�




	WSACleanup();
	return 0;
}