#include"clientHead.h"
clientSocket::clientSocket(string ipAdress, short port)
{
	//初始化套结字动态库  
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return;
	}
	//创建套接字  
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sClient)
	{
		cout << "socket failed!" << endl;
		return;
	}
	servAddr.sin_family = AF_INET;
	//如果编译通不过 属性 c++ 常规  sdl 改成否
	servAddr.sin_addr.s_addr = inet_addr(ipAdress.c_str());//设置服务端地址  这里表示本机
	servAddr.sin_port = htons(port);
	int nServAddlen = sizeof(servAddr);
}

bool clientSocket::connectToServer()
{
	//连接服务器  
	ret = connect(sClient, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == ret)
	{
		cout << "connect failed!" << endl;
		system("pause");
		return false;
	}
	//成功建立连接  可以开始通信了
	return true;
}

void clientSocket::sendData(message & Data)
{
	//向服务器发送数据  
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
	closesocket(sClient);	//关闭套接字  
	WSACleanup();			//释放套接字资源  
}