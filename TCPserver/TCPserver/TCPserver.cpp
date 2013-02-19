#include <WinSock2.h>
#include <iostream>
using namespace std;

int main()
{
	cout<<"TCP server"<<endl;
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
	/*
	SOCKET socket(
		int af,//指定地址族，对于TCP/IP协议的套接字，它只能是AF_INET（也可以写成PF_INET）
		int type,//指定Socket类型，对于1.1版本的Socket,它只支持两种类型的套接字，SOCK_STREAM指定产生流式套接字，SOCKET_DGRAM产生数据报套接字
		int protocol//与特定的地址家族的相关协议，如果指定为0，那么它就会根据地址格式和套接字类别，自动选择一个合适的协议
		);

		如果这个函数调用成功，它将返回一个新的SOCKET数据类型的套接字描述符。如果调用失败，这个函数就会返回一个INVALID_SOCKET,错误信息可以通过WSAGetLastError函数返回。
		*/
	
	//创建套接字
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);

	//将套接字绑定本地地址和端口上
	/*
	int bind(
	SOCKET s, //指定要绑定的套接字
	const struct sockaddr FAR *name,//指定了该套接字的本地地址信息，是指向sockaddr结构的指针变量，由于该地址结构是为所有的地址家族准备的，这个结构可能随所使用的网络协议不同而不同，所以要涌动第三个参数指定该地址结构的长度
	int namelen
	);
	struct sockaddr
	{
		u_short sa_family,//指定该地址家族 这里设为AF_INET
		char sa_data[14]
	};
	在TCP/IP中，我们可以用sockaddr_in结构替换sockaddr,以方便我们填写地址信息
	struct sockaddr_in
	{
		short sin_family,//地址族 对于IP地址，sin_family成员将一直是AF_INET
		unsigned short sin_port,//将要分配给套接字的端口。
		struct in_addr sin_addr,//套接字主机的ip地址 INADDR_ANY
		char sin_zero[8]//填充数
	}
	*/

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);// u_long htonl从主机字节序到TCP/IP网络字节序
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(6000);//用1024以上的端口号 转换short

	//绑定
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	//监听
	/*
	int listen
	{
		SOCKET s,
		int backlog//等待队列最大数  为SOMAXCONN 则设置最大合理值
	}
	*/
	listen(sockSrv,5);

	//循环等待
	/*
		SOCKET accept
		(
			SOCKET s,
			struct sockaddr FAR*addr,
			int FAR*addrlen
		);

		SOCKET send
		{
			const char FAR*buf,
			int len,
			int flags//指示send的行为
		};

		int recv(
		SOCKET s,
		char FAR*buf,
		int len,
		int flags
		);
	*/
	SOCKADDR_IN addrClient;
	int len=sizeof(SOCKADDR);
	while(1)
	{
		SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);

		char sendBuf[100]={0};
		sprintf(sendBuf,"Welcome %s .This is a message send from server",inet_ntoa(addrClient.sin_addr));
		send(sockConn,sendBuf,strlen(sendBuf)+1,0);

		char recvBuf[100]={0};
		recv(sockConn,recvBuf,100,0);
		cout<<"recv : "<<recvBuf<<endl;

		closesocket(sockConn);//关闭套接字
	}
	WSACleanup();
	return 0;
};