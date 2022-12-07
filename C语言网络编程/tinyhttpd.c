#pragma warning(default: 4996) /* ����4996�ľ��� */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>

#include<WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

// ��غ궨��
#define PRINTF(str) printf("[%s - %d]"#str" = %s\r\n", __func__, __LINE__, str);
#define ISspace(x) isspace((int)(x))
#define SERVER_STRING "Server: Xuan3httpd/0.1.0\r\n"

// ����ԭ�Ͷ���
int startup(u_short *port);
void error_die(const char *sc);
DWORD WINAPI accept_request(LPVOID arg);
int get_line(int sock, char* buf, int size);
void unimplemented(int sock);
void not_found(int sock);
void server_file(int sock, const char *path);
void execute_cgi(int sock, const char *path, const char *method, const char *str);
void headers(int sock, const char *path);
void cat(int sock, FILE *resource);


// ����ϸ��ʵ��
void cat(int sock, FILE *resource) {
	// ������Դ
	char buf[1024];
	int count = 0;
	/*fgets(buf, sizeof(buf), resource);
	while (!feof(resource))
	{
		count += sizeof(buf);
		send(sock, buf, strlen(buf), 0);
		fgets(buf, sizeof(buf), resource);
	}*/
	while (1)
	{
		int ret = fread(buf, sizeof(char), sizeof(buf), resource);
		if (ret <= 0)
			break;
		send(sock, buf, ret, 0);
		count += ret;
	}
	printf("������ %d ���ֽڵ�����\n", count);
}

void headers(int sock, const char *path) {
	// ������Ӧͷ��Ϣ
	char buf[1024];
	(void)path;

	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(sock, buf, strlen(buf), 0);
	strcpy(buf, SERVER_STRING);
	send(sock, buf, strlen(buf), 0);
	strcpy(buf, "Content-Type: text/html\r\n");
	send(sock, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(sock, buf, strlen(buf), 0);
}

void execute_cgi(int sock, const char *path, const char *method, const char *str) {
	// ����̬����
}

void not_found(int sock) {
	// û�ҵ���Դ
	printf("��Ŷ��û�ҵ��ļ�~\n");
}

void server_file(int sock, const char *path) {
	// ������ͨ�ļ�
	FILE *resource = NULL;
	int numchars = 1;
	char buf[1024];

	// ����ʣ�µ�����ͷ����
	while ((numchars > 0) && strcmp("\n", buf))
	{
		numchars = get_line(sock, buf, sizeof(buf));
		PRINTF(buf);
	}

	// ���ļ�
	if (strcmp(path, "res/index.html") == 0) // �����html�ļ����ı���
		resource = fopen(path, "r");
	else
		resource = fopen(path, "rb"); // �������Զ����Ʒ�ʽ��

	if (resource == NULL)
		not_found(sock);
	else
	{
		headers(sock, path);
		cat(sock, resource);
	}
	fclose(resource); // �ر��ļ�
}

int get_line(int sock, char* buf, int size)
{
	// ��������ͷ��Ϣ
	int i = 0;
	char c = '\0';
	int n;

	while ((i < size - 1) && (c != '\n'))
	{
		n = recv(sock, &c, 1, 0); // ÿ�ν��տͻ��˷��͹�����һ���ַ���buf
		if (n > 0)
		{
			if (c == '\r')
			{
				n = recv(sock, &c, 1, MSG_PEEK);
				if ((n > 0) && (c == '\n'))
					recv(sock, &c, 1, 0);
				else
					c = '\n';
			}
			buf[i] = c;
			i++;
		}
		else c = '\n';
	}
	buf[i] = '\0';

	return i;
}

void unimplemented(int sock)
{
	// ���󷽷�����
	char buf[1024];

	sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, SERVER_STRING);
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "</TITLE></HEAD>\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>HTTP request method not unsupoorted.\r\n");
	send(sock, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>");
	send(sock, buf, strlen(buf), 0);
}

DWORD WINAPI accept_request(LPVOID arg)
{
	// ���������Ĵ���
	int client = (SOCKET)arg;
	char buf[1024];
	size_t numchars;
	char method[255];
	char url[255];
	char path[255];
	size_t i, j;
	struct stat st;
	int cgi = 0;

	char *query_string = NULL;

	numchars = get_line(client, buf, sizeof(buf));
	i = 0, j = 0;
	// �жϵ�ǰ buf ���ַ���Ϊ�գ��� i ������ method �ĳ���
	while (!ISspace(buf[j]) && (i < sizeof(method) - 1))
	{
		method[i] = buf[j];
		i++; j++;
	}
	method[j] = '\0';
	PRINTF(method);

	// ���󷽷��ж�
	// ͬʱ���㲻�� GET �� POST ���󣬷����������
	if (stricmp(method, "GET") && stricmp(method, "POST"))
	{
		unimplemented(client);
		return;
	}
	if (stricmp(method, "POST") == 0)
		cgi = 1;

	i = 0;
	// �ж�buf�ַ���ǰ�Ƿ��ǿո���j��numcharsС
	while (ISspace(buf[j]) && (j < numchars))
		j++;
	// �ж�buf�ַ���ǰ��Ϊ�գ�url����δ����jС��nuchars
	while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < numchars))
	{
		// ����ַ��� url
		url[i] = buf[j];
		i++; j++;
	}
	url[i] = '\0';
	PRINTF(url);

	// ������󷽷��Ƿ�Ϊ GET
	if (stricmp(method, "GET") == 0)
	{
		query_string = url;
		while ((*query_string != '?') && (*query_string != '\0'))
			query_string++;
		if (*query_string == '?')
		{
			cgi = 1;
			*query_string = '\0';
			query_string++;
		}
	}
	// ƴװ����·��
	sprintf(path, "res%s", url);

	if (path[strlen(path) - 1] == '/')
		strcat(path, "index.html"); // ������һ���� /���Զ�ƴ��index.html�ļ�
	PRINTF(path);

	if (stat(path, &st) == -1)
	{
		while ((numchars > 0) && (strcmp("\n", buf)))
			numchars = get_line(client, buf, sizeof(buf));
		not_found(client); //û�ҵ���Դ
	}
	else {
		if ((st.st_mode & S_IFMT) == S_IFDIR)
			strcat(path, "/index.html"); // �����Ŀ¼���Զ�ƴ��

		if (!cgi)
			// ����һ����ͨ�ļ����ͻ���
			server_file(client, path);
		else
			// ����̬����
			execute_cgi(client, path, method, query_string);
	}
	closesocket(client); // �жϿͻ��˵�����
}

void error_die(const char *sc)
{
	perror(sc);
	exit(1);
}

int startup(u_short *port)
{
	// �����ʼ��
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (ret != 0) // ���������ʼ���Ƿ�ɹ�
		return FALSE;
	int serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSock == -1) error_die("socket"); // ���� serverSock �Ƿ񴴽��ɹ�
	struct sockaddr_in serverAddr; // ��ʼ����������ַ��Ϣ
	memset(&serverAddr, 0, sizeof(serverAddr)); // ����ÿ����Ա�ĳ�ʼֵΪ 0
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(*port);
	serverAddr.sin_addr.s_addr = htons(INADDR_ANY);

	// �˿ڸ���
	int opt = 1; // ����״̬
	ret = setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
	if (ret == -1) error_die("setsockopt");
	// �󶨼��
	if (bind(serverSock, (const struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		error_die("bind");
	// ��̬����˿�
	if (*port == 0)
	{
		int addrLen = sizeof(serverAddr);
		if (getsockname(serverSock, (struct sockaddr *)&serverAddr, &addrLen) == -1)
			error_die("getsockname");
		*port = htons(serverAddr.sin_port);
	}
	// �������
	if (listen(serverSock, 5) < 0)
		error_die("listen");
	return serverSock;
}

int main()
{
	u_short port = 8080; // �����ʶ�˿�
	int serverSock = startup(&port); // ��ȡ���������׽���
	printf("����������������ʼ�����˿�: %d...\n", port);

	while (1)
	{
		// �����ͻ�����Ϣ
		struct sockaddr_in clientAddr;
		int clientAddrLen = sizeof(clientAddr);
		//��ȡ�ͻ��˵��׽���
		int clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientAddrLen);

		if (clientSock == -1)
			error_die("accept");
		// �����߳�
		DWORD dwThreadID = 0;
		HANDLE handleFirst = CreateThread(NULL, 0, accept_request, (void*)clientSock, 0, &dwThreadID);
	}

	closesocket(serverSock);
	return 0;
}