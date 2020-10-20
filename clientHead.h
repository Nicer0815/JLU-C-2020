#pragma once
#include"winsock2.h"
#include<iostream>  
#include<cstring>
#include<Windows.h>
#include<string>
#include<stdio.h>
#include<iomanip>
#include<thread>
#include<time.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
typedef struct message{
	int action;       
	char ID[15];
	char table[4];
	int platform = 0;
	char XID[10];
	char QQID[10];
	char WCID[10];
	char WBID[10];
	char GID[10];
	char GMemberID[10];
	int FriendNum;
	char FriendList[20][10];
	char FriendAccount[20][10];
	char FriendNickName[20][20];
	int GroupNum;
	char GroupList[20][10];
	char GroupName[20][20];
	int  GroupStatus[20];
	char paswad[20];
	char Name[21];
	char NickName[21];
	char BirthDay[12];
	int  TAge = 0;
	char Place[41];
	int ret=0;
}mgc;
/*好友信息，非详细*/
struct FriendNode {
	int FriendId = 0;
	int FriendAccount = 0;			//平台总账号,方便推送好友
	char NickName[20];
	struct FriendNode* next = NULL;
};
/*群组信息*/
struct GroupNode {
	int GroupId;
	int status;
	char GroupName[21];
	FriendNode* Memberlist;  //暂时没有实现
	GroupNode* next = NULL;
};

/*客户端socket类*/
class clientSocket
{
public:
	clientSocket(string ipAdress, short port);
	~clientSocket();
	bool connectToServer();
	void receiveData(message & data);
	void sendData(message & data);
private:
	WSADATA wsd;            //WSADATA变量  
	SOCKET sClient;         //客户端套接字  
	SOCKADDR_IN servAddr;   //服务器地址  
	int ret;                //返回值 
};

/*各微X平台抽象类*/
class user {
private:
	int ID;  //平台总ID
protected:
	char Name[21];
	char BirthDay[12];
	int  TAge = 0;
	char Place[21];
	FriendNode* FriendList = 0;
	GroupNode * GroupList = 0;
public:
	user(message & Data, message & rent, clientSocket & mySocket);
	~user();
	int getUserID();											//返回平台总ID
	int dropGroup(int Gid);										//退群(内存)
	int  addFriend(char Fid[], char NickName[]);				//添加好友
	FriendNode* getFriendList();								//获取好友列表
	void deleteFriend(char Fid[]);								//删除好友
	int setFriendNickName(int ID, char Name[]);					//设置好友备注
	void showFriendList();										//打印好友列表
	void showGroupList();										//打印群组列表
	int  checkAddGroup(int Gid);								//添加群检查
	int  checkAddFriend(int Fid);								//添加好友检查
	virtual void showMyInfo() = 0;								//我的微X信息
	virtual int getXID() = 0;									//微X账号
	virtual char * getXName() = 0;								//名字
	virtual void Xmenu() = 0;									//菜单
	virtual int addGroup(char Gid[], char GroupName[]);			//添加群
	virtual int creatGroup(int Gid, char gName[]);				//创建群聊
	virtual int creatTempGroup(int Gid, char gName[]);			//创建群聊
	virtual void ChatToFriend(message & Data, clientSocket & mySocket) = 0;
	virtual void chooseGroup(message & Data, clientSocket & mySocket) = 0;
	bool isExist(message & Data, clientSocket & mySocket);
	void FindFriendInfo(message & Data, clientSocket & mySocket);
	void DeleteFriend(message & Data, clientSocket & mySocket);
	int  SetNickName(message & Data, clientSocket & mySocket);
	void addFriend(message & Data, clientSocket & mySocket);
	void chooseFriend(message & Data, clientSocket & mySocket);
	virtual void addGroup(message & Data, clientSocket & mySocket);
	virtual void creatGroup(message & Data, clientSocket & mySocket);
	void action(message & Data, clientSocket & mySocket);
	void getRecommendFriend(FriendNode* otherFriendList, const char * platform
		, message & Data, clientSocket & mySocket);				//获取推荐好友
};
/* QQ类 */
class MyQQ : public user {
private:
	int QID;
	bool isLinkToWeChat = false;
	int WID;
public:
	MyQQ(message & Data, message & rent, clientSocket & mySocket);
	virtual int getXID();
	virtual char * getXName();
	virtual void showMyInfo();
	virtual void Xmenu();
	virtual void ChatToFriend(message & Data, clientSocket & mySocket);	//QQ独有的点对点通信功能
	virtual void chooseGroup(message & Data, clientSocket & mySocket);
	void QQChatting(message & Data, clientSocket & mySocket);			//QQ独有的点对点通信功能
};
void QQListening(message & Data, clientSocket & mySocket);				//聊天时启用线程进行监听
void menu();
int initlogin(message & Data, message & rent, clientSocket & mySocket);	//初始登陆函数


/* 微信类 */
class MyWeChat : public user {
private:
	int WID;
	bool isLinkToQQ = false;
	int QID;
public:
	MyWeChat(message & Data, message & rent, clientSocket & mySocket);
	virtual int getXID();
	virtual char * getXName();
	virtual void showMyInfo();
	virtual void Xmenu();
	virtual void addGroup(message & Data, clientSocket & mySocket);			//多态成推荐好友加入群
	virtual void chooseGroup(message & Data, clientSocket & mySocket);
	virtual void ChatToFriend(message & Data, clientSocket & mySocket);
};

/* 微博类 */
class MyWeBo :public user {
private:
	int BID;
	bool isQQOpen = false;
	int QID;
public:
	MyWeBo(message & Data, message & rent, clientSocket & mySocket);
	virtual int getXID();
	virtual char * getXName();
	virtual void showMyInfo();
	virtual void Xmenu();
	virtual void addGroup(message & Data, clientSocket & mySocket);			//多态成推荐好友加入群
	virtual void chooseGroup(message & Data, clientSocket & mySocket);
	virtual void ChatToFriend(message & Data, clientSocket & mySocket);
};