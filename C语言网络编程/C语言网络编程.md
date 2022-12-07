## C语言网络编程

### WSADATA

- W - Windows
- S - Socket
- A - Asynchronous 异步

一种数据结构，用来存储被 `WSAStartup` 函数调用后返回的 Windows Sockets 数据。

包含了 WinSock.dll 执行的数据。

```c
// WinSock2.h
typedef struct WSAData {
        WORD                    wVersion;
        WORD                    wHighVersion;
#ifdef _WIN64
        unsigned short          iMaxSockets;
        unsigned short          iMaxUdpDg;
        char FAR *              lpVendorInfo;
        char                    szDescription[WSADESCRIPTION_LEN+1];
        char                    szSystemStatus[WSASYS_STATUS_LEN+1];
#else
        char                    szDescription[WSADESCRIPTION_LEN+1];
        char                    szSystemStatus[WSASYS_STATUS_LEN+1];
        unsigned short          iMaxSockets;
        unsigned short          iMaxUdpDg;
        char FAR *              lpVendorInfo;
#endif
} WSADATA, FAR * LPWSADATA;
```

- wVersion - Windows Sockets DLL 期望调用者使用的 WIndows Sockets 规范的版本。高位字节存储副版本号，低位字节存储主版本号，可以用 `WORD MAKEWORD(BYTE, BYTE)` 返回这个值。例如：`MAKEWORD(1, 1)`。
- wHighVersion - 该 DLL 能够支持的 Windows Sockets 规范的最高版本。通常它与 wVersion 相同。
- szDescription - 以 null 结尾的 ASCII 字符串，Windows Sockets DLL 将对 Windows Sockets 实现的描述拷贝到这个字符串中，包括制造商标识。文本（最多可以有256个字符）可以包含任何字符，但要注意不能包含控制字符和格式字符，应用程序对其最可能的使用方式是把他（可能被截断）显示在状态信息中。
- szSystemStatus - 以 null 结尾的 ASCII 字符串，Windows Sockets DLL 把有关的状态或配置信息拷贝到该字符串中。Windows Sockets DLL 应当仅在这些信息对用户或支持人员有用时才是用他们，它不应被作为 szDescription 域的扩展。
- IMAXSockets - 单个进程能够打开的 sockets 的最大数目。Windows Sockets的实现能提供一个全局的socket池，可以为任何进程分配；或者它也可以为socket分配属于进程的资源。这个数字能够很好地反映Windows Sockets DLL或网络软件的配置方式。应用程序的编写者可以通过这个数字来粗略地指明Windows Sockets的实现方式对应用程序是否有用。例如，X Windows服务器在第一次启动的时候可能会检查iMaxSockets的值：如果这个值小于8，应用程序将显示一条错误信息，指示用户重新配置网络软件（这是一种可能要使用szSystemStatus文本的场合）。显然无法保证某个应用程序能够真正分配iMaxSockets个socket，因为可能有其它WindowsSockets应用程序正在使用。
- IMAXUdpDG - Windows Sockets应用程序能够发送或接收的最大的用户数据包协议（UDP）的数据包大小，以字节为单位。如果实现方式没有限制，那么iMaxUdpDg为零。在Berkeley sockets的许多实现中，对于UDP数据包有个固有的限制（在必要时被分解），大小为8192字节。Windows Sockets的实现可以对碎片重组缓冲区的分配作出限制。对于适合的WindowsSockets 实现，iMaxUdpDg的最小值为512。注意不管iMaxUdpDg的值是什么，都不推荐你发回一个比网络的最大传送单元（MTU）还大的广播数据包。（Windows Sockets API 没有提供发现MTU的机制，但是它不会小于512个字节)。WinSock2.0版中已被废弃。
- IpVendorInfo - 指向销售商的数据结构的指针。

#### WSAStartup

- Startup

**函数原型**

```c
int WSAAPI WSAStartup(
    _In_ WORD wVersionRequested,
    _Out_ LPWSADATA lpWSAData
    );
```

**参数**

- WORD WVersionRequested - 调用者可以使用的 WIndows Sockets 规范的最高版本。高位字节指定次要版本号，低位字节指定主要版本号。
- LPWSADATA lpWSAData - 指向 WSADATA 数据结构的指针，该数据结构将接收 Windows Sockets 实现的详细信息。

**返回值**

- int 类型

如果成功，则 `WSAStartup` 函数将返回 0。否则，将返回下面的错误代码。

- WSA SYSNO TREADY - 基础网络子系统尚未准备好进行网络通信。
- WSA VERNO TSUPPORTED - 此特定的 Windows Socket 实现未提供所请求的 WIndows Sockets 支持的版本。
- WSA EINPROGRESS - Windows Sockets 1.1 的阻止操作正在进行中。
- WSA EPROCLIM - WIndows 套接字实现所支持的任务数已达到限制。
- WSA EFAULT  - IpWSAData 参数不是有效的指针。



### 套接字 socket

socket 位于应用层 与 TCP/IP 协议通信中间，抽象成一组接口。

#### socket

**作用**

用来创建一个 socket 描述符，socket 的唯一标识。

**函数原型**

```c
SOCKET WSAAPI socket(
    _In_ int af,
    _In_ int type,
    _In_ int protocol
    );
```

**参数**

- af - 协议组
  - PF_INET - IPv4
  - PF_INET6 - IPv6

- type - socket 类型
  - SOCK_STREAM - 流套接字
  - SOCK_DGRAM - 数据报套接字
- protocol - 协议
  - IPPROTO_TCP - TCP 协议
  - IPPROTO_UDP - UDP 协议

#### struct sockaddr_in

用来处理网络通信地址。

**结构**

```c
typedef struct sockaddr_in {

#if(_WIN32_WINNT < 0x0600)
    short   sin_family;
#else //(_WIN32_WINNT < 0x0600)
    ADDRESS_FAMILY sin_family;
#endif //(_WIN32_WINNT < 0x0600)

    USHORT sin_port;
    IN_ADDR sin_addr;
    CHAR sin_zero[8];
} SOCKADDR_IN, *PSOCKADDR_IN;
```

**成员**

- sin_family - 地址族（Address Family)
- sin_port - 16 位 TCP/UDP 端口号
- sin_addr - 32 位 IP 地址
- sin_zero[8] - 不使用



#### setsockopt

在 TCP 连接中，recv 等函数默认为阻塞模式（block），即直到有数据到来之前函数不会返回，而我们有时候需要一种超时机制使其在一定时间后返回而不管是否有数据到来。

**函数原型**

```c
int
WSAAPI
setsockopt(
    _In_ SOCKET s,
    _In_ int level,
    _In_ int optname,
    _In_reads_bytes_opt_(optlen) const char FAR * optval,
    _In_ int optlen
    );
```

**参数**

- s - 套接字描述符。
- level - 被设置的选项的级别。
  - SOL_SOCKET
- optname - 指定准备设置的选项
  - SO_REUSEADDR



#### bind

**作用**

**函数原型**

```c
bind(
    _In_ SOCKET s,
    _In_reads_bytes_(namelen) const struct sockaddr FAR * name,
    _In_ int namelen
    );
```



#### getsockname

**作用**

**函数原型**

```c
getsockname(
    _In_ SOCKET s,
    _Out_writes_bytes_to_(*namelen,*namelen) struct sockaddr FAR * name,
    _Inout_ int FAR * namelen
    );
```

#### listen

**作用**

**函数原型**

```c
listen(
    _In_ SOCKET s,
    _In_ int backlog
    );
```



### 线程 Thread

#### CreateThread

**作用**

创建一个在调用进程的虚拟地址空间内执行的线程。

**函数原型**

```c
HANDLE
WINAPI
CreateThread(
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ SIZE_T dwStackSize,
    _In_ LPTHREAD_START_ROUTINE lpStartAddress,
    _In_opt_ __drv_aliasesMem LPVOID lpParameter,
    _In_ DWORD dwCreationFlags,
    _Out_opt_ LPDWORD lpThreadId
    );
```

**参数**

- lpThreadAttributes - 指向 SECURITY_ATTRIBUTES 结构的指针，该结构确定紫禁城是否可以继续返回的句柄。为NULL，则无法继承句柄。
- dwStackSize - 堆栈的初始大小，以字节为单位。系统将该值四舍五入到最近的页面。如果参数为 0，则新线程使用可执行文件的默认大小。
- lpStartAddress - 指向由线程执行的应用程序定义函数的指针，该指针表示线程的起始地址。
- lpParameter - 指向传递给线程的变量的指针。
- dwCreationFlags - 控制线程创建的标志。
  - 0 - 该线程在创建后立即运行。
  - CREATE_SUSPENDEDs - 线程是在挂起状态下创建的，并且在调用 `ResumeThread` 函数之前不会运行。
  - STACK_SIZE_PARAM_IS_A_RESERVATION - 所述 dwStackSize 参数所指定堆栈的初始保留大小。如果未指定此标志，则 dwStackSize 指定提交大小。
- lpThread - 指向接收线程标识符的变量的指针。如果此参数为 NULL，则不返回线程标识符。





### 文件操作 File

#### fopen

#### fread

#### fclose

