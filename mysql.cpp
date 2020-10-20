#include "serverHead.h"
extern unsigned __int64 client[10][2];
extern char onlineName[10][20];
extern char onlineQQID[10][20];
extern int clientNum;
User_mysql::User_mysql()
{
	strcpy_s(user, "root");
	strcpy_s(pswd, "201501516lyn...");
	strcpy_s(host, "localhost");
	port = 3307;
	mysql_init(&myCont);
	if (mysql_real_connect(&myCont, host, user, pswd, "mydesign", port, NULL, 0))
	{
		mysql_query(&myCont, "SET NAMES GBK");
		if (mysql_query(&myCont, "use mydesign"))
		{
			cout << "fail to use mydesign in confirm stage " << endl;
			system("pause");
			exit(-1);
		}
		cout << "mydesign �ɹ���" << endl;

	}
	else
	{
		cout << "fail to connect mydesign in confirm stage" << endl;
		system("pause");
		exit(-1);
	}

}
//�����ƽ̨�˺��������ȷ�ԣ�����ǰ����ƽ̨�˺Ŵ洢��Data�Ա��������ƽ̨����Ĺ���
int User_mysql::login(message & Data)
{
	char order[1024];
	sprintf_s(order, "SELECT * FROM id_confirm WHERE �˺�='%s'", Data.ID);
	res = mysql_query(&myCont, order);
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			//�����ѯ��Ϊ��
			if (sql_row = mysql_fetch_row(result))
			{
				//������ȷ
				if (!strcmp(sql_row[1], Data.paswad))
				{
					//��ǰ�������˺Ŵ洢��Data�� ���� ΢Xƽ̨�Ķ���Ĺ���
					strcpy_s(Data.QQID, sql_row[3]);
					strcpy_s(Data.WCID, sql_row[4]);
					strcpy_s(Data.WBID, sql_row[5]);
					//��QQ������ʵ�ֵļ�ʱ���칦��
					strcpy_s(onlineQQID[clientNum - 1], Data.QQID);					
					return 1;
				}
				else
				{
					return 3;
				}
			}
			//�˺Ų�����
			else
			{
				return 2;
			}
		}

	}
	else
	{
		cout << "��ѯʧ��" << endl;
		system("pause");
		exit(-1);
	}
	//�˺Ų�����
	return 2;

}
//���ؿͻ��˻��� user ����������Ϣ
int User_mysql::returnInfo(message & Data)
{
	char order[1024];
	sprintf_s(order, "SELECT * FROM id_confirm WHERE �˺�='%s'", Data.ID);
	res = mysql_query(&myCont, order);
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			//�����ѯ��Ϊ��
			if (sql_row = mysql_fetch_row(result))
			{
				//������������Ϣ��
				strcpy_s(Data.ID, sql_row[0]);
				cout << "user return over" << endl;
				return 1;
			}
			//�˺Ų�����
			else
			{
				cout << "return User_Info error cause ID do not exist" << endl;
				return 2;
			}
		}
		else
		{
			cout << "return User_Info error cause ID do not exist" << endl;
			return 2;
		}

	}
	else
	{
		cout << "return info sql �������ʧ��" << endl;
		return 3;
	}

}
//QQ�������ݿ�����캯��
Friend_mysql::Friend_mysql()
{
	strcpy_s(user, "root");
	strcpy_s(pswd, "201501516lyn...");
	strcpy_s(host, "localhost");
	port = 3307;
	mysql_init(&myCont);
	if (mysql_real_connect(&myCont, host, user, pswd, "mydesign", port, NULL, 0))
	{
		mysql_query(&myCont, "SET NAMES GBK");
		if (mysql_query(&myCont, "use mydesign"))
		{
			cout << "fail to use mydesign in confirm stage " << endl;
			system("pause");
			exit(-1);
		}
		cout << "mydesign �ɹ���" << endl;

	}
	else
	{
		cout << "fail to connect mydesign in confirm stage" << endl;
		system("pause");
		exit(-1);
	}

}
//QQȺ���ݿ�����캯��
Group_mysql::Group_mysql()
{
	strcpy_s(user, "root");
	strcpy_s(pswd, "201501516lyn...");
	strcpy_s(host, "localhost");
	port = 3307;
	mysql_init(&myCont);
	if (mysql_real_connect(&myCont, host, user, pswd, "mydesign", port, NULL, 0))
	{
		mysql_query(&myCont, "SET NAMES GBK");
		if (mysql_query(&myCont, "use mydesign"))
		{
			cout << "fail to use mydesign in confirm stage " << endl;
			system("pause");
			exit(-1);
		}
		cout << "mydesign �ɹ���" << endl;

	}
	else
	{
		cout << "fail to connect mydesign in confirm stage" << endl;
		system("pause");
		exit(-1);
	}
}
//ȥ���ݿ��з��� -����- ��ϸ��Ϣ�����ͻ���myqq������������Ϣ
int Friend_mysql::returnDetails(message & Data)
{
	char order[1024];
	sprintf_s(order, "SELECT * FROM %s_details WHERE %s�˺�=%s"
		,Data.table, Data.table, Data.XID);
	
	cout << order << endl;
	
	Data.platform = 0;
	res = mysql_query(&myCont, order);
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			//��ѯ�����Ϊ��
			if (sql_row = mysql_fetch_row(result))
			{
				//ɾ��Data.platform
				Data.platform = 1;
				strcpy_s(Data.Name, sql_row[1]);		//΢X����
				Data.TAge = atoi(sql_row[2]);			//T��
				strcpy_s(Data.BirthDay, sql_row[3]);	//��������
				strcpy_s(Data.Place, sql_row[4]);		//���ڵ�
				if (strcmp(sql_row[5], "0"))
				{
					if (!strcmp("qq", Data.table))
					{
						Data.platform = 10;
						strcpy_s(Data.WCID, sql_row[5]);
					}
					else if (!strcmp("wc", Data.table))
					{
						Data.platform = 10;
						strcpy_s(Data.QQID, sql_row[5]);
					}
					else if (!strcmp("wb", Data.table))
					{
						Data.platform = 10;
						strcpy_s(Data.WBID, sql_row[5]);
					}
					else //��QQ���캯��
					{
						Data.platform = -10;
						strcpy_s(Data.QQID, sql_row[5]);		
					}
				}
				//��¼��ǰ���ߵĿͻ��˵�QQ�˺� �� ����
				if (!strcmp(Data.table, "qq"))
				{
					client[clientNum - 1][1] = atoi(Data.QQID);
					strcpy_s(onlineName[clientNum - 1], Data.Name);
					cout << clientNum << "λ��  "
						<< client[clientNum - 1][1] << "  QQ�û�: "
						<< onlineName[clientNum - 1] << "����" << endl;
				}
				return 1;
			}
			else
			{
				cout << "sql "<<Data.table<<"_details ��ѯ���Ϊ�գ����û����ܲ�����" << endl;
				return 2;
			}
		}
		else
		{
			cout << "sql " << Data.table << "_details ��ѯ���Ϊ�գ����û����ܲ�����" << endl;
			return 2;
		}
	}
	else
	{
		cout << "sql  " << Data.table << "_datails �������ʧ��" << endl;
		return 3;
	}

}
//ȥ���ݿ��з��غ����б����ͻ���myqq������������Ϣ
int Friend_mysql::returnInfo(message & Data)
{
	char order[1024];
	sprintf_s(order
		, "SELECT * FROM %s_friend WHERE �����˺�='%s'"
		,Data.table, Data.XID);
	res = mysql_query(&myCont, order);
	Data.FriendNum = 0;
	
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			//�����ѯ��Ϊ��
			while (sql_row = mysql_fetch_row(result))
			{
				strcpy_s(Data.FriendList[Data.FriendNum], sql_row[1]);
				strcpy_s(Data.FriendAccount[Data.FriendNum], sql_row[4]);
				strcpy_s(Data.FriendNickName[Data.FriendNum], sql_row[3]);
				Data.FriendNum++;
				cout << "�������" << Data.FriendNum <<":"<< sql_row[1] <<"---"<< sql_row[3];
				cout << endl;
			}
			//�˺Ų�����
			if(!Data.FriendNum)
			{
				cout << "return FriendList error cause ID do not have" << endl;
				return 2;
			}
			else
			{
				return 1;
			}
		}
		{
			cout << "return FriendList error cause ID do not have(result)" << endl;
			return 2;
		}

	}
	else
	{
		cout << "return FriendList sql �������ʧ��" << endl;
		return 3;
	}

}
//ȥ���ݿ��з���Ⱥ���б����ͻ���myqq������������Ϣ
int Group_mysql::returnInfo(message & Data)
{
	char order[1024];
	sprintf_s(order
		, "SELECT * FROM %s_group WHERE Ⱥ��Ա�˺�='%s'"
		,Data.table, Data.XID);
	res = mysql_query(&myCont, order);
	Data.GroupNum = 0;
	int cnt = 0;
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			//�����ѯ��Ϊ��
			while (sql_row = mysql_fetch_row(result))
			{
				cnt++;
				strcpy_s(Data.GroupList[Data.GroupNum], sql_row[0]);
				strcpy_s(Data.GroupName[Data.GroupNum], sql_row[2]);
				Data.GroupStatus[Data.GroupNum] = atoi(sql_row[5]);
				Data.GroupNum++;

			}
			//�˺Ų�����
			if (!cnt)
			{
				cout << "return GroupList error cause ID do not have" << endl;
				return 2;
			}
			else
			{
				return 1;
			}
		}
		{
			cout << "return GroupList error cause ID do not have(result)" << endl;
			return 2;
		}

	}
	else
	{
		cout << "return GroupList sql �������ʧ��" << endl;
		return 3;
	}
}
//ȥ���ݿ������ú��ѱ�ע���޸ĺ�����Ϣ��
int Friend_mysql::setNicknameToFriend(message & Data)
{
	char order[1024];
	sprintf_s(order, "UPDATE %s_friend SET ���ѱ�ע='%s' WHERE �����˺�=%s AND �����˺�= %s"
		,Data.table, Data.NickName, Data.ID, Data.XID);
	res = mysql_query(&myCont, order);
	//sql ���ִ�гɹ�
	//cout <<"�鿴res����ֵing��"<< res << endl;  //�ɹ��˶���res==1;
	if (!res)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
//ȥ���ݿ��аѺ��ѵ���ϸ��Ϣ����
int Friend_mysql::findFriend(message & Data)
{
	char order[1024];
	sprintf_s(order, "SELECT * FROM %s_details WHERE %s�˺�=%s"
		,Data.table,Data.table, Data.XID);
	cout << order << endl;
	res = mysql_query(&myCont, order);
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			if (sql_row = mysql_fetch_row(result))
			{
				strcpy_s(Data.Name, sql_row[1]);		//΢X����
				Data.TAge = atoi(sql_row[2]);			//T��
				strcpy_s(Data.BirthDay, sql_row[3]);	//��������
				strcpy_s(Data.Place, sql_row[4]);		//���ڵ�
				return 1;
				
			}
			else
			{
				cout << "sql "<< Data.table <<"_details ��ѯ���Ϊ�գ����û����ܲ�����" << endl;
				return 2;
			}
		}
		else
		{
			cout << "sql "<<Data.table <<"_details ��ѯ���Ϊ�գ����û����ܲ�����(result)" << endl;
			return 2;
		}
	}
	else
	{
		cout << "sql "<< Data.table <<"_datails �������ʧ��" << endl;
		return 3;
	}

}
//ȥ���ݿ���Ⱥ
int Group_mysql::dropGroup(message & Data)
{
	char order[1024];
	sprintf_s(order
		, "DELETE FROM %s_group WHERE Ⱥ��=%s AND Ⱥ��Ա�˺�=%s"
		,Data.table, Data.GID, Data.XID);
	res = mysql_query(&myCont, order);
	if (!res)
	{
		cout <<Data.XID <<"��Ⱥ"<<Data.GID<<"�ɹ�" << endl;
		return 1;
	}
	else
	{
		cout << "��Ⱥʧ�� sql �������ʧ��" << endl;
		return 0;
	}
}
//ȥ���ݿ�ɾ������
int Friend_mysql::deleteFriend(message & Data)
{
	char order[1024];
	sprintf_s(order
		, "DELETE FROM %s_friend WHERE �����˺�=%s AND �����˺�=%s"
		,Data.table, Data.ID, Data.XID);
	res = mysql_query(&myCont, order);
	sprintf_s(order
		, "DELETE FROM %s_friend WHERE �����˺�=%s AND �����˺�=%s"
		,Data.table, Data.XID, Data.ID);
	res = mysql_query(&myCont, order);
	if (!res)
	{
		cout << Data.ID << "ɾ������" << Data.XID << "�ɹ�" << endl;
		return 1;
	}
	else
	{
		cout << "ɾ������ʧ�� sql �������ʧ��" << endl;
		return 0;
	}
}
//ȥ���ݿ����Ⱥ
int Group_mysql::addGroup(message & Data)
{
	int groupKind = -1;
	char order[1024];
	sprintf_s(order
		, "SELECT * FROM %s_group WHERE Ⱥ��=%s"
		,Data.table, Data.GID);

	cout << "׼����ѯȺ�ľ�����Ϣ������" << endl;
	cout << order << endl;
	
	
	res = mysql_query(&myCont, order);
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			if (sql_row = mysql_fetch_row(result))
			{
				strcpy_s(Data.GroupName[0], sql_row[2]);
				groupKind = atoi(sql_row[1]);
				cout << "Ⱥ����" << Data.GroupName[0] << "Ⱥ����" << groupKind << endl;
			}
		}
	}
	//�൱��flagֵ,�ж�ǰ���ѯ�����Ƿ��Ѿ�˳��ִ�в�ѯ
	if (groupKind == -1)
	{
		cout << "��Ⱥʧ�� sql ���û�в鵽��Ⱥ" << endl;
		return 0;
	}
	sprintf_s(order
		, "INSERT INTO %s_group VALUES (%s,%d,'%s',%s,'%s',0)"
		,Data.table, Data.GID, groupKind,Data.GroupName[0], Data.XID,Data.Name);
	res = mysql_query(&myCont, order);
	if (!res)
	{
		cout << Data.XID << "��Ⱥ" << Data.GID << "�ɹ�" << endl;
		return 1;
	}
	else
	{
		cout << "��Ⱥʧ�� sql �������ʧ��" << endl;
		return 0;
	}
}
//ȥ���ݿ�Ӻ���
int Friend_mysql::addFriend(message & Data)
{
	int flag = 0;
	char TID[12];
	char friendName[20];
	char order[1024];
	sprintf_s(order
		, "SELECT * FROM id_confirm WHERE %s�˺�=%s"
		,Data.table, Data.XID);
	cout << order << endl;
	res = mysql_query(&myCont, order);
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			if (sql_row = mysql_fetch_row(result))
			{
				flag = 1;
				strcpy_s(TID, sql_row[0]);
				strcpy_s(friendName, sql_row[6]);
				cout << "�������֣�" << friendName << "ƽ̨���˺ţ�" << TID << endl;
			}
		}
	}
	if (!flag)
	{
		cout << "û���ҵ�" << Data.XID << endl;
		return 0;
	}
	sprintf_s(order
		, "INSERT INTO %s_friend VALUES (%s,%s,'%s','%s',%s)"
		,Data.table, Data.ID, Data.XID, friendName, Data.NickName, TID);
	res = mysql_query(&myCont, order);
	sprintf_s(order
		, "INSERT INTO %s_friend VALUES (%s,%s,'%s','%s',%s)"
		,Data.table, Data.XID, Data.ID, Data.Name, Data.Name, Data.GID);
	res = mysql_query(&myCont, order);
	if (!res)
	{
		cout << Data.ID << "��Ӻ���" << Data.XID << "�ɹ�" << endl;
		return 1;
	}
	else
	{
		cout << "��Ӻ���ʧ�� sql �������ʧ��" << endl;
		return 0;
	}	
}
//����ƽ̨���˺ţ�ȥ���ݿ��в�ѯ ΢Xƽ̨�˺��Ƿ���ڣ����ڼ����Ƽ���
int Friend_mysql::isExist(message & Data)
{
	char order[1024];
	sprintf_s(order, "SELECT * FROM id_confirm WHERE �˺�=%s AND %s�˺�<>0 "
			, Data.ID, Data.table);
	cout << order << endl;
	res = mysql_query(&myCont, order);
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			if (sql_row = mysql_fetch_row(result))
			{
				int line = -1;
				if (!strcmp("qq", Data.table))line = 3;
				else if (!strcmp("wc", Data.table))line = 4;
				else line = 5;
				strcpy_s(Data.XID, sql_row[line]);
				cout << "�ҵ���Ӧ��" << Data.table << "�˺�:" << Data.XID << endl;
				return 1;
			}
		}
	}
	return 0;
}
//ȥ���ݿ�����
int Group_mysql::kickGroupMember(message & Data)
{
	char order[1024];
	int myStatus= -1;
	int itStatus= -1;
	sprintf_s(order
		, "SELECT * FROM %s_group WHERE Ⱥ��=%s AND Ⱥ��Ա�˺�=%s"
		,Data.table, Data.GID, Data.ID);
	cout << order << endl;
	res = mysql_query(&myCont, order);
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			if (sql_row = mysql_fetch_row(result))
			{
				myStatus = atoi(sql_row[5]);
				cout << "myStatus:" << myStatus << endl;
			}
		}
	}
	if (myStatus == -1) 
	{
		cout << "û�鵽myStatus" << endl;
		return -1;
	}

	sprintf_s(order
		, "SELECT * FROM %s_group WHERE Ⱥ��=%s AND Ⱥ��Ա�˺�=%s"
		,Data.table, Data.GID, Data.XID);
	cout << order << endl;
	res = mysql_query(&myCont, order);
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			if (sql_row = mysql_fetch_row(result))
			{
				itStatus = atoi(sql_row[5]);
				cout << "itStatus:" << itStatus << endl;
			}
		}
	}
	if (itStatus == -1)
	{
		cout << "û�鵽itStatus" << endl;
		return -1;
	}
	if (myStatus <= itStatus)
	{
		cout << "��λ̫���߲���" << endl;
		return 0;
	}
	sprintf_s(order
		, "DELETE FROM %s_group WHERE Ⱥ��=%s AND Ⱥ��Ա�˺�=%s"
		,Data.table, Data.GID, Data.XID);
	cout << order << endl;
	res = mysql_query(&myCont, order);
	if (!res)
	{
		cout << Data.ID << "��" << Data.XID << "��" << Data.GID << "Ⱥ���߳�" << endl;
		return 1;
	}
	else
	{
		cout << "����sql���ִ��ʧ��" << endl;
		return -2;
	}

}
//ȥ���ݿ��а�ĳ��Ⱥ�ĳ�Ա��Ϣ����
int Group_mysql::listMember(message & Data)
{
	char order[1024];
	sprintf_s(order
		, "SELECT * FROM %s_group WHERE Ⱥ��=%s"
		,Data.table, Data.GID);
	res = mysql_query(&myCont, order);
	//��Ա����
	Data.GroupNum = 0;
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			while (sql_row = mysql_fetch_row(result))
			{
				strcpy_s(Data.GroupList[Data.GroupNum], sql_row[3]);	//�˺�
				strcpy_s(Data.GroupName[Data.GroupNum], sql_row[4]);	//����
				Data.GroupStatus[Data.GroupNum] = atoi(sql_row[5]);		//���
				Data.GroupNum++;
			}
		}
	}
	if (0 == Data.GroupNum)
	{
		cout << Data.GID << "Ⱥ��û��" << endl;
		return 0;
	}
	else
	{
		cout << Data.GID << "Ⱥ�鵽" << Data.GroupNum << "����Ա" <<endl;
		return 1;
	}
}
//ȥ���ݿ�������QQȺ
int Group_mysql::levelUpGroup(message & Data)
{
	char order[1024];
	sprintf_s(order
		, "SELECT * FROM %s_group WHERE Ⱥ��=%s AND Ⱥ��Ա�˺�=%s"
		,Data.table, Data.GID, Data.ID);
	int flag = 0;
	res = mysql_query(&myCont, order);
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			if (sql_row = mysql_fetch_row(result))
			{
				if (1 == atoi(sql_row[1]))		//Ⱥ�Ѿ�����߼�
				{
					Data.action = 0;
					return 0;
				}
				if (3 != atoi(sql_row[5]))		//����Ⱥ��
				{
					Data.action = -1;
					return -1;
				}
				flag = 1;

			}
		}
	}
	if (flag)
	{
		sprintf_s(order
			, "UPDATE %s_group SET Ⱥ����=1 WHERE Ⱥ��=%s"
			,Data.table, Data.GID);
		res = mysql_query(&myCont, order);
		cout << order << endl;
		cout <<Data.GID<< "�ɹ�����Ϊ����"<< Data.table <<"Ⱥ" << endl;
		Data.action = 1;
		return 1;
	}
	else
	{
		cout << "û���ҵ�����Ҫ���Ⱥ��" << endl;;
		return -1;
	}
}
//ȥ���ݿⴴ��Ⱥ
int Group_mysql::creatGroup(message & Data) 
{
	
	char order[1024];
	sprintf_s(order, "select Ⱥ�� FROM %s_group order by Ⱥ�� desc"
		, Data.table);
	res = mysql_query(&myCont, order);
	int maxGid = 0;
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			if (sql_row = mysql_fetch_row(result))
			{
				maxGid = atoi(sql_row[0]);
			}
		}
	}
	if (!maxGid)
	{
		cout << "sql �Զ�����Ⱥ�˺�ʧ��" << endl;
		return 0;
	}
	_itoa_s(maxGid + 1, Data.GroupList[0], 10);
	sprintf_s(order
		, "INSERT INTO %s_group VALUES(%d,1,'%s',%s,'%s',3)"
		,Data.table, maxGid + 1, Data.GroupName[1], Data.GroupList[1],Data.Name);
	res = mysql_query(&myCont, order);
	if (!res)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
//ȥ���ݿⴴ����Ⱥ(����ԭȺ��Ա��˭����˭����Ⱥ��Ⱥ��)
int Group_mysql::creatTempGroup(message & Data)
{
	char order[1024];
	sprintf_s(order, "select Ⱥ�� FROM %s_group order by Ⱥ�� desc"
		, Data.table);
	res = mysql_query(&myCont, order);
	int maxGid = 0;
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			if (sql_row = mysql_fetch_row(result))
			{
				maxGid = atoi(sql_row[0]);
			}
		}
	}
	if (!maxGid)
	{
		cout << "sql �Զ�������Ⱥ�˺�ʧ��" << endl;
		return 0;
	}
	char origin_Gid[10];
	strcpy_s(origin_Gid, Data.GID);
	char GName[21];
	strcpy_s(GName, Data.GroupName[1]);
	_itoa_s(maxGid + 1, Data.GID, 10);
	//��ԭȺ������
	sprintf_s(order
		, "SELECT * FROM %s_group WHERE Ⱥ��=%s"
		, Data.table, origin_Gid);
	res = mysql_query(&myCont, order);
	//��Ա����
	Data.GroupNum = 0;
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			while (sql_row = mysql_fetch_row(result))
			{
				strcpy_s(Data.GroupList[Data.GroupNum], sql_row[3]);	//�˺�
				strcpy_s(Data.GroupName[Data.GroupNum], sql_row[4]);	//����
				Data.GroupNum++;
			}
		}
	}
	for (int i = 0; i < Data.GroupNum; i++)
	{
		sprintf_s(order
			, "INSERT INTO %s_group VALUES(%d,2,'%s',%s,'%s',1)"
			, Data.table, maxGid + 1, GName, Data.GroupList[i], Data.GroupName[i]);
		cout << order << endl;
		res = mysql_query(&myCont, order);
		if (res)
		{
			cout << "���Ƴ�Ա����" << endl;
			return -1;
		}
	}
	sprintf_s(order
		, "UPDATE %s_group SET Ⱥ��Ա��� = 3 WHERE Ⱥ��=%d AND Ⱥ��Ա�˺�=%s"
		, Data.table, maxGid + 1, Data.XID);
	cout << order << endl;
	res = mysql_query(&myCont, order);
	if (!res)
	{
		cout << "��ȺȺ�����óɹ�" << endl;
		return 1;
	}
	else
	{
		cout << "��ȺȺ������ʧ��" << endl;
		return -1;
	}
}