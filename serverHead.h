#pragma once
#include "WinSock2.h"  
#include<cstring>
#include <mysql.h>
#include <string>
#include <cstdlib>
#include <list>
#include <cstring>
#include <iterator>
#include <fstream>
#include<iostream>
#include<thread>
#pragma comment(lib, "ws2_32.lib")  
using std::cout;
using std::endl;
/*服务器和客户端的信息包*/
typedef struct message
{
	/*服务器和客户端之间的命令约定*/
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

class serverSocket
{
public:
	serverSocket(int port);
	~serverSocket();
	SOCKET serverAccpetSocket();	//阻塞accept
private:
	WSADATA wsd;
	SOCKET sServer;					//客户端套接字  用来监听
	SOCKET temp;					//用来存放中间值
	SOCKADDR_IN addrServ;			//服务器地址
};
/*服务于总平台的数据库类*/
class User_mysql
{
public:
	User_mysql();
	int  login(message & Data);		//初始登录
	int  returnInfo(message & Data);//返回总平台的信息
private:
	char user[30];                  //登陆名字    
	char pswd[20];                  //密码
	char host[20];                  //表示本地数据库           
	unsigned int port;              //端口
	MYSQL myCont;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	int res;
};
/*服务于微X平台的好友的数据库类*/
class Friend_mysql
{
public:
	Friend_mysql();
	int addFriend(message & Data);						//添加好友
	int isExist(message & Data);						//自上而下查找微X账号是否存在
	int deleteFriend(message & Data);                   //删除相应好友
	int setNicknameToFriend(message & Data);			//设置备注
	int findFriend(message & Data);						//查找好友
	int returnInfo(message & Data);						//初始化Data，向Client端提供MyQQ对象的构造所需参数
	int returnDetails(message & Data);
private:
	char user[30];                  //登陆名字    
	char pswd[20];                  //密码
	char host[20];                  //表示本地数据库           
	unsigned int port;              //端口
	MYSQL myCont;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	int res;
};
/*服务于微X平台的群组的数据库类*/
class Group_mysql
{
public:
	Group_mysql();
	int listMember(message & Data);						//查询群组成员
	int addGroup(message & Data);						//添加群
	int dropGroup(message & Data);						//退群	
	int kickGroupMember(message & Data);				//群内T人
	int creatGroup(message & Data);						//创建新群
	int creatTempGroup(message & Data);					//创建临时讨论组
	int levelUpGroup(message & Data);					//临时讨论组升级为群组
	int returnInfo(message & Data);						//初始化Data，向Client端提供MyQQ对象的构造所需参数
private:
	char user[30];                  //登陆名字    
	char pswd[20];                  //密码
	char host[20];                  //表示本地数据库           
	unsigned int port;              //端口
	MYSQL myCont;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	int res;
};
/*启用线程  一个线程对应一个客户端*/
void clientSocketThreadFunction(SOCKET sClient);
/*从数据库获取微X平台的账号信息，为客户端微X对象构造做准备*/
void constructMyX(SOCKET sClient,Friend_mysql & Friend,Group_mysql & Group, User_mysql & User);
/*服务器与客户端同步进入菜单*/
void menu(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);
/*服务器进入选择好友界面*/
void chooseFriend(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);
/*服务器进入选择群组界面*/
void chooseGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);
/*服务器进入添加好友界面*/
void addFriend(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);
/*服务器进入添加群组界面*/
void addGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);
/*QQ的专属功能，服务器充当中继站为两个客户端发送实时信息*/
void chatToFriend(SOCKET sClient, SOCKET sClient_2, message & Data);
/*服务器进入创建群聊界面*/
void creatGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);
/*服务器根据数据库中的完全信息为客户端反馈不同平台间的推荐好友*/
void recommendFriend(SOCKET sClient, Friend_mysql & Friend, message & Data);
/*微信和微博的推荐好友加入群*/
void recommendGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);