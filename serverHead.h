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
/*�������Ϳͻ��˵���Ϣ��*/
typedef struct message
{
	/*�������Ϳͻ���֮�������Լ��*/
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
	SOCKET serverAccpetSocket();	//����accept
private:
	WSADATA wsd;
	SOCKET sServer;					//�ͻ����׽���  ��������
	SOCKET temp;					//��������м�ֵ
	SOCKADDR_IN addrServ;			//��������ַ
};
/*��������ƽ̨�����ݿ���*/
class User_mysql
{
public:
	User_mysql();
	int  login(message & Data);		//��ʼ��¼
	int  returnInfo(message & Data);//������ƽ̨����Ϣ
private:
	char user[30];                  //��½����    
	char pswd[20];                  //����
	char host[20];                  //��ʾ�������ݿ�           
	unsigned int port;              //�˿�
	MYSQL myCont;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	int res;
};
/*������΢Xƽ̨�ĺ��ѵ����ݿ���*/
class Friend_mysql
{
public:
	Friend_mysql();
	int addFriend(message & Data);						//��Ӻ���
	int isExist(message & Data);						//���϶��²���΢X�˺��Ƿ����
	int deleteFriend(message & Data);                   //ɾ����Ӧ����
	int setNicknameToFriend(message & Data);			//���ñ�ע
	int findFriend(message & Data);						//���Һ���
	int returnInfo(message & Data);						//��ʼ��Data����Client���ṩMyQQ����Ĺ����������
	int returnDetails(message & Data);
private:
	char user[30];                  //��½����    
	char pswd[20];                  //����
	char host[20];                  //��ʾ�������ݿ�           
	unsigned int port;              //�˿�
	MYSQL myCont;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	int res;
};
/*������΢Xƽ̨��Ⱥ������ݿ���*/
class Group_mysql
{
public:
	Group_mysql();
	int listMember(message & Data);						//��ѯȺ���Ա
	int addGroup(message & Data);						//���Ⱥ
	int dropGroup(message & Data);						//��Ⱥ	
	int kickGroupMember(message & Data);				//Ⱥ��T��
	int creatGroup(message & Data);						//������Ⱥ
	int creatTempGroup(message & Data);					//������ʱ������
	int levelUpGroup(message & Data);					//��ʱ����������ΪȺ��
	int returnInfo(message & Data);						//��ʼ��Data����Client���ṩMyQQ����Ĺ����������
private:
	char user[30];                  //��½����    
	char pswd[20];                  //����
	char host[20];                  //��ʾ�������ݿ�           
	unsigned int port;              //�˿�
	MYSQL myCont;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	int res;
};
/*�����߳�  һ���̶߳�Ӧһ���ͻ���*/
void clientSocketThreadFunction(SOCKET sClient);
/*�����ݿ��ȡ΢Xƽ̨���˺���Ϣ��Ϊ�ͻ���΢X��������׼��*/
void constructMyX(SOCKET sClient,Friend_mysql & Friend,Group_mysql & Group, User_mysql & User);
/*��������ͻ���ͬ������˵�*/
void menu(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);
/*����������ѡ����ѽ���*/
void chooseFriend(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);
/*����������ѡ��Ⱥ�����*/
void chooseGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);
/*������������Ӻ��ѽ���*/
void addFriend(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);
/*�������������Ⱥ�����*/
void addGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);
/*QQ��ר�����ܣ��������䵱�м�վΪ�����ͻ��˷���ʵʱ��Ϣ*/
void chatToFriend(SOCKET sClient, SOCKET sClient_2, message & Data);
/*���������봴��Ⱥ�Ľ���*/
void creatGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);
/*�������������ݿ��е���ȫ��ϢΪ�ͻ��˷�����ͬƽ̨����Ƽ�����*/
void recommendFriend(SOCKET sClient, Friend_mysql & Friend, message & Data);
/*΢�ź�΢�����Ƽ����Ѽ���Ⱥ*/
void recommendGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data);