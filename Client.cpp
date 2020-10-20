#include"clientHead.h"
//登陆函数（检测总平台账号密码）
int initlogin(message & Data, message & rent, clientSocket & mySocket)
{
	cout << "请输入账号:";
	cin >> Data.ID;
	cout << "请输入密码:";
	cin >> Data.paswad;
	int i = sizeof(Data);
	mySocket.sendData(Data);
	cout << "initlogin请求已发送" << endl;
	mySocket.receiveData(Data);
	if (Data.ret == 1)
	{
		cout << Data.ID << "登陆成功" << endl;
		return 1;
	}
	else if (Data.ret == 2)
	{
		cout << Data.ID << "账号不存在" << endl;
		return 2;
	}
	else if (Data.ret == 3)
	{
		cout << Data.ID << "密码错误" << endl;
		return 3;
	}
	else
	{
		cout << Data.ret << endl;
		cout << "未知错误" << endl;

		exit(-1);
	}
}
//初始菜单
void menu()
{
	cout << "*****************************" << endl;
	cout << endl;
	cout << "1.进入QQ" << endl;
	cout << "2.进入WeChat" << endl;
	cout << "3.进入WeBo" << endl;
	cout << "9.退出" << endl;
	cout << endl;
	cout << "*****************************" << endl;
}
//微X菜单
void MyQQ::Xmenu()
{
	cout << "*****************************" << endl;
	cout << "1.我的信息" << endl;			//1.显示我的信息 按任意键继续
	cout << "2.选择好友" << endl;			//1.设置好友备注/2.聊天/3.查看好友信息
	cout << "3.选择群聊" << endl;			//1.退出群/2.推荐给好友/3.踢人/4.查询成员/5.升级群
	cout << "4.添加好友" << endl;			//1.输入QQ账号 直接添加成功
	cout << "5.添加群组" << endl;			//1.输入QQ群账号 直接添加成功
	cout << "6.打印好友列表" << endl;		//1.访问内存 直接打印（非详细）
	cout << "7.打印群组列表" << endl;		//1.访问内存 直接打印（非详细）
	cout << "8.创建群聊" << endl;
	cout << "9.退出" << endl;
	cout << "*****************************" << endl;

}
//命令解析
void user::action(message & Data, clientSocket & mySocket)
{
	//循环式界面，需要手动返回上一级
	while (true)
	{
		Xmenu();
		cout << "请输入操作：";
		cin >> Data.action;
		int temp = Data.action;
		mySocket.sendData(Data);				//1.标记
		cout << "正在请求。。。。。。" << endl;
		mySocket.receiveData(Data);				//2.标记
		//返回相同指令表示服务器批准操作
		if (temp == Data.action)
		{
			switch (Data.action)
			{
			case 1://1.显示我的信息 按任意键继续
				showMyInfo();
				break;
			case 2://选择好友 //1.设置好友备注/2.聊天/3.查看好友信息
				chooseFriend(Data, mySocket);
				break;
			case 3://选择群聊 //1.退出群/2.推荐给好友/3.踢人/4.查询成员/5.升级群
				chooseGroup(Data, mySocket);
				break;
			case 4://添加好友 //1.输入QQ账号 直接添加成功
				addFriend(Data, mySocket);
				break;
			case 5://添加群组 //1.输入QQ群账号 直接添加成功
				addGroup(Data, mySocket);
				break;
			case 6://打印好友列表 //1.访问内存 直接打印（非详细）
				showFriendList();
				break;
			case 7://打印群组列表 //1.访问内存 直接打印（非详细）
				showGroupList();
				break;
			case 8://创建群聊
				creatGroup(Data, mySocket);
				break;
			case 9://返回上一级
				return;
			default:
				cout << "指令错误" << endl;
				break;
			};

		}
		else
		{
			cout << "请求失败，强制下线" << endl;
			exit(-1);

		}

	}
}
//设置好友备注(修改好友信息)
int user::SetNickName(message & Data, clientSocket & mySocket)
{
	cout << "请输入好友"<<Data.table<<"账号:";
	cin >> Data.XID;
	cout << "设置好友备注：";
	cin >> Data.NickName;
	//内存中找到此人(判断是好友) 并且成功修改备注/昵称
	if (setFriendNickName(atoi(Data.XID), Data.NickName))
	{
		itoa(getXID(), Data.ID, 10);
		mySocket.sendData(Data);		//让服务器去改数据库
		mySocket.receiveData(Data);		//接收反馈
		if (1 == Data.action)
		{
			cout << "设置好友备注成功！" << endl;
			cout << "按任意键继续..." << endl;
			getchar(); getchar();
			return 1;
		}
		else
		{
			cout << "在数据库中设置备注失败" << endl;
			cout << "按任意键继续..." << endl;
			getchar(); getchar();
			return -1;
		}
	}
	else
	{
		//取消请求数据库
		Data.action = -1;
		mySocket.sendData(Data);
		cout << "输入"<< Data.table <<"账号有误，您还不是他的好友/该用户不存在" << endl;
		cout << "按任意键继续..." << endl;
		getchar(); getchar();
		return 0;
	}
}
//查找好友信息（打印）
void user::FindFriendInfo(message & Data, clientSocket & mySocket)
{
	cout << "请输入好友账号:";
	cin >> Data.XID;
	//myqq.Isfriend();
	mySocket.sendData(Data);
	mySocket.receiveData(Data);
	//请求合法
	if (1 == Data.action)
	{
		cout << ">------------------------------------<" << endl;
		cout << Data.table<<"名字：" << Data.Name << endl;
		cout << Data.table << "账号：" << Data.XID << endl;
		cout << Data.table << "龄：" << Data.TAge << endl;
		cout << "出生日期：" << Data.BirthDay << endl;
		cout << "所在地：" << Data.Place << endl;
		cout << ">------------------------------------<" << endl;
		cout << "按任意键继续。。。" << endl;
		getchar(); getchar();
	}
	else
	{
		cout << "在数据库中没有查到该" << Data.table << "账号" << endl;
		cout << "按任意键继续。。。" << endl;
		getchar(); getchar();
	}
}
//删除QQ好友
void user::DeleteFriend(message & Data, clientSocket & mySocket)
{
	cout << "请输入想要删除的好友"<<Data.table<<"账号:";
	cin >> Data.XID;
	if (!checkAddFriend(atoi(Data.XID)))
	{
		itoa(getXID(), Data.ID, 10);
		mySocket.sendData(Data);				 //让服务器去查找数据库并添加	1.标记
		mySocket.receiveData(Data);				 //接收反馈						2.标记
		if (1 == Data.action)
		{
			deleteFriend(Data.XID);			//同步内存
			cout << "删除好友成功！" << endl;
			cout << "按任意键继续..." << endl;
			getchar(); getchar();
		}
		else
		{
			cout << "在数据库中添加好友失败" << endl;
			cout << "按任意键继续..." << endl;
			getchar(); getchar();

		}
	}
	else
	{
		Data.action = -1;
		mySocket.sendData(Data);
		cout << "您还不是他的好友，不能删除" << endl;
	}

}
//选择好友 //1.设置好友备注/2.聊天/3.查看好友信息/4.返回上一级
void user::chooseFriend(message & Data, clientSocket & mySocket)
{
	while (true)
	{
		cout << "*************************" << endl;
		cout << "1.设置好友备注" << endl;
		cout << "2.选择好友聊天" << endl;
		cout << "3.查看好友信息" << endl;
		cout << "4.删除好友" << endl;
		cout << "9.返回上一级" << endl;
		cout << "*************************" << endl;
		cout << "请输入指令：";
		cin >> Data.action;
		mySocket.sendData(Data);		//1.对应标记
		cout << "正在请求服务器。。。" << endl;
		mySocket.receiveData(Data);		
		switch (Data.action)
		{
		case 1://1.设置好友备注
			SetNickName(Data, mySocket);
			break;
		case 2://2.聊天
			ChatToFriend(Data, mySocket);
			break;
		case 3://3.查看好友信息
			FindFriendInfo(Data, mySocket);
			break;
		case 4:
			DeleteFriend(Data, mySocket);
			break;
		case 9://返回上一级
			return;
		default:
			cout << "输入的指令有误" << endl;
			break;
		}
	}

}
//外层聊天函数
void MyQQ::ChatToFriend(message & Data, clientSocket & mySocket)
{
	mySocket.receiveData(Data);
	cout << "当前好友" << Data.FriendNum << "位在线" << endl;
	int num = Data.FriendNum;
	
	for(int i = 0; i < num; i++)
	{
		cout <<setw(12)<< Data.FriendList[i] 
			 <<setw(12)<< Data.FriendNickName[i]
			 << endl;
		
	}
	cout << "请输入好友账号:";
	cin >> Data.QQID;
	int flag = 0;
	for (int i = 0; i < num; i++)
	{
		if (0 == strcmp(Data.QQID, Data.FriendList[i]))
		{
			Data.ret = i;
			flag = 1;
			break;
		}
	}
	if (flag)
	{
		Data.action = 1;
		mySocket.sendData(Data);
		QQChatting(Data, mySocket);

	}
	else
	{
		Data.action = -1;
		mySocket.sendData(Data);
		cout << "好友不在线，离线留言功能正在开发中" << endl;
		return;
	}
}
//线程：listen 接收并打印服务器发来的聊天信息(身份/时间标识)
void QQListening(message & Data, clientSocket & mySocket)
{
	cout << "已启用线程接听中" << endl;
	while (true)
	{
		mySocket.receiveData(Data);
		//名字  和  时间
		cout << Data.Name << "--" << Data.Place;
		cout << Data.paswad << endl;
		if (Data.action == -1)
		{
			cout << "对方已退出聊天,我方停止接收数据,可以退出" << endl;
			break;
		}
	}
}
//开始聊天
void MyQQ::QQChatting(message & Data, clientSocket & mySocket)
{
	//启用线程，接听服务器传来的信息
	//注意，引用类型的参数要使用ref()函数
	message ret;
	std::thread t1(&::QQListening, ref(ret), ref(mySocket));			
	t1.detach();
	strcpy_s(Data.Name, getXName());

	cout << "输入‘#’结束聊天" << endl;
	while (true)
	{
		time_t t;
		time(&t);
		cin >> Data.paswad;
		strcpy_s(Data.Place, ctime(&t));
		if (0 == strcmp(Data.paswad, "#"))
		{
			Data.action = -1;
			mySocket.sendData(Data);
			break;
		}
		mySocket.sendData(Data);
	}
	cout << "请等待退出提示" << endl;
	getchar();
	getchar();
	cout << "成功退出" << endl;
	//注意 线程 t1 将在这里析构
	//所以退出时要等待双方相互握手，防止服务器阻塞
}
//选择群聊 //1.退出群/2.推荐给好友(待实现)/3.踢人/4.查询成员/5.升级群/6.创建子群
void MyQQ::chooseGroup(message & Data, clientSocket & mySocket)
{
	while (true)
	{
		cout << "*************************" << endl;
		cout << "1.退出群聊" << endl;
		cout << "2.将群聊推荐给好友" << endl;
		cout << "3.群踢人" << endl;
		cout << "4.查询群成员" << endl;
		cout << "5.升级群" << endl;
		cout << "6.创建讨论组(子群)" << endl;
		cout << "9.返回上一级" << endl;
		cout << "*************************" << endl;
		cout << "请输入指令：";
		cin >> Data.action;
		mySocket.sendData(Data);		//1.对应标记
		cout << "正在请求服务器。。。" << endl;
		mySocket.receiveData(Data);
		switch (Data.action)
		{
		case 1://退出群聊
			cout << "请输入群聊ID：";
			cin >> Data.GID;
			//内存中找到该群并退出
			if (dropGroup(atoi(Data.GID)))
			{
				Data.action = 1;
				itoa(getXID(), Data.XID, 10);
				mySocket.sendData(Data);		//让服务器去改数据库    2.对应标记
				mySocket.receiveData(Data);		//接收反馈
				if (1 == Data.action)
				{
					cout << "退出群聊成功！" << endl;
					cout << "按任意键继续..." << endl;
					getchar(); getchar();
					break;
				}
				else
				{
					cout << "在数据库中退出群聊失败" << endl;
					cout << "按任意键继续..." << endl;
					getchar(); getchar();
					return;
				}

			}
			else
			{
				//不是群成员，取消请求数据库
				Data.action = -1;
				mySocket.sendData(Data);
				cout << "输入QQ群聊账号有误，您还未加入该群聊/群聊不存在" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			break;
		case 2://推荐给好友
			cout << "Sorry QQ不支持此操作。。。。" << endl;
			cout << "按任意键继续..." << endl;
			getchar(); getchar();
			break;
		case 3://踢人
			cout << "请输入群账号:";
			cin >> Data.GID;
			cout << "请输入成员QQ账号：";
			cin >> Data.XID;
			itoa(getXID(),Data.ID,10);
			mySocket.sendData(Data);
			//服务器处理并反馈
			mySocket.receiveData(Data);
			if (1 == Data.action)
			{
				cout << "您已将" << Data.XID << "踢出群聊" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			else if(-1 == Data.action)
			{
				cout << "踢出失败，该成员不存在或群不存在" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			else if (0 == Data.action)
			{
				cout << "踢出失败，您的群第位比他低" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			else
			{
				cout << "踢出失败，服务器走神啦~~~(数据库踢出语句出错)" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			break;
		case 4://查询群成员
			cout << "请输入群号码：";
			cin >> Data.GID;
			mySocket.sendData(Data);
			mySocket.receiveData(Data);
			cout << ">--------------------------<" << endl;
			for (int i = 0; i < Data.GroupNum; i++)
			{
				cout << "账号:" << Data.GroupList[i]<<"     ";
				cout << "名字:" << Data.GroupName[i]<<"     ";
				cout << "身份:" << Data.GroupStatus[i] << endl;
			}
			cout << "共 " << Data.GroupNum << " 个成员" << endl;
			cout << ">--------------------------<" << endl;
			break;
		case 5://升级群
			cout << "请输入群号码：";
			itoa(getXID(), Data.ID, 10);
			cin >> Data.GID;
			mySocket.sendData(Data);
			mySocket.receiveData(Data);
			if (1 == Data.action)
			{
				cout << "升级成功，本群已由讨论组升级为常规QQ群" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			else if (0 == Data.action)
			{
				cout << "升级失败，本群已经是常规QQ群(最高级)无法继续升级" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			else
			{
				cout << "升级失败，您不是该群群主" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			break;
		case 6:
			cout << "请输群账号：";
			cin >> Data.GID;
			cout << "请输入子群名称：";
			cin >> Data.GroupName[1];
			itoa(getXID(), Data.XID, 10);
			strcpy_s(Data.Name, getXName());
			mySocket.sendData(Data);
			mySocket.receiveData(Data);
			if (1 == Data.action)
			{
				creatTempGroup(atoi(Data.GID), Data.GroupName[0]);
				cout << "创建成功" << endl;

			}
			else
			{
				cout << "创建失败,您不在该群内" << endl;
			}
			break;
		case 9://返回上一级
			return;
		default:
			cout << "输入的指令有误" << endl;
			cout << "按任意键继续。。。" << endl;
			getchar(); getchar();
			break;
		}
	}
}
//添加好友
void user::addFriend(message & Data, clientSocket & mySocket)
{
	//推荐好友
	cout << "请输入想要添加的好友"<< Data.table <<"账号:";
	cin >> Data.XID;
	cout << "请输入您为好友设置的备注/昵称:";
	cin >> Data.NickName;
	if (checkAddFriend(atoi(Data.QQID)))
	{
		Data.action = 1;
		itoa(getXID(), Data.ID, 10);
		itoa(getUserID(), Data.GID, 10);	 //用GID临时保存平台总ID
		strcpy_s(Data.Name, getXName());	 //本账号名字
		mySocket.sendData(Data);				 //让服务器去查找数据库并添加	1.标记
		mySocket.receiveData(Data);				 //接收反馈						2.标记
		if (1 == Data.action)
		{
			addFriend(Data.XID, Data.NickName);
			cout << "添加好友成功！" << endl;
			cout << "按任意键继续..." << endl;
			getchar(); getchar();
		}
		else
		{
			cout << "在数据库中添加好友失败" << endl;
			cout << "按任意键继续..." << endl;
			getchar(); getchar();
		}
	}
	else
	{
		//已经是好友，取消好友申请
		Data.action = -1;
		mySocket.sendData(Data);						//1.标记
		cout << "您已添加该好友，请勿重复添加" << endl;
		cout << "按任意键继续..." << endl;
		getchar(); getchar();

	}

}
//添加群聊
void user::addGroup(message & Data, clientSocket & mySocket)
{
	cout << "请输入群聊ID：";
	cin >> Data.GID;
	//用户没有加入该群
	if (checkAddGroup(atoi(Data.GID)))
	{
		Data.action = 1;
		itoa(getXID(), Data.XID, 10);
		strcpy_s(Data.Name, getXName());
		mySocket.sendData(Data);		//让服务器去查找数据库并添加	1.标记
		mySocket.receiveData(Data);		//接收反馈						2.标记
		if (1 == Data.action)
		{
			addGroup(Data.GID,Data.GroupName[0]);
			cout << "添加群聊成功！" << endl;
			cout << "按任意键继续..." << endl;
			getchar(); getchar();
			
		}
		else
		{
			cout << "在数据库中退出群聊失败" << endl;
			cout << "按任意键继续..." << endl;
			getchar(); getchar();
			
		}
	}
	else
	{
		//已经是群成员，取消加群请求
		Data.action = -1;
		mySocket.sendData(Data);						//1.标记
		cout << "您已添加该群，请勿重复添加" << endl;
		cout << "按任意键继续..." << endl;
		getchar(); getchar();
	}
		
}
//创建群组
void user::creatGroup(message & Data, clientSocket & mySocket)
{
	cout << "设置群名称：";
	cin >> Data.GroupName[1];
	itoa(getXID(), Data.GroupList[1], 10);
	strcpy_s(Data.Name, getXName());
	cout << "正在创建。。。。" << endl;
	mySocket.sendData(Data);
	mySocket.receiveData(Data);
	if (1 == Data.action)
	{
		//内存加群
		creatGroup(atoi(Data.GroupList[0]), Data.GroupName[1]);
		cout << "创建群聊成功,您当前是群主" << endl;
	}
	else
	{
		cout << "创建群聊失败" << endl;
	}
}
//选择群聊 //1.退出群/2.推荐给好友(待实现)/3.踢人/4.查询成员
void MyWeChat::chooseGroup(message & Data, clientSocket & mySocket)
{
	while (true)
	{
		cout << "*************************" << endl;
		cout << "1.退出群聊" << endl;
		cout << "2.将群聊推荐给好友" << endl;
		cout << "3.群踢人" << endl;
		cout << "4.查询群成员" << endl;
		cout << "9.返回上一级" << endl;
		cout << "*************************" << endl;
		cout << "请输入指令：";
		cin >> Data.action;
		mySocket.sendData(Data);		//1.对应标记
		cout << "正在请求服务器。。。" << endl;
		mySocket.receiveData(Data);
		switch (Data.action)
		{
		case 1://退出群聊
			cout << "请输入群聊ID：";
			cin >> Data.GID;
			//内存中找到该群并退出
			if (dropGroup(atoi(Data.GID)))
			{
				Data.action = 1;
				itoa(getXID(), Data.XID, 10);
				mySocket.sendData(Data);		//让服务器去改数据库    2.对应标记
				mySocket.receiveData(Data);		//接收反馈
				if (1 == Data.action)
				{
					cout << "退出群聊成功！" << endl;
					cout << "按任意键继续..." << endl;
					getchar(); getchar();
					break;
				}
				else
				{
					cout << "在数据库中退出群聊失败" << endl;
					cout << "按任意键继续..." << endl;
					getchar(); getchar();
					return;
				}

			}
			else
			{
				//不是群成员，取消请求数据库
				Data.action = -1;
				mySocket.sendData(Data);
				cout << "输入QQ群聊账号有误，您还未加入该群聊/群聊不存在" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			break;
		case 2://推荐给好友
			cout << "Sorry 微信群推荐 功能正在开发中。。。。" << endl;
			cout << "按任意键继续..." << endl;
			getchar(); getchar();
			break;
		case 3://踢人
			cout << "请输入群账号:";
			cin >> Data.GID;
			cout << "请输入成员WeChat账号：";
			cin >> Data.XID;
			itoa(getXID(), Data.ID, 10);
			mySocket.sendData(Data);
			//服务器处理并反馈
			mySocket.receiveData(Data);
			if (1 == Data.action)
			{
				cout << "您已将" << Data.WCID << "踢出群聊" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			else if (-1 == Data.action)
			{
				cout << "踢出失败，该成员不存在或群不存在" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			else if (0 == Data.action)
			{
				cout << "踢出失败，您的群第位比他低" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			else
			{
				cout << "踢出失败，服务器走神啦~~~(数据库踢出语句出错)" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			break;
		case 4://查询群成员
			cout << "请输入群号码：";
			cin >> Data.GID;
			mySocket.sendData(Data);
			mySocket.receiveData(Data);
			cout << ">--------------------------<" << endl;
			for (int i = 0; i < Data.GroupNum; i++)
			{
				cout << "账号:" << Data.GroupList[i] << "     ";
				cout << "名字:" << Data.GroupName[i] << "     ";
				cout << "身份:" << Data.GroupStatus[i] << endl;
			}
			cout << "共 " << Data.GroupNum << " 个成员" << endl;
			cout << ">--------------------------<" << endl;
			break;
		case 9://返回上一级
			return;
		default:
			cout << "输入的指令有误" << endl;
			cout << "按任意键继续。。。" << endl;
			getchar(); getchar();
			break;
		}
	}
}
//微信的空功能
void MyWeChat::ChatToFriend(message & Data, clientSocket & mySocket)
{
	cout << "微信暂不支持即时通讯功能哦~~" << endl;
}
//选择群聊 //1.退出群/2.推荐给好友(待实现)/3.踢人/4.查询成员
void MyWeBo::chooseGroup(message & Data, clientSocket & mySocket)
{
	while (true)
	{
		cout << "*************************" << endl;
		cout << "1.退出群聊" << endl;
		cout << "2.将群聊推荐给好友" << endl;
		cout << "3.群踢人" << endl;
		cout << "4.查询群成员" << endl;
		cout << "9.返回上一级" << endl;
		cout << "*************************" << endl;
		cout << "请输入指令：";
		cin >> Data.action;
		mySocket.sendData(Data);		//1.对应标记
		cout << "正在请求服务器。。。" << endl;
		mySocket.receiveData(Data);
		switch (Data.action)
		{
		case 1://退出群聊
			cout << "请输入群聊ID：";
			cin >> Data.GID;
			//内存中找到该群并退出
			if (dropGroup(atoi(Data.GID)))
			{
				Data.action = 1;
				itoa(getXID(), Data.XID, 10);
				mySocket.sendData(Data);		//让服务器去改数据库    2.对应标记
				mySocket.receiveData(Data);		//接收反馈
				if (1 == Data.action)
				{
					cout << "退出群聊成功！" << endl;
					cout << "按任意键继续..." << endl;
					getchar(); getchar();
					break;
				}
				else
				{
					cout << "在数据库中退出群聊失败" << endl;
					cout << "按任意键继续..." << endl;
					getchar(); getchar();
					return;
				}

			}
			else
			{
				//不是群成员，取消请求数据库
				Data.action = -1;
				mySocket.sendData(Data);
				cout << "输入QQ群聊账号有误，您还未加入该群聊/群聊不存在" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			break;
		case 2://推荐给好友
			cout << "Sorry 微博群推荐 功能正在开发中。。。。" << endl;
			cout << "按任意键继续..." << endl;
			getchar(); getchar();
			break;
		case 3://踢人
			cout << "请输入群账号:";
			cin >> Data.GID;
			cout << "请输入成员WeChat账号：";
			cin >> Data.XID;
			itoa(getXID(), Data.ID, 10);
			mySocket.sendData(Data);
			//服务器处理并反馈
			mySocket.receiveData(Data);
			if (1 == Data.action)
			{
				cout << "您已将" << Data.WCID << "踢出群聊" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			else if (-1 == Data.action)
			{
				cout << "踢出失败，该成员不存在或群不存在" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			else if (0 == Data.action)
			{
				cout << "踢出失败，您的群第位比他低" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			else
			{
				cout << "踢出失败，服务器走神啦~~~(数据库踢出语句出错)" << endl;
				cout << "按任意键继续..." << endl;
				getchar(); getchar();
			}
			break;
		case 4://查询群成员
			cout << "请输入群号码：";
			cin >> Data.GID;
			mySocket.sendData(Data);
			mySocket.receiveData(Data);
			cout << ">--------------------------<" << endl;
			for (int i = 0; i < Data.GroupNum; i++)
			{
				cout << "账号:" << Data.GroupList[i] << "     ";
				cout << "名字:" << Data.GroupName[i] << "     ";
				cout << "身份:" << Data.GroupStatus[i] << endl;
			}
			cout << "共 " << Data.GroupNum << " 个成员" << endl;
			cout << ">--------------------------<" << endl;
			break;
		case 9://返回上一级
			return;
		default:
			cout << "输入的指令有误" << endl;
			cout << "按任意键继续。。。" << endl;
			getchar(); getchar();
			break;
		}
	}
}
//微博的空功能
void MyWeBo::ChatToFriend(message & Data, clientSocket & mySocket)
{
	cout << "微博暂不支持即时通讯功能哦~~" << endl;
}
