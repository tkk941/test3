#include<stdio.h>

#define CLIENT_ADDR "127.0.0.1"
#define CLIENT_PORT 5000
#define TRANS_BYTE_SIZE 128
#define LISTEN_NET_NUM 5

#pragma once

enum CMD {
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_NEW_USER_JOIN,
	CMD_ERROR
};
struct DataHeader {
	short dataLength;
	short cmd;
};
struct Login : public DataHeader {
	Login() {
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	// DataHeader INFO
	char userName[32];
	char passWord[32];
};
struct LoginResult : public DataHeader {
	LoginResult() {
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
};
struct Logout : public DataHeader {
	Logout() {
		dataLength = sizeof(Logout);
		cmd = CMD_LOGOUT;
	}
	char userName[32];
};
struct LogoutResult : public DataHeader {
	LogoutResult() {
		dataLength = sizeof(LogoutResult);
		cmd = CMD_LOGOUT_RESULT;
		result = 0;
	}
	int result;
};
struct NewUserJoin : public DataHeader {
	NewUserJoin() {
		dataLength = sizeof(NewUserJoin);
		cmd = CMD_NEW_USER_JOIN;
		scok = 0;
	}
	int scok;
};

bool Processor(SOCKET _cSock) {
	//������
	char szRecv[4096] = { };
	// 5 ���տͻ�������
	int nlen = recv(_cSock, szRecv, sizeof(DataHeader), 0);
	DataHeader* header = (DataHeader*)szRecv;
	if (nlen <= 0) {
		printf("�ͻ���<Socket = %d>���˳������������ \n",_cSock);
		return false;
	}
	switch (header->cmd) {
	case CMD_LOGIN: {
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		Login* login = (Login*)szRecv;
		printf("�յ��ͻ���<Socket = %d>����:CMD_LOGIN,, ���ݳ��ȣ�%d , userName = %s, passWord=%s\n", _cSock, login->dataLength, login->userName, login->passWord);
		//�����ж��û������Ƿ���ȷ�Ĺ���
		LoginResult ret;
		send(_cSock, (char*)&ret, sizeof(LoginResult), 0);
	}
	break;
	case CMD_LOGOUT: {
		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		Logout* logout = (Logout*)szRecv;
		printf("�յ��ͻ���<Socket = %d>����:CMD_LOGOUT,, ���ݳ��ȣ�%d , userName = %s\n", _cSock, logout->dataLength, logout->userName);
		//�����ж��û������Ƿ���ȷ�Ĺ���
		LogoutResult ret;
		send(_cSock, (char*)&ret, sizeof(ret), 0);
	}
	break;
	default: {
		DataHeader header = { 0,CMD_ERROR };
		send(_cSock, (char*)&header, sizeof(header), 0);
	}
	break;
	}
}