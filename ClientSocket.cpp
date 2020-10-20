#include"clientHead.h"
clientSocket::clientSocket(string ipAdress, short port)
{
	//��ʼ���׽��ֶ�̬��  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return;
	}
	//�����׽���  
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sClient)
	{
		cout << "socket failed!" << endl;
		return;
	}
	servAddr.sin_family = AF_INET;
	//�������ͨ���� ���� c++ ����  sdl �ĳɷ�
	servAddr.sin_addr.s_addr = inet_addr(ipAdress.c_str());//���÷���˵�ַ  �����ʾ����
	servAddr.sin_port = htons(port);
	int nServAddlen = sizeof(servAddr);
}

bool clientSocket::connectToServer()
{
	//���ӷ�����  
	ret = connect(sClient, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == ret)
	{
		cout << "connect failed!" << endl;
		system("pause");
		return false;
	}
	//�ɹ���������  ���Կ�ʼͨ����
	return true;
}

void clientSocket::sendData(message & Data)
{
	//���������������  
	ret = send(sClient, (char*)&Data, sizeof(Data), 0);
	if (SOCKET_ERROR == ret)
	{
		cout << "send failed!" << endl;
		return;
	}
}

void clientSocket::receiveData(message & Data)
{
	recv(sClient, (char*)&Data, sizeof(Data), 0);
}

clientSocket::~clientSocket()
{
	closesocket(sClient);	//�ر��׽���  
	WSACleanup();			//�ͷ��׽�����Դ  
}