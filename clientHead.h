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
/*������Ϣ������ϸ*/
struct FriendNode {
	int FriendId = 0;
	int FriendAccount = 0;			//ƽ̨���˺�,�������ͺ���
	char NickName[20];
	struct FriendNode* next = NULL;
};
/*Ⱥ����Ϣ*/
struct GroupNode {
	int GroupId;
	int status;
	char GroupName[21];
	FriendNode* Memberlist;  //��ʱû��ʵ��
	GroupNode* next = NULL;
};

/*�ͻ���socket��*/
class clientSocket
{
public:
	clientSocket(string ipAdress, short port);
	~clientSocket();
	bool connectToServer();
	void receiveData(message & data);
	void sendData(message & data);
private:
	WSADATA wsd;            //WSADATA����  
	SOCKET sClient;         //�ͻ����׽���  
	SOCKADDR_IN servAddr;   //��������ַ  
	int ret;                //����ֵ 
};

/*��΢Xƽ̨������*/
class user {
private:
	int ID;  //ƽ̨��ID
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
	int getUserID();											//����ƽ̨��ID
	int dropGroup(int Gid);										//��Ⱥ(�ڴ�)
	int  addFriend(char Fid[], char NickName[]);				//��Ӻ���
	FriendNode* getFriendList();								//��ȡ�����б�
	void deleteFriend(char Fid[]);								//ɾ������
	int setFriendNickName(int ID, char Name[]);					//���ú��ѱ�ע
	void showFriendList();										//��ӡ�����б�
	void showGroupList();										//��ӡȺ���б�
	int  checkAddGroup(int Gid);								//���Ⱥ���
	int  checkAddFriend(int Fid);								//��Ӻ��Ѽ��
	virtual void showMyInfo() = 0;								//�ҵ�΢X��Ϣ
	virtual int getXID() = 0;									//΢X�˺�
	virtual char * getXName() = 0;								//����
	virtual void Xmenu() = 0;									//�˵�
	virtual int addGroup(char Gid[], char GroupName[]);			//���Ⱥ
	virtual int creatGroup(int Gid, char gName[]);				//����Ⱥ��
	virtual int creatTempGroup(int Gid, char gName[]);			//����Ⱥ��
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
		, message & Data, clientSocket & mySocket);				//��ȡ�Ƽ�����
};
/* QQ�� */
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
	virtual void ChatToFriend(message & Data, clientSocket & mySocket);	//QQ���еĵ�Ե�ͨ�Ź���
	virtual void chooseGroup(message & Data, clientSocket & mySocket);
	void QQChatting(message & Data, clientSocket & mySocket);			//QQ���еĵ�Ե�ͨ�Ź���
};
void QQListening(message & Data, clientSocket & mySocket);				//����ʱ�����߳̽��м���
void menu();
int initlogin(message & Data, message & rent, clientSocket & mySocket);	//��ʼ��½����


/* ΢���� */
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
	virtual void addGroup(message & Data, clientSocket & mySocket);			//��̬���Ƽ����Ѽ���Ⱥ
	virtual void chooseGroup(message & Data, clientSocket & mySocket);
	virtual void ChatToFriend(message & Data, clientSocket & mySocket);
};

/* ΢���� */
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
	virtual void addGroup(message & Data, clientSocket & mySocket);			//��̬���Ƽ����Ѽ���Ⱥ
	virtual void chooseGroup(message & Data, clientSocket & mySocket);
	virtual void ChatToFriend(message & Data, clientSocket & mySocket);
};