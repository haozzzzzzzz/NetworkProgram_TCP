#include <WinSock2.h>
#include <iostream>
using namespace std;

int main()
{
	cout<<"TCP server"<<endl;
	/*
	�����׽��ֿ�
	int WSAStarup(
		WORD wVersionRequested,
		LPWSADATA lpWSAData
	);
	wVersionRequested��������ָ��׼�����ص�Winsock��İ汾
	��λ�ֽ�ָ������Ҫ��WinSock��ĸ��汾������λ�ֽ��������汾
	����MAKEWORD(x,y)(���У�x�Ǹ�λ�ֽڣ�y�ǵ�λ�ֽ�)����ػ��wVersionRequested����ȷֵ

	lpWSAData������ָ��WSADATA�ṹ��ָ�룬WSAStartup������صĿ�汾�йص���Ϣ��������ṹ����

	struct WSAData {
	WORD wVersion;//����ʹ�õ�Winsock�汾
	WORD wHighVersion;//����Winsock�����߰汾
	char szDescription[WSADESCRIPTION_LEN+1];
	char szSystemStatus[WSASYSSTATUS_LEN+1];
	unsigned short iMaxSockets;//�����Դ򿪹����׽���
	unsigned short iMaxUdpDg;//���ݰ�����󳤶�
	char FAR * lpVendorInfo;//ΪWinsockʵʩ�����йص�ָ��������ϢԤ����
	};

	��WinSock.dll���ߵײ�������ϵͳû�б���ȷ��ʼ����û�б��ҵ���WSAStarup������WSASYSNOTREADY.

	����ÿ��WSAStarup�ĳɹ����ã��ɹ�����WinSock DLL�󣩣�����󶼶�Ӧһ��WSACleanUp���ã��Ա��ͷ�Ϊ��Ӧ�ó���������Դ
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
		int af,//ָ����ַ�壬����TCP/IPЭ����׽��֣���ֻ����AF_INET��Ҳ����д��PF_INET��
		int type,//ָ��Socket���ͣ�����1.1�汾��Socket,��ֻ֧���������͵��׽��֣�SOCK_STREAMָ��������ʽ�׽��֣�SOCKET_DGRAM�������ݱ��׽���
		int protocol//���ض��ĵ�ַ��������Э�飬���ָ��Ϊ0����ô���ͻ���ݵ�ַ��ʽ���׽�������Զ�ѡ��һ�����ʵ�Э��
		);

		�������������óɹ�����������һ���µ�SOCKET�������͵��׽������������������ʧ�ܣ���������ͻ᷵��һ��INVALID_SOCKET,������Ϣ����ͨ��WSAGetLastError�������ء�
		*/
	
	//�����׽���
	SOCKET sockSrv=socket(AF_INET,SOCK_STREAM,0);

	//���׽��ְ󶨱��ص�ַ�Ͷ˿���
	/*
	int bind(
	SOCKET s, //ָ��Ҫ�󶨵��׽���
	const struct sockaddr FAR *name,//ָ���˸��׽��ֵı��ص�ַ��Ϣ����ָ��sockaddr�ṹ��ָ����������ڸõ�ַ�ṹ��Ϊ���еĵ�ַ����׼���ģ�����ṹ��������ʹ�õ�����Э�鲻ͬ����ͬ������Ҫӿ������������ָ���õ�ַ�ṹ�ĳ���
	int namelen
	);
	struct sockaddr
	{
		u_short sa_family,//ָ���õ�ַ���� ������ΪAF_INET
		char sa_data[14]
	};
	��TCP/IP�У����ǿ�����sockaddr_in�ṹ�滻sockaddr,�Է���������д��ַ��Ϣ
	struct sockaddr_in
	{
		short sin_family,//��ַ�� ����IP��ַ��sin_family��Ա��һֱ��AF_INET
		unsigned short sin_port,//��Ҫ������׽��ֵĶ˿ڡ�
		struct in_addr sin_addr,//�׽���������ip��ַ INADDR_ANY
		char sin_zero[8]//�����
	}
	*/

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);// u_long htonl�������ֽ���TCP/IP�����ֽ���
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(6000);//��1024���ϵĶ˿ں� ת��short

	//��
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));

	//����
	/*
	int listen
	{
		SOCKET s,
		int backlog//�ȴ����������  ΪSOMAXCONN ������������ֵ
	}
	*/
	listen(sockSrv,5);

	//ѭ���ȴ�
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
			int flags//ָʾsend����Ϊ
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

		closesocket(sockConn);//�ر��׽���
	}
	WSACleanup();
	return 0;
};