#pragma once
#include"clientHead.h"
#include<iostream>
#include<mysql.h>
using namespace std;
//返回平台总ID
int user::getUserID() {
	return this->ID;
}
//基类构造函数，初始化平台总ID
user::user(message & Data, message & rent, clientSocket & mySocket) {  //将用户资料加载至内存
	//发送初始化请求
	Data.action = 10;
	mySocket.sendData(Data);
	cout << "初始化user请求已发送" << endl;
	mySocket.receiveData(Data);
	if (Data.action == 1)
	{
		this->ID = atoi(Data.ID);
		cout << "初始化user请求成功" << endl;
		cout << "user.ID" << Data.ID << endl;
	}
}
//基类析构函数
user::~user() {
	FriendNode* fres = FriendList;
	FriendNode* fresp = FriendList->next;
	int friendNum = FriendList->FriendId;
	GroupNode * gres = GroupList;
	GroupNode * gresp = GroupList->next;
	int groupNum = GroupList->GroupId;
	int i = 1;
	while (friendNum--)
	{
		delete fres;
		fres = fresp;
		if(fresp) fresp = fresp->next;
	}
	delete fres;
	while (groupNum--)
	{
		delete gres;
		gres = gresp;
		if(gresp) gresp = gresp->next;
	}
	delete gres;
	cout << "user 析构完毕" << endl;
}
//构造QQ对象，从数据库获取数据，初始化个人信息，好友列表，群组列表等
MyQQ::MyQQ(message & Data, message & rent, clientSocket & mySocket) :user(Data, rent, mySocket)
{
	Data.action = 11;
	strcpy_s(Data.XID, Data.QQID);
	mySocket.sendData(Data);
	cout << "初始化MyQQ请求已发送" << endl;
	mySocket.receiveData(Data);
	if (Data.action == 1)
	{
		this->TAge = Data.TAge;
		strcpy_s(this->BirthDay, Data.BirthDay);
		strcpy_s(this->Place, Data.Place);
		strcpy_s(this->Name, Data.Name);
		//绑定了微信
		if (Data.platform >= 10)
		{
			cout << "检测到绑定的微信账号:" << Data.WCID << endl;
			this->isLinkToWeChat = true;
			this->WID = atoi(Data.WCID);
		}
		this->QID = atoi(Data.QQID);
		FriendNode * res;
		FriendList = new FriendNode;
		FriendNode * rear = FriendList;
		FriendList->FriendId = Data.FriendNum;  //第一个结点的Id存放的是好友个数
		for (int i = 0; i < Data.FriendNum; i++)
		{
			res = new FriendNode;
			res->FriendId = atoi(Data.FriendList[i]);
			res->FriendAccount = atoi(Data.FriendAccount[i]);
			strcpy_s(res->NickName, Data.FriendNickName[i]);
			cout << i+1 << "个好友ID:  " << res->FriendId 
				<< "  NickName:" << res->NickName << endl;
			rear->next = res;
			rear = res;
		}
		cout << "共：" << Data.FriendNum << "个好友" << endl;


		GroupNode * ress;
		GroupList = new GroupNode;
		GroupNode* rearr = GroupList;
		GroupList->GroupId = Data.GroupNum;		 //第一个结点的Id存放的是群组个数
		GroupList->status = atoi(Data.ID);
		for (int i = 0; i < Data.GroupNum; i++)
		{
			ress = new GroupNode;
			ress->GroupId = atoi(Data.GroupList[i]);
			strcpy_s(ress->GroupName, Data.GroupName[i]);
			ress->status = Data.GroupStatus[i];
			cout << i+1 << "个群组GID:" << ress->GroupId << "群名：" 
				<< ress->GroupName <<"身份："<<ress->status<< endl;
			rearr->next = ress;
			rearr = ress;
		}

		cout << "初始化MyQQ请求成功！" << endl;
		
	}
	else
	{
		cout << "用户不存在QQ账号" << endl;
	}
}
//打印本人详细信息
void MyQQ::showMyInfo()
{
	cout << ">------------------------------------<" << endl;
	cout << "名字：" << this->Name << endl;
	cout << "QQ账号：" << this->QID << endl;
	cout << "QQ龄：" << this->TAge << endl;
	cout << "出生日期：" << this->BirthDay << endl;
	cout << "所在地：" << this->Place << endl;
	if (this->isLinkToWeChat)
	{
		cout << "绑定的微信账号:" << this->WID << endl;
	}
	else
	{
		cout << "未绑定微信账号" << endl;
	}
	cout << ">------------------------------------<" << endl;
	cout << "按任意键继续。。。" << endl;
	getchar(); getchar();
}
//打印好友列表
void user::showFriendList()
{
	int num = this->FriendList->FriendId;
	FriendNode * pre = this->FriendList->next;
	cout << "*************************************" << endl;
	cout << "好友备注---|---好友账号" << endl;
	while (num--)
	{
		cout <<setw(10)<<pre->NickName <<setw(10)<< pre->FriendId << endl;
		pre = pre->next;
	}
	cout << "*************************************" << endl;
	cout << "按任意键继续。。。" << endl;
	getchar(); getchar();
}
//打印群组信息
void user::showGroupList()
{
	int num = this->GroupList->GroupId;
	GroupNode * pre = this->GroupList->next;
	cout << "*************************************" << endl;
	cout << "群组名称---|---群组账号---|---群组地位" << endl;
	while (num--)
	{
		cout<<setw(12) <<pre->GroupName 
			<<setw(12) <<pre->GroupId 
			<<setw(12) <<pre->status<< endl;
		pre = pre->next;
	}
	cout << "*************************************" << endl;
	cout << "按任意键继续。。。" << endl;
	getchar(); getchar();
}
//设置好友备注
int user::setFriendNickName(int ID, char Name[])
{
	int Fnum = this->FriendList->FriendId;
	FriendNode * pre = this->FriendList->next;
	while (Fnum--)
	{
		//找到此好友
		if (pre->FriendId == ID)
		{
			strcpy_s(pre->NickName, Name);
			return 1;
		}
		pre = pre->next;
	}
	return 0;
}	
//返回QQ账号
int MyQQ::getXID()
{
	return this->QID;
}
//返回QQ名字
char * MyQQ::getXName()
{
	return this->Name;
}
//添加好友前判断是否重复添加
int  user::checkAddFriend(int Fid)
{
	int friendNum = this->FriendList->FriendId;
	FriendNode * pre = this->FriendList->next;
	//判断是否重复添加
	while (friendNum--)
	{
		if (pre->FriendId == Fid)
		{
			return 0;
		}
		pre = pre->next;
	}
	return 1;
}
//群的申请加入
int  user::checkAddGroup(int Gid)
{
	int groupNum = this->GroupList->GroupId;
	GroupNode * pre = this->GroupList->next;
	//判断是否重复添加
	while (groupNum--)
	{
		if (pre->GroupId == Gid)
		{
			return 0;
		}
		pre = pre->next;
	}
	return 1;

}
//群的退出
int  user::dropGroup(int Gid)
{
	int groupNum = this->GroupList->GroupId;
	GroupNode * pre = this->GroupList->next;
	GroupNode * prv = this->GroupList;
	//判断是否添加
	while (groupNum--)
	{
		//找到目标，退群！
		if (pre->GroupId == Gid)
		{
			prv->next = pre->next;
			delete pre;
			this->GroupList->GroupId--;
			return 1;
		}
		pre = pre->next;
		prv = prv->next;
	}
	return 0;
}
//创建讨论组(子群)
int user::creatTempGroup(int Gid, char gName[])
{
	int groupNum = this->GroupList->GroupId;
	GroupNode * res = new GroupNode;
	res->GroupId = Gid;
	cout << "子群号码:  " << Gid << endl;
	res->status = 3;
	strcpy_s(res->GroupName, gName);
	res->next = GroupList->next;
	GroupList->next = res;
	GroupList->GroupId++;
	return 1;
}
//群的创建(正常版)非临时讨论组
int user::creatGroup(int Gid, char gName[])
{
	int groupNum = this->GroupList->GroupId;
	GroupNode * res = new GroupNode;
	res->GroupId = Gid;
	cout << "群号码:  " << Gid << endl;
	res->status = 3;
	strcpy_s(res->GroupName, gName);
	res->next = GroupList->next;
	GroupList->next = res;
	GroupList->GroupId++;
	return 1;
}
//将从服务器发送过来的 加群 Data刷新到内存
int user::addGroup(char Gid[], char GroupName[])
{
	GroupNode * res = new GroupNode;
	res->GroupId = atoi(Gid);
	//记得让服务器改Data
	//把新加入的群放到链表头部
	strcpy_s(res->GroupName, GroupName);
	res->status = 0;
	res->next = this->GroupList->next;
	this->GroupList->next = res;
	this->GroupList->GroupId++;
	return 1;
}
//将从服务器发送过来的 加好友 Data刷新到内存
int user::addFriend(char Fid[], char NickName[])
{
	FriendNode * res = new FriendNode;
	res->FriendId = atoi(Fid);
	strcpy_s(res->NickName, NickName);
	res->next = this->FriendList->next;
	this->FriendList->next = res;
	this->FriendList->FriendId++;
	return 1;
}
//将从服务器发送过来的 删好友 Data刷新到内存
void user::deleteFriend(char Fid[])
{
	int friendNum = this->FriendList->FriendId;
	FriendNode * pre = this->FriendList->next;
	FriendNode * prv = this->FriendList;
	while (friendNum--)
	{
		if (pre->FriendId == atoi(Fid))
		{
			prv->next = pre->next;
			cout << "内存中删除" << Fid << endl;
			this->FriendList->FriendId--;
			delete pre;
			return;
		}
		pre = pre->next;
		prv = prv->next;
	}
	cout << "警告：内存中没有完成删除好友" << Fid << endl;

}
//获取当前好友列表
FriendNode* user::getFriendList()
{
	return this->FriendList;
}
//获取推荐好友  返回推荐的个数
void user::getRecommendFriend(
	FriendNode* otherFriendList
	, const char * platform
	, message & Data
	, clientSocket & mySocket)
{
	int cnt = 0;
	FriendNode * myFriend = this->FriendList;
	FriendNode * res = myFriend->next;
	FriendNode * loc = otherFriendList->next;
	for (int i = 0; i < otherFriendList->FriendId; i++)
	{
		int flag = 1;
		res = myFriend->next;
		for (int j = 0; j < myFriend->FriendId; j++)
		{
			if (res->FriendAccount == loc->FriendAccount)
			{
				flag = 0;
				break;
			}
			res = res->next;
		}
		//在当前好友列表中没有找到 other 平台已有的 loc 好友
		if (flag)
		{
			itoa(loc->FriendAccount, Data.ID, 10);
			Data.action = 3;
			if (isExist(Data, mySocket))
			{
				cout << "为您推荐来自 " << platform << " 的好友:" 
					 << loc->NickName << " " << Data.XID << endl;
			}
		}
		loc = loc->next;
	}
}
bool user::isExist(message & Data, clientSocket & mySocket)
{
	mySocket.sendData(Data);
	mySocket.receiveData(Data);
	if (Data.action == 4)
	{
		return true;
	}
	return false;
}
//返回微信账号
int MyWeChat::getXID()
{
	return this->WID;
}
//返回微信名字
char * MyWeChat::getXName()
{
	return this->Name;
}
//打印本人详细信息
void MyWeChat::showMyInfo()
{
	cout << ">------------------------------------<" << endl;
	cout << "名字：" << this->Name << endl;
	cout << "微信账号：" << this->WID << endl;
	cout << "微信龄：" << this->TAge << endl;
	cout << "出生日期：" << this->BirthDay << endl;
	cout << "所在地：" << this->Place << endl;
	if (this->isLinkToQQ)
	{
		cout << "绑定的QQ账号:" << this->QID << endl;
	}
	else
	{
		cout << "未绑定微信账号" << endl;
	}
	cout << ">------------------------------------<" << endl;
	cout << "按任意键继续。。。" << endl;
	getchar(); getchar();
}
//微信菜单
void MyWeChat::Xmenu()
{
	cout << "*****************************" << endl;
	cout << "1.我的信息" << endl;			//1.显示我的信息 按任意键继续
	cout << "2.选择好友" << endl;			//1.设置好友备注/2.聊天/3.查看好友信息
	cout << "3.选择群聊" << endl;			//1.退出群/2.推荐给好友/3.踢人/4.查询成员/5.升级群
	cout << "4.添加好友" << endl;			//1.输入微信账号 直接添加成功
	cout << "5.推荐群组" << endl;			//1.输入微信群账号 和 好友账号 直接拉好友加入
	cout << "6.打印好友列表" << endl;		//1.访问内存 直接打印（非详细）
	cout << "7.打印群组列表" << endl;		//1.访问内存 直接打印（非详细）
	cout << "8.创建群聊" << endl;
	cout << "9.退出" << endl;
	cout << "*****************************" << endl;

}
//微信构造函数
MyWeChat::MyWeChat(message & Data, message & rent, clientSocket & mySocket) :user(Data, rent, mySocket)
{
	Data.action = 11;
	strcpy_s(Data.XID, Data.WCID);
	mySocket.sendData(Data);
	cout << "初始化MyWeChat请求已发送" << endl;
	mySocket.receiveData(Data);
	if (Data.action == 1)
	{
		this->TAge = Data.TAge;
		strcpy_s(this->BirthDay, Data.BirthDay);
		strcpy_s(this->Place, Data.Place);
		strcpy_s(this->Name, Data.Name);
		//绑定了QQ
		if (Data.platform >= 10)
		{
			cout << "检测到绑定的QQ账号:" << Data.QQID << endl;
			this->isLinkToQQ = true;
			this->QID = atoi(Data.QQID);
		}
		this->WID = atoi(Data.WCID);
		FriendNode * res;
		FriendList = new FriendNode;
		FriendNode * rear = FriendList;
		FriendList->FriendId = Data.FriendNum;  //第一个结点的Id存放的是好友个数
		for (int i = 0; i < Data.FriendNum; i++)
		{
			res = new FriendNode;
			res->next = NULL;
			res->FriendId = atoi(Data.FriendList[i]);
			res->FriendAccount = atoi(Data.FriendAccount[i]);
			strcpy_s(res->NickName, Data.FriendNickName[i]);
			cout << i + 1 << "个好友ID:  " << res->FriendId
				<< "  NickName:" << res->NickName << endl;
			rear->next = res;
			rear = res;
		}
		cout << "共：" << Data.FriendNum << "个好友" << endl;


		GroupNode * ress;
		GroupList = new GroupNode;
		GroupNode* rearr = GroupList;
		GroupList->GroupId = Data.GroupNum;		 //第一个结点的Id存放的是群组个数
		GroupList->status = atoi(Data.ID);
		for (int i = 0; i < Data.GroupNum; i++)
		{
			ress = new GroupNode;
			ress->next = NULL;
			ress->GroupId = atoi(Data.GroupList[i]);
			strcpy_s(ress->GroupName, Data.GroupName[i]);
			ress->status = Data.GroupStatus[i];
			cout << i + 1 << "个群组GID:" << ress->GroupId << "群名："
				<< ress->GroupName << "身份：" << ress->status << endl;
			rearr->next = ress;
			rearr = ress;
		}

		cout << "初始化MyWeChat请求成功！" << endl;

	}
	else
	{
		cout << "用户不存在MyWeChat账号" << endl;
	}
}
// *多态* 推荐群组给好友
void MyWeChat::addGroup(message & Data, clientSocket & mySocket)
{
	cout << "请输入好友ID:";
	cin >> Data.XID;
	//不是好友
	if (checkAddFriend(atoi(Data.XID)))
	{
		cout << "您还不是他的好友，不能推荐" << endl;
		Data.action = -1;
		mySocket.sendData(Data);
		return;
	}
	cout << "请输入群聊ID:";
	cin >> Data.GID;
	if (checkAddGroup(atoi(Data.GID)))
	{
		cout << "您还不在该群内，不能推荐" << endl;
		Data.action = -1;
		mySocket.sendData(Data);
		return;
	}
	Data.action = 1;
	mySocket.sendData(Data);
	mySocket.receiveData(Data);
	if (Data.action == 1)
	{
		cout << "推荐成功，该好友已被您拉入群内" << endl;
	}
	//人不存在
	else if (Data.action == 2)
	{
		cout << "该用户不存在，请输入正确的账号" << endl;
	}
	//群不存在
	else if (Data.action == 3)
	{
		cout << "该群不存在，请输入正确的群账号" << endl;
	}
	else
	{
		cout << "未知错误" << endl;
	}
	cout << "按任意键继续..." << endl;
	getchar(); getchar();
}
// *多态* 推荐群组给好友
void MyWeBo::addGroup(message & Data, clientSocket & mySocket)
{
	cout << "请输入好友ID:";
	cin >> Data.XID;
	//不是好友
	if (checkAddFriend(atoi(Data.XID)))
	{
		cout << "您还不是他的好友，不能推荐" << endl;
		Data.action = -1;
		mySocket.sendData(Data);
		return;
	}
	cout << "请输入群聊ID:";
	cin >> Data.GID;
	if (checkAddGroup(atoi(Data.GID)))
	{
		cout << "您还不在该群内，不能推荐" << endl;
		Data.action = -1;
		mySocket.sendData(Data);
		return;
	}
	Data.action = 1;
	mySocket.sendData(Data);
	mySocket.receiveData(Data);
	if (Data.action == 1)
	{
		cout << "推荐成功，该好友已被您拉入群内" << endl;
	}
	//人不存在
	else if (Data.action == 2)
	{
		cout << "该用户不存在，请输入正确的账号" << endl;
	}
	//群不存在
	else if (Data.action == 3)
	{
		cout << "该群不存在，请输入正确的群账号" << endl;
	}
	else
	{
		cout << "未知错误" << endl;
	}
	cout << "按任意键继续..." << endl;
	getchar(); getchar();
}
//返回微博账号
int MyWeBo::getXID()
{
	return this->BID;
}
//返回微博名字
char * MyWeBo::getXName()
{
	return this->Name;
}
//打印本人详细信息
void MyWeBo::showMyInfo()
{
	cout << ">------------------------------------<" << endl;
	cout << "名字：" << this->Name << endl;
	cout << "微信账号：" << this->BID << endl;
	cout << "微信龄：" << this->TAge << endl;
	cout << "出生日期：" << this->BirthDay << endl;
	cout << "所在地：" << this->Place << endl;
	if (this->isQQOpen)
	{
		cout << "共用的的QQ账号:" << this->QID << endl;
	}
	else
	{
		cout << "未共用qq账号" << endl;
	}
	cout << ">------------------------------------<" << endl;
	cout << "按任意键继续。。。" << endl;
	getchar(); getchar();
}
//微博菜单
void MyWeBo::Xmenu()
{
	cout << "*****************************" << endl;
	cout << "1.我的信息" << endl;			//1.显示我的信息 按任意键继续
	cout << "2.选择好友" << endl;			//1.设置好友备注/2.聊天/3.查看好友信息
	cout << "3.选择群聊" << endl;			//1.退出群/2.推荐给好友/3.踢人/4.查询成员/5.升级群
	cout << "4.添加好友" << endl;			//1.输入账号 直接添加成功
	cout << "5.推荐群组" << endl;			//1.输入微博群账号 推荐好友直接加入
	cout << "6.打印好友列表" << endl;		//1.访问内存 直接打印（非详细）
	cout << "7.打印群组列表" << endl;		//1.访问内存 直接打印（非详细）
	cout << "8.创建群聊" << endl;
	cout << "9.退出" << endl;
	cout << "*****************************" << endl;

}
//微博构造函数
MyWeBo::MyWeBo(message & Data, message & rent, clientSocket & mySocket) :user(Data, rent, mySocket)
{
	Data.action = 11;
	strcpy_s(Data.XID, Data.WBID);
	mySocket.sendData(Data);
	cout << "初始化MyWeBo请求已发送" << endl;
	mySocket.receiveData(Data);
	if (Data.action == 1)
	{
		this->TAge = Data.TAge;
		strcpy_s(this->BirthDay, Data.BirthDay);
		strcpy_s(this->Place, Data.Place);
		strcpy_s(this->Name, Data.Name);
		//绑定了QQ
		if (Data.platform >= 10)
		{
			cout << "检测到共用的QQ账号:" << Data.QQID << endl;
			this->isQQOpen = true;
			this->QID = atoi(Data.QQID);
		}
		this->BID = atoi(Data.WBID);
		FriendNode * res;
		FriendList = new FriendNode;
		FriendNode * rear = FriendList;
		FriendList->FriendId = Data.FriendNum;  //第一个结点的Id存放的是好友个数
		for (int i = 0; i < Data.FriendNum; i++)
		{
			res = new FriendNode;
			res->next = NULL;
			res->FriendId = atoi(Data.FriendList[i]);
			res->FriendAccount = atoi(Data.FriendAccount[i]);
			strcpy_s(res->NickName, Data.FriendNickName[i]);
			cout << i + 1 << "个好友ID:  " << res->FriendId
				<< "  NickName:" << res->NickName << endl;
			rear->next = res;
			rear = res;
		}
		cout << "共：" << Data.FriendNum << "个好友" << endl;


		GroupNode * ress;
		GroupList = new GroupNode;
		GroupNode* rearr = GroupList;
		GroupList->GroupId = Data.GroupNum;		 //第一个结点的Id存放的是群组个数
		GroupList->status = atoi(Data.ID);
		for (int i = 0; i < Data.GroupNum; i++)
		{
			ress = new GroupNode;
			ress->next = NULL;
			ress->GroupId = atoi(Data.GroupList[i]);
			strcpy_s(ress->GroupName, Data.GroupName[i]);
			ress->status = Data.GroupStatus[i];
			cout << i + 1 << "个群组GID:" << ress->GroupId << "群名："
				<< ress->GroupName << "身份：" << ress->status << endl;
			rearr->next = ress;
			rearr = ress;
		}

		cout << "初始化MyWeBo请求成功！" << endl;

	}
	else
	{
		cout << "用户不存在MyWeBo账号" << endl;
	}
}
