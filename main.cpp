#include "serverHead.h"
unsigned __int64 client[10][2];
char onlineName[10][20];
char onlineQQID[10][20];
int clientNum;
int main()
{
	serverSocket mySocket(8888);
	cout << "*******----服务器启动----*******" << endl;
	cout << "端口号8888开始监听" << endl;
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
			std::cout << "接收到一个客户端 :" << sClient << std::endl;
		}
		std::thread t1(clientSocketThreadFunction, sClient);//启用线程
		t1.detach();
	}
	return 0;
}

