#include <WinSock2.h>
#include <iostream>
using namespace std;

int main()
{
	cout<<"TCP client"<<endl;
		/*
	加载套接字库
	int WSAStarup(
		WORD wVersionRequested,
		LPWSADATA lpWSAData
	);
	wVersionRequested参数用于指定准备加载的Winsock库的版本
	高位字节指定所需要的WinSock库的副版本，而低位字节则是主版本
	可用MAKEWORD(x,y)(其中，x是高位字节，y是低位字节)方便地获得wVersionRequested的正确值

	lpWSAData参数是指向WSADATA结构的指针，WSAStartup用其加载的库版本有关的信息填在这个结构体中

	struct WSAData {
	WORD wVersion;//打算使用的Winsock版本
	WORD wHighVersion;//现有Winsock库的最高版本
	char szDescription[WSADESCRIPTION_LEN+1];
	char szSystemStatus[WSASYSSTATUS_LEN+1];
	unsigned short iMaxSockets;//最大可以打开过少套接字
	unsigned short iMaxUdpDg;//数据包的最大长度
	char FAR * lpVendorInfo;//为Winsock实施方案有关的指定厂商信息预留的
	};

	若WinSock.dll或者底层网络子系统没有被正确初始化或没有被找到，WSAStarup将返回WSASYSNOTREADY.

	对于每个WSAStarup的成功调用（成功加载WinSock DLL后），在最后都对应一个WSACleanUp调用，以便释放为该应用程序分配的资源
	*/

	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) { 
		printf("WSAStartup failed with error: %d\n", err);
		return 1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		printf("Could not find a usable version of Winsock.dll\n");
		WSACleanup();
		return 1;
	}

	//创建套接字
	SOCKET socketClient=socket(AF_INET,SOCK_STREAM,0);

	//客户端不需要绑定bind
	//链接服务器端
	/*
	int connect(
	SOCKET s,
	const struct sockaddr FAR*name,
	int namelen
	);
	*/
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(6000);

	connect(socketClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	//接收
	char recvBuf[100]={0};
	recv(socketClient,recvBuf,100,0);
	cout<<"recv : "<<recvBuf<<endl;

	//发送
	send(socketClient,"This is a message send from client.",strlen("This is a message send from client.")+1,0);
	closesocket(socketClient);
	WSACleanup();
	return 0;
}