#pragma warning(default: 4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/type.h>
#include<sys/stat.h>

#include<WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
int startup(u_short *port);


// ÍøÂç³õÊ¼»¯
int startup(u_short *port)
{
	WSAData wsadata;
	int ret = WSAStartup(MAKEWORD(1, 1), &wsadata);
	int server_sock = socket();
	return 0;
}

int main()
{
	u_short port = 8080;
	return 0;
}