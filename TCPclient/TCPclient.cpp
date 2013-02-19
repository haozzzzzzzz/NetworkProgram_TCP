#include <WinSock2.h>
#include <iostream>
using namespace std;

int main()
{
	cout<<"TCP client"<<endl;
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

	//�����׽���
	SOCKET socketClient=socket(AF_INET,SOCK_STREAM,0);

	//�ͻ��˲���Ҫ��bind
	//���ӷ�������
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

	//����
	char recvBuf[100]={0};
	recv(socketClient,recvBuf,100,0);
	cout<<"recv : "<<recvBuf<<endl;

	//����
	send(socketClient,"This is a message send from client.",strlen("This is a message send from client.")+1,0);
	closesocket(socketClient);
	WSACleanup();
	return 0;
}