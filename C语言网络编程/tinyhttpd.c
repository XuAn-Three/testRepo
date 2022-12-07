#pragma warning(default: 4996) /* 忽视4996的警告 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>

#include<WinSock2.h>
#pragma comment(lib, "WS2_32.lib")

// 相关宏定义
#define PRINTF(str) printf("[%s - %d]"#str" = %s\r\n", __func__, __LINE__, str);
#define ISspace(x) isspace((int)(x))
#define SERVER_STRING "Server: Xuan3httpd/0.1.0\r\n"

// 函数原型定义
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


// 函数细节实现
void cat(int sock, FILE *resource) {
	// 发送资源
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
	printf("发送了 %d 个字节的内容\n", count);
}

void headers(int sock, const char *path) {
	// 发送响应头信息
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
	// 处理动态请求
}

void not_found(int sock) {
	// 没找到资源
	printf("啊哦！没找到文件~\n");
}

void server_file(int sock, const char *path) {
	// 发送普通文件
	FILE *resource = NULL;
	int numchars = 1;
	char buf[1024];

	// 解析剩下的请求头内容
	while ((numchars > 0) && strcmp("\n", buf))
	{
		numchars = get_line(sock, buf, sizeof(buf));
		PRINTF(buf);
	}

	// 打开文件
	if (strcmp(path, "res/index.html") == 0) // 如果是html文件以文本打开
		resource = fopen(path, "r");
	else
		resource = fopen(path, "rb"); // 其他的以二进制方式打开

	if (resource == NULL)
		not_found(sock);
	else
	{
		headers(sock, path);
		cat(sock, resource);
	}
	fclose(resource); // 关闭文件
}

int get_line(int sock, char* buf, int size)
{
	// 解析请求头信息
	int i = 0;
	char c = '\0';
	int n;

	while ((i < size - 1) && (c != '\n'))
	{
		n = recv(sock, &c, 1, 0); // 每次接收客户端发送过来的一个字符给buf
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
	// 请求方法错误
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
	// 接受请求后的处理
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
	// 判断当前 buf 的字符不为空，且 i 不大于 method 的长度
	while (!ISspace(buf[j]) && (i < sizeof(method) - 1))
	{
		method[i] = buf[j];
		i++; j++;
	}
	method[j] = '\0';
	PRINTF(method);

	// 请求方法判断
	// 同时满足不是 GET 和 POST 请求，发送相关内容
	if (stricmp(method, "GET") && stricmp(method, "POST"))
	{
		unimplemented(client);
		return;
	}
	if (stricmp(method, "POST") == 0)
		cgi = 1;

	i = 0;
	// 判断buf字符当前是否是空格，且j比numchars小
	while (ISspace(buf[j]) && (j < numchars))
		j++;
	// 判断buf字符当前不为空，url长度未满，j小于nuchars
	while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < numchars))
	{
		// 添加字符到 url
		url[i] = buf[j];
		i++; j++;
	}
	url[i] = '\0';
	PRINTF(url);

	// 检测请求方法是否为 GET
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
	// 拼装访问路径
	sprintf(path, "res%s", url);

	if (path[strlen(path) - 1] == '/')
		strcat(path, "index.html"); // 如果最后一个是 /，自动拼接index.html文件
	PRINTF(path);

	if (stat(path, &st) == -1)
	{
		while ((numchars > 0) && (strcmp("\n", buf)))
			numchars = get_line(client, buf, sizeof(buf));
		not_found(client); //没找到资源
	}
	else {
		if ((st.st_mode & S_IFMT) == S_IFDIR)
			strcat(path, "/index.html"); // 如果是目录，自动拼接

		if (!cgi)
			// 发送一个普通文件给客户端
			server_file(client, path);
		else
			// 处理动态请求
			execute_cgi(client, path, method, query_string);
	}
	closesocket(client); // 切断客户端的请求
}

void error_die(const char *sc)
{
	perror(sc);
	exit(1);
}

int startup(u_short *port)
{
	// 网络初始化
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (ret != 0) // 检验网络初始化是否成功
		return FALSE;
	int serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSock == -1) error_die("socket"); // 检验 serverSock 是否创建成功
	struct sockaddr_in serverAddr; // 初始化服务器地址信息
	memset(&serverAddr, 0, sizeof(serverAddr)); // 设置每个成员的初始值为 0
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(*port);
	serverAddr.sin_addr.s_addr = htons(INADDR_ANY);

	// 端口复用
	int opt = 1; // 设置状态
	ret = setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
	if (ret == -1) error_die("setsockopt");
	// 绑定检测
	if (bind(serverSock, (const struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		error_die("bind");
	// 动态分配端口
	if (*port == 0)
	{
		int addrLen = sizeof(serverAddr);
		if (getsockname(serverSock, (struct sockaddr *)&serverAddr, &addrLen) == -1)
			error_die("getsockname");
		*port = htons(serverAddr.sin_port);
	}
	// 监听检测
	if (listen(serverSock, 5) < 0)
		error_die("listen");
	return serverSock;
}

int main()
{
	u_short port = 8080; // 定义初识端口
	int serverSock = startup(&port); // 获取服务器的套接字
	printf("服务器已启动，开始监听端口: %d...\n", port);

	while (1)
	{
		// 创建客户端信息
		struct sockaddr_in clientAddr;
		int clientAddrLen = sizeof(clientAddr);
		//获取客户端的套接字
		int clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientAddrLen);

		if (clientSock == -1)
			error_die("accept");
		// 创建线程
		DWORD dwThreadID = 0;
		HANDLE handleFirst = CreateThread(NULL, 0, accept_request, (void*)clientSock, 0, &dwThreadID);
	}

	closesocket(serverSock);
	return 0;
}