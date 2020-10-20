#include "serverHead.h"
serverSocket::serverSocket(int port)
{
	//初始化套结字动态库
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		std::cout << "WSAStartup failed!" << std::endl;
		return;
	}

	//开始创建服务端socket
	//创建套接字  AF_INET：ipv4   SOCK_STREAM：使用tcp
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sServer)
	{
		std::cout << "socket failed!" << std::endl;
		return;
	}

	//服务器套接字地址   
	addrServ.sin_family = AF_INET;//IPv4 
	addrServ.sin_port = htons(port);//设置端口 建议大于1024
	addrServ.sin_addr.s_addr = INADDR_ANY; //表示接受任何客户端的请求

	//绑定套接字  绑定服务端socket 和 端口
	int ret = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == ret)
	{
		std::cout << "bind failed!" << std::endl;
		return;
	}

	//开始监听   
	ret = listen(sServer, 10);
	if (SOCKET_ERROR == ret)
	{
		std::cout << "listen failed!" << std::endl;
		return;
	}
}

serverSocket::~serverSocket()
{
	closesocket(sServer);   //关闭套接字  
	WSACleanup();           //释放套接字资源;  
}

SOCKET serverSocket::serverAccpetSocket()
{
	//接受客户端请求  
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
