#include "serverHead.h"
unsigned __int64 client[10][2];
char onlineName[10][20];
char onlineQQID[10][20];
int clientNum;
int main()
{
	serverSocket mySocket(8888);
	cout << "*******----����������----*******" << endl;
	cout << "�˿ں�8888��ʼ����" << endl;
	while (1)
	{
		SOCKET sClient = mySocket.serverAccpetSocket();
		if (sClient == -1)
		{
			continue;
		}
		else
		{
			client[clientNum][0] = sClient;
			clientNum++;
			std::cout << "���յ�һ���ͻ��� :" << sClient << std::endl;
		}
		std::thread t1(clientSocketThreadFunction, sClient);//�����߳�
		t1.detach();
	}
	return 0;
}

