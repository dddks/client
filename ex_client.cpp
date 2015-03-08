#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define S_IP "127.0.0.1"
#define S_PORT 7203
#define B_SIZE 1024

void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);

}

void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);

}



int main()
{
	int check;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit("socket()");


	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(S_IP);
	serveraddr.sin_port = htons(S_PORT);
	check = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (check == SOCKET_ERROR)
		err_quit("connect()");


	char buf[B_SIZE + 1];
	int len;


	while (1)
	{
		printf("\n[보낼데이터] ");
		//enter입력 종료
		if (fgets(buf, B_SIZE + 1, stdin) == NULL)
			break;

		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		check = send(sock, buf, strlen(buf), 0);
		if (check == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		printf("[TCP클라이언트] %d바이트를 보냈습니다.\n", check);

		check = recv(sock, buf, check, 0);
		if (check == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		else if (check == 0)
		{
			break;
		}

		buf[check] = '\0';
		printf("[TCP클라이언트] %d바이트를 받았습니다. \n", check);
		printf("[받은데이터]%s\n", buf);
	}


	closesocket(sock);


	WSACleanup();

	return 0;

}