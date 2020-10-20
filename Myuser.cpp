#pragma once
#include"clientHead.h"
#include<iostream>
#include<mysql.h>
using namespace std;
//����ƽ̨��ID
int user::getUserID() {
	return this->ID;
}
//���๹�캯������ʼ��ƽ̨��ID
user::user(message & Data, message & rent, clientSocket & mySocket) {  //���û����ϼ������ڴ�
	//���ͳ�ʼ������
	Data.action = 10;
	mySocket.sendData(Data);
	cout << "��ʼ��user�����ѷ���" << endl;
	mySocket.receiveData(Data);
	if (Data.action == 1)
	{
		this->ID = atoi(Data.ID);
		cout << "��ʼ��user����ɹ�" << endl;
		cout << "user.ID" << Data.ID << endl;
	}
}
//������������
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
	cout << "user �������" << endl;
}
//����QQ���󣬴����ݿ��ȡ���ݣ���ʼ��������Ϣ�������б�Ⱥ���б��
MyQQ::MyQQ(message & Data, message & rent, clientSocket & mySocket) :user(Data, rent, mySocket)
{
	Data.action = 11;
	strcpy_s(Data.XID, Data.QQID);
	mySocket.sendData(Data);
	cout << "��ʼ��MyQQ�����ѷ���" << endl;
	mySocket.receiveData(Data);
	if (Data.action == 1)
	{
		this->TAge = Data.TAge;
		strcpy_s(this->BirthDay, Data.BirthDay);
		strcpy_s(this->Place, Data.Place);
		strcpy_s(this->Name, Data.Name);
		//����΢��
		if (Data.platform >= 10)
		{
			cout << "��⵽�󶨵�΢���˺�:" << Data.WCID << endl;
			this->isLinkToWeChat = true;
			this->WID = atoi(Data.WCID);
		}
		this->QID = atoi(Data.QQID);
		FriendNode * res;
		FriendList = new FriendNode;
		FriendNode * rear = FriendList;
		FriendList->FriendId = Data.FriendNum;  //��һ������Id��ŵ��Ǻ��Ѹ���
		for (int i = 0; i < Data.FriendNum; i++)
		{
			res = new FriendNode;
			res->FriendId = atoi(Data.FriendList[i]);
			res->FriendAccount = atoi(Data.FriendAccount[i]);
			strcpy_s(res->NickName, Data.FriendNickName[i]);
			cout << i+1 << "������ID:  " << res->FriendId 
				<< "  NickName:" << res->NickName << endl;
			rear->next = res;
			rear = res;
		}
		cout << "����" << Data.FriendNum << "������" << endl;


		GroupNode * ress;
		GroupList = new GroupNode;
		GroupNode* rearr = GroupList;
		GroupList->GroupId = Data.GroupNum;		 //��һ������Id��ŵ���Ⱥ�����
		GroupList->status = atoi(Data.ID);
		for (int i = 0; i < Data.GroupNum; i++)
		{
			ress = new GroupNode;
			ress->GroupId = atoi(Data.GroupList[i]);
			strcpy_s(ress->GroupName, Data.GroupName[i]);
			ress->status = Data.GroupStatus[i];
			cout << i+1 << "��Ⱥ��GID:" << ress->GroupId << "Ⱥ����" 
				<< ress->GroupName <<"��ݣ�"<<ress->status<< endl;
			rearr->next = ress;
			rearr = ress;
		}

		cout << "��ʼ��MyQQ����ɹ���" << endl;
		
	}
	else
	{
		cout << "�û�������QQ�˺�" << endl;
	}
}
//��ӡ������ϸ��Ϣ
void MyQQ::showMyInfo()
{
	cout << ">------------------------------------<" << endl;
	cout << "���֣�" << this->Name << endl;
	cout << "QQ�˺ţ�" << this->QID << endl;
	cout << "QQ�䣺" << this->TAge << endl;
	cout << "�������ڣ�" << this->BirthDay << endl;
	cout << "���ڵأ�" << this->Place << endl;
	if (this->isLinkToWeChat)
	{
		cout << "�󶨵�΢���˺�:" << this->WID << endl;
	}
	else
	{
		cout << "δ��΢���˺�" << endl;
	}
	cout << ">------------------------------------<" << endl;
	cout << "�����������������" << endl;
	getchar(); getchar();
}
//��ӡ�����б�
void user::showFriendList()
{
	int num = this->FriendList->FriendId;
	FriendNode * pre = this->FriendList->next;
	cout << "*************************************" << endl;
	cout << "���ѱ�ע---|---�����˺�" << endl;
	while (num--)
	{
		cout <<setw(10)<<pre->NickName <<setw(10)<< pre->FriendId << endl;
		pre = pre->next;
	}
	cout << "*************************************" << endl;
	cout << "�����������������" << endl;
	getchar(); getchar();
}
//��ӡȺ����Ϣ
void user::showGroupList()
{
	int num = this->GroupList->GroupId;
	GroupNode * pre = this->GroupList->next;
	cout << "*************************************" << endl;
	cout << "Ⱥ������---|---Ⱥ���˺�---|---Ⱥ���λ" << endl;
	while (num--)
	{
		cout<<setw(12) <<pre->GroupName 
			<<setw(12) <<pre->GroupId 
			<<setw(12) <<pre->status<< endl;
		pre = pre->next;
	}
	cout << "*************************************" << endl;
	cout << "�����������������" << endl;
	getchar(); getchar();
}
//���ú��ѱ�ע
int user::setFriendNickName(int ID, char Name[])
{
	int Fnum = this->FriendList->FriendId;
	FriendNode * pre = this->FriendList->next;
	while (Fnum--)
	{
		//�ҵ��˺���
		if (pre->FriendId == ID)
		{
			strcpy_s(pre->NickName, Name);
			return 1;
		}
		pre = pre->next;
	}
	return 0;
}	
//����QQ�˺�
int MyQQ::getXID()
{
	return this->QID;
}
//����QQ����
char * MyQQ::getXName()
{
	return this->Name;
}
//��Ӻ���ǰ�ж��Ƿ��ظ����
int  user::checkAddFriend(int Fid)
{
	int friendNum = this->FriendList->FriendId;
	FriendNode * pre = this->FriendList->next;
	//�ж��Ƿ��ظ����
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
//Ⱥ���������
int  user::checkAddGroup(int Gid)
{
	int groupNum = this->GroupList->GroupId;
	GroupNode * pre = this->GroupList->next;
	//�ж��Ƿ��ظ����
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
//Ⱥ���˳�
int  user::dropGroup(int Gid)
{
	int groupNum = this->GroupList->GroupId;
	GroupNode * pre = this->GroupList->next;
	GroupNode * prv = this->GroupList;
	//�ж��Ƿ����
	while (groupNum--)
	{
		//�ҵ�Ŀ�꣬��Ⱥ��
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
//����������(��Ⱥ)
int user::creatTempGroup(int Gid, char gName[])
{
	int groupNum = this->GroupList->GroupId;
	GroupNode * res = new GroupNode;
	res->GroupId = Gid;
	cout << "��Ⱥ����:  " << Gid << endl;
	res->status = 3;
	strcpy_s(res->GroupName, gName);
	res->next = GroupList->next;
	GroupList->next = res;
	GroupList->GroupId++;
	return 1;
}
//Ⱥ�Ĵ���(������)����ʱ������
int user::creatGroup(int Gid, char gName[])
{
	int groupNum = this->GroupList->GroupId;
	GroupNode * res = new GroupNode;
	res->GroupId = Gid;
	cout << "Ⱥ����:  " << Gid << endl;
	res->status = 3;
	strcpy_s(res->GroupName, gName);
	res->next = GroupList->next;
	GroupList->next = res;
	GroupList->GroupId++;
	return 1;
}
//���ӷ��������͹����� ��Ⱥ Dataˢ�µ��ڴ�
int user::addGroup(char Gid[], char GroupName[])
{
	GroupNode * res = new GroupNode;
	res->GroupId = atoi(Gid);
	//�ǵ��÷�������Data
	//���¼����Ⱥ�ŵ�����ͷ��
	strcpy_s(res->GroupName, GroupName);
	res->status = 0;
	res->next = this->GroupList->next;
	this->GroupList->next = res;
	this->GroupList->GroupId++;
	return 1;
}
//���ӷ��������͹����� �Ӻ��� Dataˢ�µ��ڴ�
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
//���ӷ��������͹����� ɾ���� Dataˢ�µ��ڴ�
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
			cout << "�ڴ���ɾ��" << Fid << endl;
			this->FriendList->FriendId--;
			delete pre;
			return;
		}
		pre = pre->next;
		prv = prv->next;
	}
	cout << "���棺�ڴ���û�����ɾ������" << Fid << endl;

}
//��ȡ��ǰ�����б�
FriendNode* user::getFriendList()
{
	return this->FriendList;
}
//��ȡ�Ƽ�����  �����Ƽ��ĸ���
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
		//�ڵ�ǰ�����б���û���ҵ� other ƽ̨���е� loc ����
		if (flag)
		{
			itoa(loc->FriendAccount, Data.ID, 10);
			Data.action = 3;
			if (isExist(Data, mySocket))
			{
				cout << "Ϊ���Ƽ����� " << platform << " �ĺ���:" 
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
//����΢���˺�
int MyWeChat::getXID()
{
	return this->WID;
}
//����΢������
char * MyWeChat::getXName()
{
	return this->Name;
}
//��ӡ������ϸ��Ϣ
void MyWeChat::showMyInfo()
{
	cout << ">------------------------------------<" << endl;
	cout << "���֣�" << this->Name << endl;
	cout << "΢���˺ţ�" << this->WID << endl;
	cout << "΢���䣺" << this->TAge << endl;
	cout << "�������ڣ�" << this->BirthDay << endl;
	cout << "���ڵأ�" << this->Place << endl;
	if (this->isLinkToQQ)
	{
		cout << "�󶨵�QQ�˺�:" << this->QID << endl;
	}
	else
	{
		cout << "δ��΢���˺�" << endl;
	}
	cout << ">------------------------------------<" << endl;
	cout << "�����������������" << endl;
	getchar(); getchar();
}
//΢�Ų˵�
void MyWeChat::Xmenu()
{
	cout << "*****************************" << endl;
	cout << "1.�ҵ���Ϣ" << endl;			//1.��ʾ�ҵ���Ϣ �����������
	cout << "2.ѡ�����" << endl;			//1.���ú��ѱ�ע/2.����/3.�鿴������Ϣ
	cout << "3.ѡ��Ⱥ��" << endl;			//1.�˳�Ⱥ/2.�Ƽ�������/3.����/4.��ѯ��Ա/5.����Ⱥ
	cout << "4.��Ӻ���" << endl;			//1.����΢���˺� ֱ����ӳɹ�
	cout << "5.�Ƽ�Ⱥ��" << endl;			//1.����΢��Ⱥ�˺� �� �����˺� ֱ�������Ѽ���
	cout << "6.��ӡ�����б�" << endl;		//1.�����ڴ� ֱ�Ӵ�ӡ������ϸ��
	cout << "7.��ӡȺ���б�" << endl;		//1.�����ڴ� ֱ�Ӵ�ӡ������ϸ��
	cout << "8.����Ⱥ��" << endl;
	cout << "9.�˳�" << endl;
	cout << "*****************************" << endl;

}
//΢�Ź��캯��
MyWeChat::MyWeChat(message & Data, message & rent, clientSocket & mySocket) :user(Data, rent, mySocket)
{
	Data.action = 11;
	strcpy_s(Data.XID, Data.WCID);
	mySocket.sendData(Data);
	cout << "��ʼ��MyWeChat�����ѷ���" << endl;
	mySocket.receiveData(Data);
	if (Data.action == 1)
	{
		this->TAge = Data.TAge;
		strcpy_s(this->BirthDay, Data.BirthDay);
		strcpy_s(this->Place, Data.Place);
		strcpy_s(this->Name, Data.Name);
		//����QQ
		if (Data.platform >= 10)
		{
			cout << "��⵽�󶨵�QQ�˺�:" << Data.QQID << endl;
			this->isLinkToQQ = true;
			this->QID = atoi(Data.QQID);
		}
		this->WID = atoi(Data.WCID);
		FriendNode * res;
		FriendList = new FriendNode;
		FriendNode * rear = FriendList;
		FriendList->FriendId = Data.FriendNum;  //��һ������Id��ŵ��Ǻ��Ѹ���
		for (int i = 0; i < Data.FriendNum; i++)
		{
			res = new FriendNode;
			res->next = NULL;
			res->FriendId = atoi(Data.FriendList[i]);
			res->FriendAccount = atoi(Data.FriendAccount[i]);
			strcpy_s(res->NickName, Data.FriendNickName[i]);
			cout << i + 1 << "������ID:  " << res->FriendId
				<< "  NickName:" << res->NickName << endl;
			rear->next = res;
			rear = res;
		}
		cout << "����" << Data.FriendNum << "������" << endl;


		GroupNode * ress;
		GroupList = new GroupNode;
		GroupNode* rearr = GroupList;
		GroupList->GroupId = Data.GroupNum;		 //��һ������Id��ŵ���Ⱥ�����
		GroupList->status = atoi(Data.ID);
		for (int i = 0; i < Data.GroupNum; i++)
		{
			ress = new GroupNode;
			ress->next = NULL;
			ress->GroupId = atoi(Data.GroupList[i]);
			strcpy_s(ress->GroupName, Data.GroupName[i]);
			ress->status = Data.GroupStatus[i];
			cout << i + 1 << "��Ⱥ��GID:" << ress->GroupId << "Ⱥ����"
				<< ress->GroupName << "��ݣ�" << ress->status << endl;
			rearr->next = ress;
			rearr = ress;
		}

		cout << "��ʼ��MyWeChat����ɹ���" << endl;

	}
	else
	{
		cout << "�û�������MyWeChat�˺�" << endl;
	}
}
// *��̬* �Ƽ�Ⱥ�������
void MyWeChat::addGroup(message & Data, clientSocket & mySocket)
{
	cout << "���������ID:";
	cin >> Data.XID;
	//���Ǻ���
	if (checkAddFriend(atoi(Data.XID)))
	{
		cout << "�����������ĺ��ѣ������Ƽ�" << endl;
		Data.action = -1;
		mySocket.sendData(Data);
		return;
	}
	cout << "������Ⱥ��ID:";
	cin >> Data.GID;
	if (checkAddGroup(atoi(Data.GID)))
	{
		cout << "�������ڸ�Ⱥ�ڣ������Ƽ�" << endl;
		Data.action = -1;
		mySocket.sendData(Data);
		return;
	}
	Data.action = 1;
	mySocket.sendData(Data);
	mySocket.receiveData(Data);
	if (Data.action == 1)
	{
		cout << "�Ƽ��ɹ����ú����ѱ�������Ⱥ��" << endl;
	}
	//�˲�����
	else if (Data.action == 2)
	{
		cout << "���û������ڣ���������ȷ���˺�" << endl;
	}
	//Ⱥ������
	else if (Data.action == 3)
	{
		cout << "��Ⱥ�����ڣ���������ȷ��Ⱥ�˺�" << endl;
	}
	else
	{
		cout << "δ֪����" << endl;
	}
	cout << "�����������..." << endl;
	getchar(); getchar();
}
// *��̬* �Ƽ�Ⱥ�������
void MyWeBo::addGroup(message & Data, clientSocket & mySocket)
{
	cout << "���������ID:";
	cin >> Data.XID;
	//���Ǻ���
	if (checkAddFriend(atoi(Data.XID)))
	{
		cout << "�����������ĺ��ѣ������Ƽ�" << endl;
		Data.action = -1;
		mySocket.sendData(Data);
		return;
	}
	cout << "������Ⱥ��ID:";
	cin >> Data.GID;
	if (checkAddGroup(atoi(Data.GID)))
	{
		cout << "�������ڸ�Ⱥ�ڣ������Ƽ�" << endl;
		Data.action = -1;
		mySocket.sendData(Data);
		return;
	}
	Data.action = 1;
	mySocket.sendData(Data);
	mySocket.receiveData(Data);
	if (Data.action == 1)
	{
		cout << "�Ƽ��ɹ����ú����ѱ�������Ⱥ��" << endl;
	}
	//�˲�����
	else if (Data.action == 2)
	{
		cout << "���û������ڣ���������ȷ���˺�" << endl;
	}
	//Ⱥ������
	else if (Data.action == 3)
	{
		cout << "��Ⱥ�����ڣ���������ȷ��Ⱥ�˺�" << endl;
	}
	else
	{
		cout << "δ֪����" << endl;
	}
	cout << "�����������..." << endl;
	getchar(); getchar();
}
//����΢���˺�
int MyWeBo::getXID()
{
	return this->BID;
}
//����΢������
char * MyWeBo::getXName()
{
	return this->Name;
}
//��ӡ������ϸ��Ϣ
void MyWeBo::showMyInfo()
{
	cout << ">------------------------------------<" << endl;
	cout << "���֣�" << this->Name << endl;
	cout << "΢���˺ţ�" << this->BID << endl;
	cout << "΢���䣺" << this->TAge << endl;
	cout << "�������ڣ�" << this->BirthDay << endl;
	cout << "���ڵأ�" << this->Place << endl;
	if (this->isQQOpen)
	{
		cout << "���õĵ�QQ�˺�:" << this->QID << endl;
	}
	else
	{
		cout << "δ����qq�˺�" << endl;
	}
	cout << ">------------------------------------<" << endl;
	cout << "�����������������" << endl;
	getchar(); getchar();
}
//΢���˵�
void MyWeBo::Xmenu()
{
	cout << "*****************************" << endl;
	cout << "1.�ҵ���Ϣ" << endl;			//1.��ʾ�ҵ���Ϣ �����������
	cout << "2.ѡ�����" << endl;			//1.���ú��ѱ�ע/2.����/3.�鿴������Ϣ
	cout << "3.ѡ��Ⱥ��" << endl;			//1.�˳�Ⱥ/2.�Ƽ�������/3.����/4.��ѯ��Ա/5.����Ⱥ
	cout << "4.��Ӻ���" << endl;			//1.�����˺� ֱ����ӳɹ�
	cout << "5.�Ƽ�Ⱥ��" << endl;			//1.����΢��Ⱥ�˺� �Ƽ�����ֱ�Ӽ���
	cout << "6.��ӡ�����б�" << endl;		//1.�����ڴ� ֱ�Ӵ�ӡ������ϸ��
	cout << "7.��ӡȺ���б�" << endl;		//1.�����ڴ� ֱ�Ӵ�ӡ������ϸ��
	cout << "8.����Ⱥ��" << endl;
	cout << "9.�˳�" << endl;
	cout << "*****************************" << endl;

}
//΢�����캯��
MyWeBo::MyWeBo(message & Data, message & rent, clientSocket & mySocket) :user(Data, rent, mySocket)
{
	Data.action = 11;
	strcpy_s(Data.XID, Data.WBID);
	mySocket.sendData(Data);
	cout << "��ʼ��MyWeBo�����ѷ���" << endl;
	mySocket.receiveData(Data);
	if (Data.action == 1)
	{
		this->TAge = Data.TAge;
		strcpy_s(this->BirthDay, Data.BirthDay);
		strcpy_s(this->Place, Data.Place);
		strcpy_s(this->Name, Data.Name);
		//����QQ
		if (Data.platform >= 10)
		{
			cout << "��⵽���õ�QQ�˺�:" << Data.QQID << endl;
			this->isQQOpen = true;
			this->QID = atoi(Data.QQID);
		}
		this->BID = atoi(Data.WBID);
		FriendNode * res;
		FriendList = new FriendNode;
		FriendNode * rear = FriendList;
		FriendList->FriendId = Data.FriendNum;  //��һ������Id��ŵ��Ǻ��Ѹ���
		for (int i = 0; i < Data.FriendNum; i++)
		{
			res = new FriendNode;
			res->next = NULL;
			res->FriendId = atoi(Data.FriendList[i]);
			res->FriendAccount = atoi(Data.FriendAccount[i]);
			strcpy_s(res->NickName, Data.FriendNickName[i]);
			cout << i + 1 << "������ID:  " << res->FriendId
				<< "  NickName:" << res->NickName << endl;
			rear->next = res;
			rear = res;
		}
		cout << "����" << Data.FriendNum << "������" << endl;


		GroupNode * ress;
		GroupList = new GroupNode;
		GroupNode* rearr = GroupList;
		GroupList->GroupId = Data.GroupNum;		 //��һ������Id��ŵ���Ⱥ�����
		GroupList->status = atoi(Data.ID);
		for (int i = 0; i < Data.GroupNum; i++)
		{
			ress = new GroupNode;
			ress->next = NULL;
			ress->GroupId = atoi(Data.GroupList[i]);
			strcpy_s(ress->GroupName, Data.GroupName[i]);
			ress->status = Data.GroupStatus[i];
			cout << i + 1 << "��Ⱥ��GID:" << ress->GroupId << "Ⱥ����"
				<< ress->GroupName << "��ݣ�" << ress->status << endl;
			rearr->next = ress;
			rearr = ress;
		}

		cout << "��ʼ��MyWeBo����ɹ���" << endl;

	}
	else
	{
		cout << "�û�������MyWeBo�˺�" << endl;
	}
}
