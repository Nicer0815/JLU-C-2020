#include "serverHead.h"
serverSocket::serverSocket(int port)
{
	//��ʼ���׽��ֶ�̬��
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		std::cout << "WSAStartup failed!" << std::endl;
		return;
	}

	//��ʼ���������socket
	//�����׽���  AF_INET��ipv4   SOCK_STREAM��ʹ��tcp
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		std::cout << "socket failed!" << std::endl;
		return;
	}

	//�������׽��ֵ�ַ   
	addrServ.sin_family = AF_INET;//IPv4 
	addrServ.sin_port = htons(port);//���ö˿� �������1024
	addrServ.sin_addr.s_addr = INADDR_ANY; //��ʾ�����κοͻ��˵�����

	//���׽���  �󶨷����socket �� �˿�
	int ret = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == ret)
	{
		std::cout << "bind failed!" << std::endl;
		return;
	}

	//��ʼ����   
	ret = listen(sServer, 10);
	if (SOCKET_ERROR == ret)
	{
		std::cout << "listen failed!" << std::endl;
		return;
	}
}

serverSocket::~serverSocket()
{
	closesocket(sServer);   //�ر��׽���  
	WSACleanup();           //�ͷ��׽�����Դ;  
}

SOCKET serverSocket::serverAccpetSocket()
{
	//���ܿͻ�������  
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	temp = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
	if (INVALID_SOCKET == temp)
	{
		std::cout << "accept failed!" << std::endl;
		return -1;
	}
	//listScli.push_front(temp);
	return temp;
}
