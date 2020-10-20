#include "serverHead.h"
extern unsigned __int64 client[10][2];
extern char onlineName[10][20];
extern char onlineQQID[10][20];
extern int clientNum;
/*�����߳�  һ���̶߳�Ӧһ���ͻ���*/
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
			std::cout << sClient << "���󣺿�������" << std::endl;
			return;
		}
		switch (User.login(Data))
		{
			//��½�ɹ�
		case 1:cout << Data.ID<<"��½�ɹ�" << endl; 
			Data.ret = 1;
			send(sClient,(char*)&Data,sizeof(Data), 0);
			break;

			//�˺Ų�����
		case 2:cout << "�˺Ų�����" << endl;
			Data.ret = 2;
			send(sClient, (char*)&Data, sizeof(Data), 0);
			break;

			//�������
		case 3:cout << "�������" << endl; 
			Data.ret = 3;
			send(sClient, (char*)&Data, sizeof(Data), 0);
			break;

		default:cout << "δ֪����" << endl; exit(-1);
		}
		//��½�ɹ�������ѭ���������ٴε�½
		if (Data.ret == 1)
		{
			break;
		}
		else
		{
			continue;
		}

	}
	//�����ݿ��в�ѯ���ݣ������ͻ��˹���MyQQ
	constructMyX(sClient, Friend, Group, User);
	//�����ݿ��в�ѯ���ݣ������ͻ��˹���MyWeChat
	constructMyX(sClient, Friend, Group, User);
	//�����ݿ��в�ѯ���ݣ������ͻ��˹���MyWeBo
	constructMyX(sClient, Friend, Group, User);

	//�Կͻ��˵����ݽ��н���
	while (true)
	{
		cout << "���˵��ȴ�ָ���С�����" << endl;
		int ret = recv(sClient, (char*)&Data, sizeof(Data), 0);
		if (SOCKET_ERROR == ret)
		{
			std::cout << sClient << "��������" << std::endl;
			return;
		}
		int flag = 0;
		switch (Data.action)
		{
			//����QQ
		case 1:cout << "�ͻ��˽���QQ" << endl;
			strcpy_s(Data.table,"qq");
			recommendFriend(sClient,Friend,Data);
			menu(sClient, Friend, Group, Data);
			break;
			//����WeChat
		case 2: cout << "�ͻ��˽���΢��" << endl;
			strcpy_s(Data.table, "wc");
			recommendFriend(sClient, Friend, Data);
			menu(sClient, Friend, Group, Data);
			break;
			//����WeBo
		case 3:cout << "�ͻ��˽���΢��" << endl;
			strcpy_s(Data.table, "wb");
			recommendFriend(sClient, Friend, Data);
			menu(sClient, Friend, Group, Data);
			break;
			//��ӡ�ҵ���Ϣ
		case 4: break;		
			//�˳�flag
		case 9: flag = 1; break;		
		default:std::cout << "������� ���������룺" << std::endl;
		}
		if (flag == 1)
		{
			std::cout << "�ͻ��� " << sClient << " ����" << std::endl;
			break;
		}
	}
	
}
/*�����ݿ��ȡ΢Xƽ̨���˺���Ϣ��Ϊ�ͻ���΢X��������׼��*/
void constructMyX(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, User_mysql & User)
{
	mgc Data;
	//user ���첿��
	recv(sClient, (char *)&Data, sizeof(Data), 0);
	if (Data.action == 10)
	{
		cout << "My"<< Data.table <<" - user is constructing" << endl;	
		//��ʾ��׼�������
		Data.action = 1;
		//��ʼ��User ���ݰ�
		User.returnInfo(Data);
		send(sClient, (char *)&Data, sizeof(Data), 0);
	}
	else
	{
		cout << "user construct error!" << endl;
		exit(-1);
	}

	//΢X���첿��
	recv(sClient, (char *)&Data, sizeof(Data), 0);
	if (Data.action == 11)
	{
		cout <<Data.table<<" is constructing" << endl;
		//��ʾ��׼�������
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
/*��������ͻ���ͬ������˵�*/
void menu(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data)
{
	//�Ӳ˵�ѭ��,��Ҫ�ֶ�������һ��
	cout << "�ͻ��˽���"<<Data.table<<"�˵�" << endl;
	while (true)
	{
		recv(sClient, (char *)&Data, sizeof(Data), 0);		//1.���
		send(sClient, (char *)&Data, sizeof(Data), 0);		//2.���
		switch (Data.action)
		{
		case 1://1.��ʾ�ҵ���Ϣ �����������
			break;
		case 2://1.���ú��ѱ�ע/2.����/3.�鿴������Ϣ/4.ɾ������
			cout << "�ͻ���ѡ���˺���" << endl;
			chooseFriend(sClient, Friend, Group, Data);
			break;
		case 3://1.�˳�Ⱥ/2.�Ƽ�������/3.����/4.��ѯ��Ա/5.����Ⱥ
			cout << "�ͻ���ѡ����Ⱥ��" << endl;
			chooseGroup(sClient, Friend, Group, Data);
			break;
		case 4://1.���� X�˺� ֱ����ӳɹ�
			cout << "�ͻ���ѡ������Ӻ���" << endl;
			addFriend(sClient, Friend, Group, Data);
			break;
		case 5://1.���� XȺ�˺� ֱ����ӳɹ�
			if (!strcmp(Data.table, "qq"))
			{
				cout << "�ͻ���ѡ�������Ⱥ��" << endl;
				addGroup(sClient, Friend, Group, Data);
			}
			else
			{
				cout << "�ͻ���ѡ�����Ƽ�Ⱥ�ĸ�����" << endl;
				recommendGroup(sClient, Friend, Group, Data);
			}
			break;
		case 6://��ӡ�����б� //1.�����ڴ� ֱ�Ӵ�ӡ������ϸ��
			cout << "�ͻ���ѡ���˴�ӡ�����б�" << endl;
			break;
		case 7://��ӡȺ���б� //1.�����ڴ� ֱ�Ӵ�ӡ������ϸ��
			cout << "�ͻ���ѡ���˴�ӡȺ���б�" << endl;
			break;
		case 8://����Ⱥ��
			cout << "�ͻ�ѡ���˴���Ⱥ��" << endl;
			creatGroup(sClient, Friend, Group, Data);
			break;
		case 9://������һ��
			cout << "�ͻ���ѡ�����˳�" << endl;
			return;
		default:
			cout << "�����ָ��" << endl;
			break;
		}
	}
}
/*QQ��ר�����ܣ��������䵱�м�վΪ�����ͻ��˷���ʵʱ��Ϣ*/
void chatToFriend(SOCKET sClient, SOCKET sClient_2, message & Data)
{
	while (Data.action != -1)
	{
		recv(sClient, (char *)&Data, sizeof(Data), 0);
		send(sClient_2, (char *)&Data, sizeof(Data), 0);
	}
}
/*����������ѡ����ѽ���*/
void chooseFriend(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data)
{
	while (true)
	{
		recv(sClient, (char *)&Data, sizeof(Data), 0);		//1.��Ӧ���
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

		case 2://2.����
			if (strcmp("qq", Data.table))break;
			Data.FriendNum = clientNum;
			//��ͻ��˷��͵�ǰ�����û�
			for (int i = 0; i < clientNum; i++)
			{
				cout << "onlineQQID��" << i <<"λ��"<< onlineQQID[i] << endl;
				strcpy_s(Data.FriendList[i], onlineQQID[i]);
				strcpy_s(Data.FriendNickName[i], onlineName[i]);
			}
			send(sClient, (char *)&Data, sizeof(Data), 0);
			recv(sClient, (char *)&Data, sizeof(Data), 0);
			if (1 == Data.action)
			{
				chatToFriend(sClient,client[Data.ret][0],Data);
				cout << "һλ�û����˳�����" << endl;
			}
			else
			{
				cout << "�ͻ���ѡ��ĺ��Ѳ��Ϸ�" << endl;
			}
			break;
		case 3://3.�鿴������Ϣ
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
		case 9://������һ��
			return;
		default:
			cout << "chooseFriend..�ͻ��������ָ������....." << endl;
			break;
		}
	}
}
/*����������ѡ��Ⱥ�����*/
void chooseGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data)
{
	while (true)
	{
		int ret;
		recv(sClient, (char *)&Data, sizeof(Data), 0);		//1.��Ӧ���
		send(sClient, (char *)&Data, sizeof(Data), 0);
		switch (Data.action)
		{
		case 1://�˳�Ⱥ��
			recv(sClient, (char *)&Data, sizeof(Data), 0);		//2.��Ӧ���
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
				cout << "�ͻ���ȡ������ '�˳�Ⱥ��' " << endl;
			}
			break;
		case 2://�Ƽ�������
			cout << "΢��Ⱥ �Ƽ��������ڿ�����..." << endl;
			break;
		case 3://����	
			recv(sClient, (char *)&Data, sizeof(Data), 0);
			ret = Group.kickGroupMember(Data);
			if (1 == ret)//�ɹ��߳�
			{
				Data.action = 1;
				send(sClient, (char *)&Data, sizeof(Data), 0);
			}
			//������ϸ��
			else if(-1 == ret)//û�鵽�����
			{
				Data.action = -1;
				send(sClient, (char *)&Data, sizeof(Data), 0);
			}
			else if (0 == ret)//��λ̫��
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
		case 4://��ѯȺ��Ա
			recv(sClient, (char *)&Data, sizeof(Data), 0);
			Group.listMember(Data);
			send(sClient, (char *)&Data, sizeof(Data), 0);
			break;
		case 5://����Ⱥ
			if (strcmp("qq", Data.table))break;
			recv(sClient, (char *)&Data, sizeof(Data), 0);
			Group.levelUpGroup(Data);
			send(sClient, (char *)&Data, sizeof(Data), 0);
			break;
		case 6://������Ⱥ
			if (strcmp("qq", Data.table))break;
			recv(sClient, (char *)&Data, sizeof(Data), 0);
			Data.action = Group.creatTempGroup(Data);
			send(sClient, (char *)&Data, sizeof(Data), 0);
			break;
		case 9://������һ��
			cout << "�ͻ��˷�����"<<Data.table<<"�˵�" << endl;
			return;
		default:
			cout << "chooseGroup..�ͻ��������ָ������....." << endl;
			break;
		}
	}


}
/*������������Ӻ��ѽ���*/
void addFriend(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data)
{
	recv(sClient, (char *)&Data, sizeof(Data), 0);			//1.���
	if (1 == Data.action)
	{
		if (Friend.addFriend(Data))
		{
			Data.action = 1;
			send(sClient, (char *)&Data, sizeof(Data), 0);	//2.���
		}
		else
		{
			Data.action = -1;
			send(sClient, (char *)&Data, sizeof(Data), 0);	//2.���
		}
	}
	else
	{
		cout << "�ͻ���ȡ������ '��Ӻ���' " << endl;
	}
}
/*�������������Ⱥ�����*/
void addGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data)
{
	recv(sClient, (char *)&Data, sizeof(Data), 0);			//1.���
	if (1 == Data.action)
	{
		if (Group.addGroup(Data))
		{
			Data.action = 1;
			send(sClient, (char *)&Data, sizeof(Data), 0);	//2.���
		}
		else
		{
			Data.action = -1;
			send(sClient, (char *)&Data, sizeof(Data), 0);	//2.���
		}
	}
	else
	{
		cout << "�ͻ���ȡ������ '���Ⱥ��' " << endl;
	}
}
/*���������봴��Ⱥ�Ľ���*/
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
/*�������������ݿ��е���ȫ��ϢΪ�ͻ��˷�����ͬƽ̨����Ƽ�����*/
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
/*΢�ź�΢�����Ƽ����Ѽ���Ⱥ*/
void recommendGroup(SOCKET sClient, Friend_mysql & Friend, Group_mysql & Group, message & Data)
{
	recv(sClient, (char *)&Data, sizeof(Data), 0);
	if (-1 == Data.action)
	{
		cout << "�ͻ���ȡ���Ƽ�Ⱥ���������Զ�������һ��" <<endl;
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