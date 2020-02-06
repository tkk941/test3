#define CLIENT_ADDR "127.0.0.1"
#define CLIENT_PORT 5000
#define TRANS_BYTE_SIZE 128

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
		printf("��������Ͽ����ӣ���������� \n");
		return false;
	}
	switch (header->cmd) {
	    case CMD_LOGIN_RESULT: {
			recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
			LoginResult* login = (LoginResult*)szRecv;
			printf("�յ��������Ϣ:CMD_LOGIN_RESULT, ���ݳ��ȣ�%d\n",  header->dataLength);
	    }
	    break;
	    case CMD_LOGOUT_RESULT: {
			recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
			LogoutResult* logout = (LogoutResult*)szRecv;
			printf("�յ��������Ϣ:CMD_LOGOUT_RESULT,���ݳ��ȣ�%d\n", header->dataLength);
	    }
	    break;
	    case CMD_NEW_USER_JOIN: {
			recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
			NewUserJoin* userJoin = (NewUserJoin*)szRecv;
			printf("�յ��������Ϣ:CMD_NEW_USER_JOIN,���ݳ��ȣ�%d\n", header->dataLength);
	    }
	    break;
	    }
}