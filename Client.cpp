#include"clientHead.h"
//��½�����������ƽ̨�˺����룩
int initlogin(message & Data, message & rent, clientSocket & mySocket)
{
	cout << "�������˺�:";
	cin >> Data.ID;
	cout << "����������:";
	cin >> Data.paswad;
	int i = sizeof(Data);
	mySocket.sendData(Data);
	cout << "initlogin�����ѷ���" << endl;
	mySocket.receiveData(Data);
	if (Data.ret == 1)
	{
		cout << Data.ID << "��½�ɹ�" << endl;
		return 1;
	}
	else if (Data.ret == 2)
	{
		cout << Data.ID << "�˺Ų�����" << endl;
		return 2;
	}
	else if (Data.ret == 3)
	{
		cout << Data.ID << "�������" << endl;
		return 3;
	}
	else
	{
		cout << Data.ret << endl;
		cout << "δ֪����" << endl;

		exit(-1);
	}
}
//��ʼ�˵�
void menu()
{
	cout << "*****************************" << endl;
	cout << endl;
	cout << "1.����QQ" << endl;
	cout << "2.����WeChat" << endl;
	cout << "3.����WeBo" << endl;
	cout << "9.�˳�" << endl;
	cout << endl;
	cout << "*****************************" << endl;
}
//΢X�˵�
void MyQQ::Xmenu()
{
	cout << "*****************************" << endl;
	cout << "1.�ҵ���Ϣ" << endl;			//1.��ʾ�ҵ���Ϣ �����������
	cout << "2.ѡ�����" << endl;			//1.���ú��ѱ�ע/2.����/3.�鿴������Ϣ
	cout << "3.ѡ��Ⱥ��" << endl;			//1.�˳�Ⱥ/2.�Ƽ�������/3.����/4.��ѯ��Ա/5.����Ⱥ
	cout << "4.��Ӻ���" << endl;			//1.����QQ�˺� ֱ����ӳɹ�
	cout << "5.���Ⱥ��" << endl;			//1.����QQȺ�˺� ֱ����ӳɹ�
	cout << "6.��ӡ�����б�" << endl;		//1.�����ڴ� ֱ�Ӵ�ӡ������ϸ��
	cout << "7.��ӡȺ���б�" << endl;		//1.�����ڴ� ֱ�Ӵ�ӡ������ϸ��
	cout << "8.����Ⱥ��" << endl;
	cout << "9.�˳�" << endl;
	cout << "*****************************" << endl;

}
//�������
void user::action(message & Data, clientSocket & mySocket)
{
	//ѭ��ʽ���棬��Ҫ�ֶ�������һ��
	while (true)
	{
		Xmenu();
		cout << "�����������";
		cin >> Data.action;
		int temp = Data.action;
		mySocket.sendData(Data);				//1.���
		cout << "�������󡣡���������" << endl;
		mySocket.receiveData(Data);				//2.���
		//������ָͬ���ʾ��������׼����
		if (temp == Data.action)
		{
			switch (Data.action)
			{
			case 1://1.��ʾ�ҵ���Ϣ �����������
				showMyInfo();
				break;
			case 2://ѡ����� //1.���ú��ѱ�ע/2.����/3.�鿴������Ϣ
				chooseFriend(Data, mySocket);
				break;
			case 3://ѡ��Ⱥ�� //1.�˳�Ⱥ/2.�Ƽ�������/3.����/4.��ѯ��Ա/5.����Ⱥ
				chooseGroup(Data, mySocket);
				break;
			case 4://��Ӻ��� //1.����QQ�˺� ֱ����ӳɹ�
				addFriend(Data, mySocket);
				break;
			case 5://���Ⱥ�� //1.����QQȺ�˺� ֱ����ӳɹ�
				addGroup(Data, mySocket);
				break;
			case 6://��ӡ�����б� //1.�����ڴ� ֱ�Ӵ�ӡ������ϸ��
				showFriendList();
				break;
			case 7://��ӡȺ���б� //1.�����ڴ� ֱ�Ӵ�ӡ������ϸ��
				showGroupList();
				break;
			case 8://����Ⱥ��
				creatGroup(Data, mySocket);
				break;
			case 9://������һ��
				return;
			default:
				cout << "ָ�����" << endl;
				break;
			};

		}
		else
		{
			cout << "����ʧ�ܣ�ǿ������" << endl;
			exit(-1);

		}

	}
}
//���ú��ѱ�ע(�޸ĺ�����Ϣ)
int user::SetNickName(message & Data, clientSocket & mySocket)
{
	cout << "���������"<<Data.table<<"�˺�:";
	cin >> Data.XID;
	cout << "���ú��ѱ�ע��";
	cin >> Data.NickName;
	//�ڴ����ҵ�����(�ж��Ǻ���) ���ҳɹ��޸ı�ע/�ǳ�
	if (setFriendNickName(atoi(Data.XID), Data.NickName))
	{
		itoa(getXID(), Data.ID, 10);
		mySocket.sendData(Data);		//�÷�����ȥ�����ݿ�
		mySocket.receiveData(Data);		//���շ���
		if (1 == Data.action)
		{
			cout << "���ú��ѱ�ע�ɹ���" << endl;
			cout << "�����������..." << endl;
			getchar(); getchar();
			return 1;
		}
		else
		{
			cout << "�����ݿ������ñ�עʧ��" << endl;
			cout << "�����������..." << endl;
			getchar(); getchar();
			return -1;
		}
	}
	else
	{
		//ȡ���������ݿ�
		Data.action = -1;
		mySocket.sendData(Data);
		cout << "����"<< Data.table <<"�˺����������������ĺ���/���û�������" << endl;
		cout << "�����������..." << endl;
		getchar(); getchar();
		return 0;
	}
}
//���Һ�����Ϣ����ӡ��
void user::FindFriendInfo(message & Data, clientSocket & mySocket)
{
	cout << "����������˺�:";
	cin >> Data.XID;
	//myqq.Isfriend();
	mySocket.sendData(Data);
	mySocket.receiveData(Data);
	//����Ϸ�
	if (1 == Data.action)
	{
		cout << ">------------------------------------<" << endl;
		cout << Data.table<<"���֣�" << Data.Name << endl;
		cout << Data.table << "�˺ţ�" << Data.XID << endl;
		cout << Data.table << "�䣺" << Data.TAge << endl;
		cout << "�������ڣ�" << Data.BirthDay << endl;
		cout << "���ڵأ�" << Data.Place << endl;
		cout << ">------------------------------------<" << endl;
		cout << "�����������������" << endl;
		getchar(); getchar();
	}
	else
	{
		cout << "�����ݿ���û�в鵽��" << Data.table << "�˺�" << endl;
		cout << "�����������������" << endl;
		getchar(); getchar();
	}
}
//ɾ��QQ����
void user::DeleteFriend(message & Data, clientSocket & mySocket)
{
	cout << "��������Ҫɾ���ĺ���"<<Data.table<<"�˺�:";
	cin >> Data.XID;
	if (!checkAddFriend(atoi(Data.XID)))
	{
		itoa(getXID(), Data.ID, 10);
		mySocket.sendData(Data);				 //�÷�����ȥ�������ݿⲢ���	1.���
		mySocket.receiveData(Data);				 //���շ���						2.���
		if (1 == Data.action)
		{
			deleteFriend(Data.XID);			//ͬ���ڴ�
			cout << "ɾ�����ѳɹ���" << endl;
			cout << "�����������..." << endl;
			getchar(); getchar();
		}
		else
		{
			cout << "�����ݿ�����Ӻ���ʧ��" << endl;
			cout << "�����������..." << endl;
			getchar(); getchar();

		}
	}
	else
	{
		Data.action = -1;
		mySocket.sendData(Data);
		cout << "�����������ĺ��ѣ�����ɾ��" << endl;
	}

}
//ѡ����� //1.���ú��ѱ�ע/2.����/3.�鿴������Ϣ/4.������һ��
void user::chooseFriend(message & Data, clientSocket & mySocket)
{
	while (true)
	{
		cout << "*************************" << endl;
		cout << "1.���ú��ѱ�ע" << endl;
		cout << "2.ѡ���������" << endl;
		cout << "3.�鿴������Ϣ" << endl;
		cout << "4.ɾ������" << endl;
		cout << "9.������һ��" << endl;
		cout << "*************************" << endl;
		cout << "������ָ�";
		cin >> Data.action;
		mySocket.sendData(Data);		//1.��Ӧ���
		cout << "�������������������" << endl;
		mySocket.receiveData(Data);		
		switch (Data.action)
		{
		case 1://1.���ú��ѱ�ע
			SetNickName(Data, mySocket);
			break;
		case 2://2.����
			ChatToFriend(Data, mySocket);
			break;
		case 3://3.�鿴������Ϣ
			FindFriendInfo(Data, mySocket);
			break;
		case 4:
			DeleteFriend(Data, mySocket);
			break;
		case 9://������һ��
			return;
		default:
			cout << "�����ָ������" << endl;
			break;
		}
	}

}
//������캯��
void MyQQ::ChatToFriend(message & Data, clientSocket & mySocket)
{
	mySocket.receiveData(Data);
	cout << "��ǰ����" << Data.FriendNum << "λ����" << endl;
	int num = Data.FriendNum;
	
	for(int i = 0; i < num; i++)
	{
		cout <<setw(12)<< Data.FriendList[i] 
			 <<setw(12)<< Data.FriendNickName[i]
			 << endl;
		
	}
	cout << "����������˺�:";
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
		cout << "���Ѳ����ߣ��������Թ������ڿ�����" << endl;
		return;
	}
}
//�̣߳�listen ���ղ���ӡ������������������Ϣ(���/ʱ���ʶ)
void QQListening(message & Data, clientSocket & mySocket)
{
	cout << "�������߳̽�����" << endl;
	while (true)
	{
		mySocket.receiveData(Data);
		//����  ��  ʱ��
		cout << Data.Name << "--" << Data.Place;
		cout << Data.paswad << endl;
		if (Data.action == -1)
		{
			cout << "�Է����˳�����,�ҷ�ֹͣ��������,�����˳�" << endl;
			break;
		}
	}
}
//��ʼ����
void MyQQ::QQChatting(message & Data, clientSocket & mySocket)
{
	//�����̣߳�������������������Ϣ
	//ע�⣬�������͵Ĳ���Ҫʹ��ref()����
	message ret;
	std::thread t1(&::QQListening, ref(ret), ref(mySocket));			
	t1.detach();
	strcpy_s(Data.Name, getXName());

	cout << "���롮#����������" << endl;
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
	cout << "��ȴ��˳���ʾ" << endl;
	getchar();
	getchar();
	cout << "�ɹ��˳�" << endl;
	//ע�� �߳� t1 ������������
	//�����˳�ʱҪ�ȴ�˫���໥���֣���ֹ����������
}
//ѡ��Ⱥ�� //1.�˳�Ⱥ/2.�Ƽ�������(��ʵ��)/3.����/4.��ѯ��Ա/5.����Ⱥ/6.������Ⱥ
void MyQQ::chooseGroup(message & Data, clientSocket & mySocket)
{
	while (true)
	{
		cout << "*************************" << endl;
		cout << "1.�˳�Ⱥ��" << endl;
		cout << "2.��Ⱥ���Ƽ�������" << endl;
		cout << "3.Ⱥ����" << endl;
		cout << "4.��ѯȺ��Ա" << endl;
		cout << "5.����Ⱥ" << endl;
		cout << "6.����������(��Ⱥ)" << endl;
		cout << "9.������һ��" << endl;
		cout << "*************************" << endl;
		cout << "������ָ�";
		cin >> Data.action;
		mySocket.sendData(Data);		//1.��Ӧ���
		cout << "�������������������" << endl;
		mySocket.receiveData(Data);
		switch (Data.action)
		{
		case 1://�˳�Ⱥ��
			cout << "������Ⱥ��ID��";
			cin >> Data.GID;
			//�ڴ����ҵ���Ⱥ���˳�
			if (dropGroup(atoi(Data.GID)))
			{
				Data.action = 1;
				itoa(getXID(), Data.XID, 10);
				mySocket.sendData(Data);		//�÷�����ȥ�����ݿ�    2.��Ӧ���
				mySocket.receiveData(Data);		//���շ���
				if (1 == Data.action)
				{
					cout << "�˳�Ⱥ�ĳɹ���" << endl;
					cout << "�����������..." << endl;
					getchar(); getchar();
					break;
				}
				else
				{
					cout << "�����ݿ����˳�Ⱥ��ʧ��" << endl;
					cout << "�����������..." << endl;
					getchar(); getchar();
					return;
				}

			}
			else
			{
				//����Ⱥ��Ա��ȡ���������ݿ�
				Data.action = -1;
				mySocket.sendData(Data);
				cout << "����QQȺ���˺���������δ�����Ⱥ��/Ⱥ�Ĳ�����" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			break;
		case 2://�Ƽ�������
			cout << "Sorry QQ��֧�ִ˲�����������" << endl;
			cout << "�����������..." << endl;
			getchar(); getchar();
			break;
		case 3://����
			cout << "������Ⱥ�˺�:";
			cin >> Data.GID;
			cout << "�������ԱQQ�˺ţ�";
			cin >> Data.XID;
			itoa(getXID(),Data.ID,10);
			mySocket.sendData(Data);
			//��������������
			mySocket.receiveData(Data);
			if (1 == Data.action)
			{
				cout << "���ѽ�" << Data.XID << "�߳�Ⱥ��" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			else if(-1 == Data.action)
			{
				cout << "�߳�ʧ�ܣ��ó�Ա�����ڻ�Ⱥ������" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			else if (0 == Data.action)
			{
				cout << "�߳�ʧ�ܣ�����Ⱥ��λ������" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			else
			{
				cout << "�߳�ʧ�ܣ�������������~~~(���ݿ��߳�������)" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			break;
		case 4://��ѯȺ��Ա
			cout << "������Ⱥ���룺";
			cin >> Data.GID;
			mySocket.sendData(Data);
			mySocket.receiveData(Data);
			cout << ">--------------------------<" << endl;
			for (int i = 0; i < Data.GroupNum; i++)
			{
				cout << "�˺�:" << Data.GroupList[i]<<"     ";
				cout << "����:" << Data.GroupName[i]<<"     ";
				cout << "���:" << Data.GroupStatus[i] << endl;
			}
			cout << "�� " << Data.GroupNum << " ����Ա" << endl;
			cout << ">--------------------------<" << endl;
			break;
		case 5://����Ⱥ
			cout << "������Ⱥ���룺";
			itoa(getXID(), Data.ID, 10);
			cin >> Data.GID;
			mySocket.sendData(Data);
			mySocket.receiveData(Data);
			if (1 == Data.action)
			{
				cout << "�����ɹ�����Ⱥ��������������Ϊ����QQȺ" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			else if (0 == Data.action)
			{
				cout << "����ʧ�ܣ���Ⱥ�Ѿ��ǳ���QQȺ(��߼�)�޷���������" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			else
			{
				cout << "����ʧ�ܣ������Ǹ�ȺȺ��" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			break;
		case 6:
			cout << "����Ⱥ�˺ţ�";
			cin >> Data.GID;
			cout << "��������Ⱥ���ƣ�";
			cin >> Data.GroupName[1];
			itoa(getXID(), Data.XID, 10);
			strcpy_s(Data.Name, getXName());
			mySocket.sendData(Data);
			mySocket.receiveData(Data);
			if (1 == Data.action)
			{
				creatTempGroup(atoi(Data.GID), Data.GroupName[0]);
				cout << "�����ɹ�" << endl;

			}
			else
			{
				cout << "����ʧ��,�����ڸ�Ⱥ��" << endl;
			}
			break;
		case 9://������һ��
			return;
		default:
			cout << "�����ָ������" << endl;
			cout << "�����������������" << endl;
			getchar(); getchar();
			break;
		}
	}
}
//��Ӻ���
void user::addFriend(message & Data, clientSocket & mySocket)
{
	//�Ƽ�����
	cout << "��������Ҫ��ӵĺ���"<< Data.table <<"�˺�:";
	cin >> Data.XID;
	cout << "��������Ϊ�������õı�ע/�ǳ�:";
	cin >> Data.NickName;
	if (checkAddFriend(atoi(Data.QQID)))
	{
		Data.action = 1;
		itoa(getXID(), Data.ID, 10);
		itoa(getUserID(), Data.GID, 10);	 //��GID��ʱ����ƽ̨��ID
		strcpy_s(Data.Name, getXName());	 //���˺�����
		mySocket.sendData(Data);				 //�÷�����ȥ�������ݿⲢ���	1.���
		mySocket.receiveData(Data);				 //���շ���						2.���
		if (1 == Data.action)
		{
			addFriend(Data.XID, Data.NickName);
			cout << "��Ӻ��ѳɹ���" << endl;
			cout << "�����������..." << endl;
			getchar(); getchar();
		}
		else
		{
			cout << "�����ݿ�����Ӻ���ʧ��" << endl;
			cout << "�����������..." << endl;
			getchar(); getchar();
		}
	}
	else
	{
		//�Ѿ��Ǻ��ѣ�ȡ����������
		Data.action = -1;
		mySocket.sendData(Data);						//1.���
		cout << "������Ӹú��ѣ������ظ����" << endl;
		cout << "�����������..." << endl;
		getchar(); getchar();

	}

}
//���Ⱥ��
void user::addGroup(message & Data, clientSocket & mySocket)
{
	cout << "������Ⱥ��ID��";
	cin >> Data.GID;
	//�û�û�м����Ⱥ
	if (checkAddGroup(atoi(Data.GID)))
	{
		Data.action = 1;
		itoa(getXID(), Data.XID, 10);
		strcpy_s(Data.Name, getXName());
		mySocket.sendData(Data);		//�÷�����ȥ�������ݿⲢ���	1.���
		mySocket.receiveData(Data);		//���շ���						2.���
		if (1 == Data.action)
		{
			addGroup(Data.GID,Data.GroupName[0]);
			cout << "���Ⱥ�ĳɹ���" << endl;
			cout << "�����������..." << endl;
			getchar(); getchar();
			
		}
		else
		{
			cout << "�����ݿ����˳�Ⱥ��ʧ��" << endl;
			cout << "�����������..." << endl;
			getchar(); getchar();
			
		}
	}
	else
	{
		//�Ѿ���Ⱥ��Ա��ȡ����Ⱥ����
		Data.action = -1;
		mySocket.sendData(Data);						//1.���
		cout << "������Ӹ�Ⱥ�������ظ����" << endl;
		cout << "�����������..." << endl;
		getchar(); getchar();
	}
		
}
//����Ⱥ��
void user::creatGroup(message & Data, clientSocket & mySocket)
{
	cout << "����Ⱥ���ƣ�";
	cin >> Data.GroupName[1];
	itoa(getXID(), Data.GroupList[1], 10);
	strcpy_s(Data.Name, getXName());
	cout << "���ڴ�����������" << endl;
	mySocket.sendData(Data);
	mySocket.receiveData(Data);
	if (1 == Data.action)
	{
		//�ڴ��Ⱥ
		creatGroup(atoi(Data.GroupList[0]), Data.GroupName[1]);
		cout << "����Ⱥ�ĳɹ�,����ǰ��Ⱥ��" << endl;
	}
	else
	{
		cout << "����Ⱥ��ʧ��" << endl;
	}
}
//ѡ��Ⱥ�� //1.�˳�Ⱥ/2.�Ƽ�������(��ʵ��)/3.����/4.��ѯ��Ա
void MyWeChat::chooseGroup(message & Data, clientSocket & mySocket)
{
	while (true)
	{
		cout << "*************************" << endl;
		cout << "1.�˳�Ⱥ��" << endl;
		cout << "2.��Ⱥ���Ƽ�������" << endl;
		cout << "3.Ⱥ����" << endl;
		cout << "4.��ѯȺ��Ա" << endl;
		cout << "9.������һ��" << endl;
		cout << "*************************" << endl;
		cout << "������ָ�";
		cin >> Data.action;
		mySocket.sendData(Data);		//1.��Ӧ���
		cout << "�������������������" << endl;
		mySocket.receiveData(Data);
		switch (Data.action)
		{
		case 1://�˳�Ⱥ��
			cout << "������Ⱥ��ID��";
			cin >> Data.GID;
			//�ڴ����ҵ���Ⱥ���˳�
			if (dropGroup(atoi(Data.GID)))
			{
				Data.action = 1;
				itoa(getXID(), Data.XID, 10);
				mySocket.sendData(Data);		//�÷�����ȥ�����ݿ�    2.��Ӧ���
				mySocket.receiveData(Data);		//���շ���
				if (1 == Data.action)
				{
					cout << "�˳�Ⱥ�ĳɹ���" << endl;
					cout << "�����������..." << endl;
					getchar(); getchar();
					break;
				}
				else
				{
					cout << "�����ݿ����˳�Ⱥ��ʧ��" << endl;
					cout << "�����������..." << endl;
					getchar(); getchar();
					return;
				}

			}
			else
			{
				//����Ⱥ��Ա��ȡ���������ݿ�
				Data.action = -1;
				mySocket.sendData(Data);
				cout << "����QQȺ���˺���������δ�����Ⱥ��/Ⱥ�Ĳ�����" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			break;
		case 2://�Ƽ�������
			cout << "Sorry ΢��Ⱥ�Ƽ� �������ڿ����С�������" << endl;
			cout << "�����������..." << endl;
			getchar(); getchar();
			break;
		case 3://����
			cout << "������Ⱥ�˺�:";
			cin >> Data.GID;
			cout << "�������ԱWeChat�˺ţ�";
			cin >> Data.XID;
			itoa(getXID(), Data.ID, 10);
			mySocket.sendData(Data);
			//��������������
			mySocket.receiveData(Data);
			if (1 == Data.action)
			{
				cout << "���ѽ�" << Data.WCID << "�߳�Ⱥ��" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			else if (-1 == Data.action)
			{
				cout << "�߳�ʧ�ܣ��ó�Ա�����ڻ�Ⱥ������" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			else if (0 == Data.action)
			{
				cout << "�߳�ʧ�ܣ�����Ⱥ��λ������" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			else
			{
				cout << "�߳�ʧ�ܣ�������������~~~(���ݿ��߳�������)" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			break;
		case 4://��ѯȺ��Ա
			cout << "������Ⱥ���룺";
			cin >> Data.GID;
			mySocket.sendData(Data);
			mySocket.receiveData(Data);
			cout << ">--------------------------<" << endl;
			for (int i = 0; i < Data.GroupNum; i++)
			{
				cout << "�˺�:" << Data.GroupList[i] << "     ";
				cout << "����:" << Data.GroupName[i] << "     ";
				cout << "���:" << Data.GroupStatus[i] << endl;
			}
			cout << "�� " << Data.GroupNum << " ����Ա" << endl;
			cout << ">--------------------------<" << endl;
			break;
		case 9://������һ��
			return;
		default:
			cout << "�����ָ������" << endl;
			cout << "�����������������" << endl;
			getchar(); getchar();
			break;
		}
	}
}
//΢�ŵĿչ���
void MyWeChat::ChatToFriend(message & Data, clientSocket & mySocket)
{
	cout << "΢���ݲ�֧�ּ�ʱͨѶ����Ŷ~~" << endl;
}
//ѡ��Ⱥ�� //1.�˳�Ⱥ/2.�Ƽ�������(��ʵ��)/3.����/4.��ѯ��Ա
void MyWeBo::chooseGroup(message & Data, clientSocket & mySocket)
{
	while (true)
	{
		cout << "*************************" << endl;
		cout << "1.�˳�Ⱥ��" << endl;
		cout << "2.��Ⱥ���Ƽ�������" << endl;
		cout << "3.Ⱥ����" << endl;
		cout << "4.��ѯȺ��Ա" << endl;
		cout << "9.������һ��" << endl;
		cout << "*************************" << endl;
		cout << "������ָ�";
		cin >> Data.action;
		mySocket.sendData(Data);		//1.��Ӧ���
		cout << "�������������������" << endl;
		mySocket.receiveData(Data);
		switch (Data.action)
		{
		case 1://�˳�Ⱥ��
			cout << "������Ⱥ��ID��";
			cin >> Data.GID;
			//�ڴ����ҵ���Ⱥ���˳�
			if (dropGroup(atoi(Data.GID)))
			{
				Data.action = 1;
				itoa(getXID(), Data.XID, 10);
				mySocket.sendData(Data);		//�÷�����ȥ�����ݿ�    2.��Ӧ���
				mySocket.receiveData(Data);		//���շ���
				if (1 == Data.action)
				{
					cout << "�˳�Ⱥ�ĳɹ���" << endl;
					cout << "�����������..." << endl;
					getchar(); getchar();
					break;
				}
				else
				{
					cout << "�����ݿ����˳�Ⱥ��ʧ��" << endl;
					cout << "�����������..." << endl;
					getchar(); getchar();
					return;
				}

			}
			else
			{
				//����Ⱥ��Ա��ȡ���������ݿ�
				Data.action = -1;
				mySocket.sendData(Data);
				cout << "����QQȺ���˺���������δ�����Ⱥ��/Ⱥ�Ĳ�����" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			break;
		case 2://�Ƽ�������
			cout << "Sorry ΢��Ⱥ�Ƽ� �������ڿ����С�������" << endl;
			cout << "�����������..." << endl;
			getchar(); getchar();
			break;
		case 3://����
			cout << "������Ⱥ�˺�:";
			cin >> Data.GID;
			cout << "�������ԱWeChat�˺ţ�";
			cin >> Data.XID;
			itoa(getXID(), Data.ID, 10);
			mySocket.sendData(Data);
			//��������������
			mySocket.receiveData(Data);
			if (1 == Data.action)
			{
				cout << "���ѽ�" << Data.WCID << "�߳�Ⱥ��" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			else if (-1 == Data.action)
			{
				cout << "�߳�ʧ�ܣ��ó�Ա�����ڻ�Ⱥ������" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			else if (0 == Data.action)
			{
				cout << "�߳�ʧ�ܣ�����Ⱥ��λ������" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			else
			{
				cout << "�߳�ʧ�ܣ�������������~~~(���ݿ��߳�������)" << endl;
				cout << "�����������..." << endl;
				getchar(); getchar();
			}
			break;
		case 4://��ѯȺ��Ա
			cout << "������Ⱥ���룺";
			cin >> Data.GID;
			mySocket.sendData(Data);
			mySocket.receiveData(Data);
			cout << ">--------------------------<" << endl;
			for (int i = 0; i < Data.GroupNum; i++)
			{
				cout << "�˺�:" << Data.GroupList[i] << "     ";
				cout << "����:" << Data.GroupName[i] << "     ";
				cout << "���:" << Data.GroupStatus[i] << endl;
			}
			cout << "�� " << Data.GroupNum << " ����Ա" << endl;
			cout << ">--------------------------<" << endl;
			break;
		case 9://������һ��
			return;
		default:
			cout << "�����ָ������" << endl;
			cout << "�����������������" << endl;
			getchar(); getchar();
			break;
		}
	}
}
//΢���Ŀչ���
void MyWeBo::ChatToFriend(message & Data, clientSocket & mySocket)
{
	cout << "΢���ݲ�֧�ּ�ʱͨѶ����Ŷ~~" << endl;
}
