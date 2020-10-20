#include "serverHead.h"
extern unsigned __int64 client[10][2];
extern char onlineName[10][20];
extern char onlineQQID[10][20];
extern int clientNum;
/*启用线程  一个线程对应一个客户端*/
void clientSocketThreadFunction(SOCKET sClient)
{
	Friend_mysql Friend;
	Group_mysql  Group;
	User_mysql	 User;
	mgc Data;
	//checkuser
	while (true)
	{
		int ret = recv(sClient, (char*)&Data, sizeof(Data), 0);
		if (SOCKET_ERROR == ret)
		{
			perror("recv");
			std::cout << sClient << "错误：可能下线" << std::endl;
			return;
		}
		switch (User.login(Data))
		{
			//登陆成功
		case 1:cout << Data.ID<<"登陆成功" << endl; 
			Data.ret = 1;
			send(sClient,(char*)&Data,sizeof(Data), 0);
			break;

			//账号不存在
		case 2:cout << "账号不存在" << endl;
			Data.ret = 2;
			send(sClient, (char*)&Data, sizeof(Data), 0);
			break;

			//密码错误
		case 3:cout << "密码错误" << endl; 
			Data.ret = 3;
			send(sClient, (char*)&Data, sizeof(Data), 0);
			break;

		default:cout << "未知错误" << endl; exit(-1);
		}
		//登陆成功就跳出循环，否则再次登陆
		if (Data.ret == 1)
		{
			break;
		}
		else
		{
			continue;
		}

	}
	//从数据库中查询数据，帮助客户端构造MyQQ
	constructMyX(sClient, Friend, Group, User);
	//从数据库中查询数据，帮助客户端构造MyWeChat
	constructMyX(sClient, Friend, Group, User);
	//从数据库中查询数据，帮助客户端构造MyWeBo
	constructMyX(sClient, Friend, Group, User);

	//对客户端的数据进行解析
	while (true)
	{
		cout << "主菜单等待指令中。。。" << endl;
		int ret = recv(sClient, (char*)&Data, sizeof(Data), 0);
		if (SOCKET_ERROR == ret)
		{
			std::cout << sClient << "可能下线" << std::endl;
			return;
		}
		int flag = 0;
		switch (Data.action)
		{
			//进入QQ
		case 1:cout << "客户端进入QQ" << endl;
			strcpy_s(Data.table,"qq");
			recommendFriend(sClient,Friend,Data);
			menu(sClient, Friend, Group, Data);
			break;
			//进入WeChat
		case 2: cout << "客户端进入微信" << endl;
			strcpy_s(Data.table, "wc");
			recommendFriend(sClient, Friend, Data);
			menu(sClient, Friend, Group, Data);
			break;
			//进入WeBo
		case 3:cout << "客户端进入微博" << endl;
			strcpy_s(Data.table, "wb");
			recommendFriend(sClient, Friend, Data);
			menu(sClient, Friend, Group, Data);
			break;
			//打印我的信息
		case 4: break;		
			//退出flag
		case 9: flag = 1; break;		
		default:std::cout << "输入错误 请重新输入：" << std::endl;
		}
		if (flag == 1)
		{
			std::cout << "客户端 " << sClient << " 下线" << std::endl;
			break;
		}
	}
	
}
/*从数据库获取微X平台的账号信息，为客户端微X对象构造做准备*/
void constructMyX(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, User_mysql & User)
{
	mgc Data;
	//user 构造部分
	recv(sClient, (char *)&Data, sizeof(Data), 0);
	if (Data.action == 10)
	{
		cout << "My"<< Data.table <<" - user is constructing" << endl;	
		//表示批准构造操作
		Data.action = 1;
		//初始化User 数据包
		User.returnInfo(Data);
		send(sClient, (char *)&Data, sizeof(Data), 0);
	}
	else
	{
		cout << "user construct error!" << endl;
		exit(-1);
	}

	//微X构造部分
	recv(sClient, (char *)&Data, sizeof(Data), 0);
	if (Data.action == 11)
	{
		cout <<Data.table<<" is constructing" << endl;
		//表示批准构造操作
		Data.action = 1;
		Friend.returnDetails(Data);
		Friend.returnInfo(Data);
		Group.returnInfo(Data);
		send(sClient, (char *)&Data, sizeof(Data), 0);

	}
	else
	{
		cout << "My"<< Data.table <<" construct error" << endl;
		exit(-1);

	}

}
/*服务器与客户端同步进入菜单*/
void menu(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data)
{
	//子菜单循环,需要手动返回上一级
	cout << "客户端进入"<<Data.table<<"菜单" << endl;
	while (true)
	{
		recv(sClient, (char *)&Data, sizeof(Data), 0);		//1.标记
		send(sClient, (char *)&Data, sizeof(Data), 0);		//2.标记
		switch (Data.action)
		{
		case 1://1.显示我的信息 按任意键继续
			break;
		case 2://1.设置好友备注/2.聊天/3.查看好友信息/4.删除好友
			cout << "客户端选择了好友" << endl;
			chooseFriend(sClient, Friend, Group, Data);
			break;
		case 3://1.退出群/2.推荐给好友/3.踢人/4.查询成员/5.升级群
			cout << "客户端选择了群组" << endl;
			chooseGroup(sClient, Friend, Group, Data);
			break;
		case 4://1.输入 X账号 直接添加成功
			cout << "客户端选择了添加好友" << endl;
			addFriend(sClient, Friend, Group, Data);
			break;
		case 5://1.输入 X群账号 直接添加成功
			if (!strcmp(Data.table, "qq"))
			{
				cout << "客户端选择了添加群组" << endl;
				addGroup(sClient, Friend, Group, Data);
			}
			else
			{
				cout << "客户端选择了推荐群聊给好友" << endl;
				recommendGroup(sClient, Friend, Group, Data);
			}
			break;
		case 6://打印好友列表 //1.访问内存 直接打印（非详细）
			cout << "客户端选择了打印好友列表" << endl;
			break;
		case 7://打印群组列表 //1.访问内存 直接打印（非详细）
			cout << "客户端选择了打印群组列表" << endl;
			break;
		case 8://创建群聊
			cout << "客户选择了创建群聊" << endl;
			creatGroup(sClient, Friend, Group, Data);
			break;
		case 9://返回上一级
			cout << "客户端选择了退出" << endl;
			return;
		default:
			cout << "错误的指令" << endl;
			break;
		}
	}
}
/*QQ的专属功能，服务器充当中继站为两个客户端发送实时信息*/
void chatToFriend(SOCKET sClient, SOCKET sClient_2, message & Data)
{
	while (Data.action != -1)
	{
		recv(sClient, (char *)&Data, sizeof(Data), 0);
		send(sClient_2, (char *)&Data, sizeof(Data), 0);
	}
}
/*服务器进入选择好友界面*/
void chooseFriend(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data)
{
	while (true)
	{
		recv(sClient, (char *)&Data, sizeof(Data), 0);		//1.对应标记
		send(sClient, (char *)&Data, sizeof(Data), 0);
		switch (Data.action)
		{
		case 1:
			recv(sClient, (char *)&Data, sizeof(Data), 0);
			if (Data.action != -1)
			{
				if (Friend.setNicknameToFriend(Data))
				{
					Data.action = 1;
					send(sClient, (char *)&Data, sizeof(Data), 0);
					break;
				}
				else
				{
					Data.action = -1;
					send(sClient, (char *)&Data, sizeof(Data), 0);
					break;
				}
			}
			else
			{
				break;
			}

		case 2://2.聊天
			if (strcmp("qq", Data.table))break;
			Data.FriendNum = clientNum;
			//向客户端发送当前在线用户
			for (int i = 0; i < clientNum; i++)
			{
				cout << "onlineQQID第" << i <<"位："<< onlineQQID[i] << endl;
				strcpy_s(Data.FriendList[i], onlineQQID[i]);
				strcpy_s(Data.FriendNickName[i], onlineName[i]);
			}
			send(sClient, (char *)&Data, sizeof(Data), 0);
			recv(sClient, (char *)&Data, sizeof(Data), 0);
			if (1 == Data.action)
			{
				chatToFriend(sClient,client[Data.ret][0],Data);
				cout << "一位用户已退出聊天" << endl;
			}
			else
			{
				cout << "客户端选择的好友不合法" << endl;
			}
			break;
		case 3://3.查看好友信息
			recv(sClient, (char *)&Data, sizeof(Data), 0);
			if (Friend.findFriend(Data))
			{
				Data.action = 1;
				send(sClient, (char *)&Data, sizeof(Data), 0);
			}
			else
			{
				Data.action = -1;
				send(sClient, (char *)&Data, sizeof(Data), 0);
			}
			break;
		case 4:
			recv(sClient, (char *)&Data, sizeof(Data), 0);
			if (Data.action != -1)
			{
				if (Friend.deleteFriend(Data))
				{
					Data.action = 1;
					send(sClient, (char *)&Data, sizeof(Data), 0);				
				}
				else
				{
					Data.action = -1;
					send(sClient, (char *)&Data, sizeof(Data), 0);
				}
			}
			break;
		case 9://返回上一级
			return;
		default:
			cout << "chooseFriend..客户端输入的指令有误....." << endl;
			break;
		}
	}
}
/*服务器进入选择群组界面*/
void chooseGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data)
{
	while (true)
	{
		int ret;
		recv(sClient, (char *)&Data, sizeof(Data), 0);		//1.对应标记
		send(sClient, (char *)&Data, sizeof(Data), 0);
		switch (Data.action)
		{
		case 1://退出群聊
			recv(sClient, (char *)&Data, sizeof(Data), 0);		//2.对应标记
			if (1 == Data.action) {
				if (Group.dropGroup(Data))
				{
					Data.action = 1;
					send(sClient, (char *)&Data, sizeof(Data), 0);
				}
				else
				{
					Data.action = -1;
					send(sClient, (char *)&Data, sizeof(Data), 0);
				}
			}
			else
			{
				cout << "客户端取消请求 '退出群聊' " << endl;
			}
			break;
		case 2://推荐给好友
			cout << "微信群 推荐功能正在开发中..." << endl;
			break;
		case 3://踢人	
			recv(sClient, (char *)&Data, sizeof(Data), 0);
			ret = Group.kickGroupMember(Data);
			if (1 == ret)//成功踢出
			{
				Data.action = 1;
				send(sClient, (char *)&Data, sizeof(Data), 0);
			}
			//后期再细化
			else if(-1 == ret)//没查到这个人
			{
				Data.action = -1;
				send(sClient, (char *)&Data, sizeof(Data), 0);
			}
			else if (0 == ret)//第位太低
			{
				Data.action = 0;
				send(sClient, (char *)&Data, sizeof(Data), 0);
			}
			else 
			{
				Data.action = -2;
				send(sClient, (char *)&Data, sizeof(Data), 0);
			}
			break;
		case 4://查询群成员
			recv(sClient, (char *)&Data, sizeof(Data), 0);
			Group.listMember(Data);
			send(sClient, (char *)&Data, sizeof(Data), 0);
			break;
		case 5://升级群
			if (strcmp("qq", Data.table))break;
			recv(sClient, (char *)&Data, sizeof(Data), 0);
			Group.levelUpGroup(Data);
			send(sClient, (char *)&Data, sizeof(Data), 0);
			break;
		case 6://创建子群
			if (strcmp("qq", Data.table))break;
			recv(sClient, (char *)&Data, sizeof(Data), 0);
			Data.action = Group.creatTempGroup(Data);
			send(sClient, (char *)&Data, sizeof(Data), 0);
			break;
		case 9://返回上一级
			cout << "客户端返回了"<<Data.table<<"菜单" << endl;
			return;
		default:
			cout << "chooseGroup..客户端输入的指令有误....." << endl;
			break;
		}
	}


}
/*服务器进入添加好友界面*/
void addFriend(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data)
{
	recv(sClient, (char *)&Data, sizeof(Data), 0);			//1.标记
	if (1 == Data.action)
	{
		if (Friend.addFriend(Data))
		{
			Data.action = 1;
			send(sClient, (char *)&Data, sizeof(Data), 0);	//2.标记
		}
		else
		{
			Data.action = -1;
			send(sClient, (char *)&Data, sizeof(Data), 0);	//2.标记
		}
	}
	else
	{
		cout << "客户端取消请求 '添加好友' " << endl;
	}
}
/*服务器进入添加群组界面*/
void addGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data)
{
	recv(sClient, (char *)&Data, sizeof(Data), 0);			//1.标记
	if (1 == Data.action)
	{
		if (Group.addGroup(Data))
		{
			Data.action = 1;
			send(sClient, (char *)&Data, sizeof(Data), 0);	//2.标记
		}
		else
		{
			Data.action = -1;
			send(sClient, (char *)&Data, sizeof(Data), 0);	//2.标记
		}
	}
	else
	{
		cout << "客户端取消请求 '添加群聊' " << endl;
	}
}
/*服务器进入创建群聊界面*/
void creatGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data)
{
	recv(sClient, (char *)&Data, sizeof(Data), 0);
	if (Group.creatGroup(Data))
	{
		Data.action = 1;
		send(sClient, (char *)&Data, sizeof(Data), 0);
	}
	else
	{
		Data.action = -1;
		send(sClient, (char *)&Data, sizeof(Data), 0);
	}
}
/*服务器根据数据库中的完全信息为客户端反馈不同平台间的推荐好友*/
void recommendFriend(SOCKET sClient, Friend_mysql & Friend, message & Data)
{
	while (1)
	{
		recv(sClient, (char *)&Data, sizeof(Data), 0);
		if (Data.action == 1)return;
		else if (Data.action == 3)
		{
			if (Friend.isExist(Data))
			{
				Data.action = 4;
				send(sClient, (char *)&Data, sizeof(Data), 0);
			}
			else
			{
				Data.action = 2;
				send(sClient, (char *)&Data, sizeof(Data), 0);
			}
		}
	}
}
/*微信和微博的推荐好友加入群*/
void recommendGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data)
{
	recv(sClient, (char *)&Data, sizeof(Data), 0);
	if (-1 == Data.action)
	{
		cout << "客户端取消推荐群，服务器自动返回上一层" <<endl;
		return;
	}
	if (1 == Friend.findFriend(Data))
	{
		if (1 == Group.addGroup(Data))
		{
			Data.action = 1;
			send(sClient, (char *)&Data, sizeof(Data), 0);
		}
		else
		{
			Data.action = 3;
			send(sClient, (char *)&Data, sizeof(Data), 0);
		}
	}
	else
	{
		Data.action = 2;
		send(sClient, (char *)&Data, sizeof(Data), 0);
	}
}