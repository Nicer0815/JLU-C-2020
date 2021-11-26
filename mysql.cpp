#include "serverHead.h"
extern unsigned __int64 client[10][2];
extern char onlineName[10][20];
extern char onlineQQID[10][20];
extern int clientNum;
User_mysql::User_mysql()
{
	strcpy_s(user, "root");
	strcpy_s(pswd, "2");
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
		cout << "mydesign 成功打开" << endl;

	}
	else
	{
		cout << "fail to connect mydesign in confirm stage" << endl;
		system("pause");
		exit(-1);
	}

}
//检测总平台账号密码的正确性，并提前将各平台账号存储至Data以便接下来各平台对象的构造
int User_mysql::login(message & Data)
{
	char order[1024];
	sprintf_s(order, "SELECT * FROM id_confirm WHERE 账号='%s'", Data.ID);
	res = mysql_query(&myCont, order);
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			//如果查询不为空
			if (sql_row = mysql_fetch_row(result))
			{
				//密码正确
				if (!strcmp(sql_row[1], Data.paswad))
				{
					//提前把三个账号存储到Data中 方便 微X平台的对象的构造
					strcpy_s(Data.QQID, sql_row[3]);
					strcpy_s(Data.WCID, sql_row[4]);
					strcpy_s(Data.WBID, sql_row[5]);
					//对QQ特异性实现的即时聊天功能
					strcpy_s(onlineQQID[clientNum - 1], Data.QQID);					
					return 1;
				}
				else
				{
					return 3;
				}
			}
			//账号不存在
			else
			{
				return 2;
			}
		}

	}
	else
	{
		cout << "查询失败" << endl;
		system("pause");
		exit(-1);
	}
	//账号不存在
	return 2;

}
//返回客户端基类 user 构造所需信息
int User_mysql::returnInfo(message & Data)
{
	char order[1024];
	sprintf_s(order, "SELECT * FROM id_confirm WHERE 账号='%s'", Data.ID);
	res = mysql_query(&myCont, order);
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			//如果查询不为空
			if (sql_row = mysql_fetch_row(result))
			{
				//后期再增加信息？
				strcpy_s(Data.ID, sql_row[0]);
				cout << "user return over" << endl;
				return 1;
			}
			//账号不存在
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
		cout << "return info sql 请求语句失败" << endl;
		return 3;
	}

}
//QQ好友数据库对象构造函数
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
		cout << "mydesign 成功打开" << endl;

	}
	else
	{
		cout << "fail to connect mydesign in confirm stage" << endl;
		system("pause");
		exit(-1);
	}

}
//QQ群数据库对象构造函数
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
		cout << "mydesign 成功打开" << endl;

	}
	else
	{
		cout << "fail to connect mydesign in confirm stage" << endl;
		system("pause");
		exit(-1);
	}
}
//去数据库中返回 -本人- 详细信息，即客户端myqq对象构造所需信息
int Friend_mysql::returnDetails(message & Data)
{
	char order[1024];
	sprintf_s(order, "SELECT * FROM %s_details WHERE %s账号=%s"
		,Data.table, Data.table, Data.XID);
	
	cout << order << endl;
	
	Data.platform = 0;
	res = mysql_query(&myCont, order);
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			//查询结果不为空
			if (sql_row = mysql_fetch_row(result))
			{
				//删了Data.platform
				Data.platform = 1;
				strcpy_s(Data.Name, sql_row[1]);		//微X名字
				Data.TAge = atoi(sql_row[2]);			//T龄
				strcpy_s(Data.BirthDay, sql_row[3]);	//出生日期
				strcpy_s(Data.Place, sql_row[4]);		//所在地
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
					else //非QQ构造函数
					{
						Data.platform = -10;
						strcpy_s(Data.QQID, sql_row[5]);		
					}
				}
				//记录当前上线的客户端的QQ账号 和 名字
				if (!strcmp(Data.table, "qq"))
				{
					client[clientNum - 1][1] = atoi(Data.QQID);
					strcpy_s(onlineName[clientNum - 1], Data.Name);
					cout << clientNum << "位：  "
						<< client[clientNum - 1][1] << "  QQ用户: "
						<< onlineName[clientNum - 1] << "上线" << endl;
				}
				return 1;
			}
			else
			{
				cout << "sql "<<Data.table<<"_details 查询结果为空，该用户可能不存在" << endl;
				return 2;
			}
		}
		else
		{
			cout << "sql " << Data.table << "_details 查询结果为空，该用户可能不存在" << endl;
			return 2;
		}
	}
	else
	{
		cout << "sql  " << Data.table << "_datails 请求语句失败" << endl;
		return 3;
	}

}
//去数据库中返回好友列表，即客户端myqq对象构造所需信息
int Friend_mysql::returnInfo(message & Data)
{
	char order[1024];
	sprintf_s(order
		, "SELECT * FROM %s_friend WHERE 所属账号='%s'"
		,Data.table, Data.XID);
	res = mysql_query(&myCont, order);
	Data.FriendNum = 0;
	
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			//如果查询不为空
			while (sql_row = mysql_fetch_row(result))
			{
				strcpy_s(Data.FriendList[Data.FriendNum], sql_row[1]);
				strcpy_s(Data.FriendAccount[Data.FriendNum], sql_row[4]);
				strcpy_s(Data.FriendNickName[Data.FriendNum], sql_row[3]);
				Data.FriendNum++;
				cout << "好友序号" << Data.FriendNum <<":"<< sql_row[1] <<"---"<< sql_row[3];
				cout << endl;
			}
			//账号不存在
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
		cout << "return FriendList sql 请求语句失败" << endl;
		return 3;
	}

}
//去数据库中返回群组列表，即客户端myqq对象构造所需信息
int Group_mysql::returnInfo(message & Data)
{
	char order[1024];
	sprintf_s(order
		, "SELECT * FROM %s_group WHERE 群成员账号='%s'"
		,Data.table, Data.XID);
	res = mysql_query(&myCont, order);
	Data.GroupNum = 0;
	int cnt = 0;
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			//如果查询不为空
			while (sql_row = mysql_fetch_row(result))
			{
				cnt++;
				strcpy_s(Data.GroupList[Data.GroupNum], sql_row[0]);
				strcpy_s(Data.GroupName[Data.GroupNum], sql_row[2]);
				Data.GroupStatus[Data.GroupNum] = atoi(sql_row[5]);
				Data.GroupNum++;

			}
			//账号不存在
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
		cout << "return GroupList sql 请求语句失败" << endl;
		return 3;
	}
}
//去数据库中设置好友备注（修改好友信息）
int Friend_mysql::setNicknameToFriend(message & Data)
{
	char order[1024];
	sprintf_s(order, "UPDATE %s_friend SET 好友备注='%s' WHERE 所属账号=%s AND 好友账号= %s"
		,Data.table, Data.NickName, Data.ID, Data.XID);
	res = mysql_query(&myCont, order);
	//sql 语句执行成功
	//cout <<"查看res返回值ing："<< res << endl;  //成功了而且res==1;
	if (!res)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
//去数据库中把好友的详细信息返回
int Friend_mysql::findFriend(message & Data)
{
	char order[1024];
	sprintf_s(order, "SELECT * FROM %s_details WHERE %s账号=%s"
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
				strcpy_s(Data.Name, sql_row[1]);		//微X名字
				Data.TAge = atoi(sql_row[2]);			//T龄
				strcpy_s(Data.BirthDay, sql_row[3]);	//出生日期
				strcpy_s(Data.Place, sql_row[4]);		//所在地
				return 1;
				
			}
			else
			{
				cout << "sql "<< Data.table <<"_details 查询结果为空，该用户可能不存在" << endl;
				return 2;
			}
		}
		else
		{
			cout << "sql "<<Data.table <<"_details 查询结果为空，该用户可能不存在(result)" << endl;
			return 2;
		}
	}
	else
	{
		cout << "sql "<< Data.table <<"_datails 请求语句失败" << endl;
		return 3;
	}

}
//去数据库退群
int Group_mysql::dropGroup(message & Data)
{
	char order[1024];
	sprintf_s(order
		, "DELETE FROM %s_group WHERE 群号=%s AND 群成员账号=%s"
		,Data.table, Data.GID, Data.XID);
	res = mysql_query(&myCont, order);
	if (!res)
	{
		cout <<Data.XID <<"退群"<<Data.GID<<"成功" << endl;
		return 1;
	}
	else
	{
		cout << "退群失败 sql 语句请求失败" << endl;
		return 0;
	}
}
//去数据库删除好友
int Friend_mysql::deleteFriend(message & Data)
{
	char order[1024];
	sprintf_s(order
		, "DELETE FROM %s_friend WHERE 所属账号=%s AND 好友账号=%s"
		,Data.table, Data.ID, Data.XID);
	res = mysql_query(&myCont, order);
	sprintf_s(order
		, "DELETE FROM %s_friend WHERE 所属账号=%s AND 好友账号=%s"
		,Data.table, Data.XID, Data.ID);
	res = mysql_query(&myCont, order);
	if (!res)
	{
		cout << Data.ID << "删除好友" << Data.XID << "成功" << endl;
		return 1;
	}
	else
	{
		cout << "删除好友失败 sql 语句请求失败" << endl;
		return 0;
	}
}
//去数据库添加群
int Group_mysql::addGroup(message & Data)
{
	int groupKind = -1;
	char order[1024];
	sprintf_s(order
		, "SELECT * FROM %s_group WHERE 群号=%s"
		,Data.table, Data.GID);

	cout << "准备查询群的具体信息。。。" << endl;
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
				cout << "群名称" << Data.GroupName[0] << "群类型" << groupKind << endl;
			}
		}
	}
	//相当于flag值,判断前面查询过程是否已经顺利执行查询
	if (groupKind == -1)
	{
		cout << "加群失败 sql 语句没有查到该群" << endl;
		return 0;
	}
	sprintf_s(order
		, "INSERT INTO %s_group VALUES (%s,%d,'%s',%s,'%s',0)"
		,Data.table, Data.GID, groupKind,Data.GroupName[0], Data.XID,Data.Name);
	res = mysql_query(&myCont, order);
	if (!res)
	{
		cout << Data.XID << "加群" << Data.GID << "成功" << endl;
		return 1;
	}
	else
	{
		cout << "加群失败 sql 语句请求失败" << endl;
		return 0;
	}
}
//去数据库加好友
int Friend_mysql::addFriend(message & Data)
{
	int flag = 0;
	char TID[12];
	char friendName[20];
	char order[1024];
	sprintf_s(order
		, "SELECT * FROM id_confirm WHERE %s账号=%s"
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
				cout << "好友名字：" << friendName << "平台总账号：" << TID << endl;
			}
		}
	}
	if (!flag)
	{
		cout << "没有找到" << Data.XID << endl;
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
		cout << Data.ID << "添加好友" << Data.XID << "成功" << endl;
		return 1;
	}
	else
	{
		cout << "添加好友失败 sql 语句请求失败" << endl;
		return 0;
	}	
}
//根据平台总账号，去数据库中查询 微X平台账号是否存在（存在即可推荐）
int Friend_mysql::isExist(message & Data)
{
	char order[1024];
	sprintf_s(order, "SELECT * FROM id_confirm WHERE 账号=%s AND %s账号<>0 "
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
				cout << "找到对应的" << Data.table << "账号:" << Data.XID << endl;
				return 1;
			}
		}
	}
	return 0;
}
//去数据库踢人
int Group_mysql::kickGroupMember(message & Data)
{
	char order[1024];
	int myStatus= -1;
	int itStatus= -1;
	sprintf_s(order
		, "SELECT * FROM %s_group WHERE 群号=%s AND 群成员账号=%s"
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
		cout << "没查到myStatus" << endl;
		return -1;
	}

	sprintf_s(order
		, "SELECT * FROM %s_group WHERE 群号=%s AND 群成员账号=%s"
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
		cout << "没查到itStatus" << endl;
		return -1;
	}
	if (myStatus <= itStatus)
	{
		cout << "地位太低踢不了" << endl;
		return 0;
	}
	sprintf_s(order
		, "DELETE FROM %s_group WHERE 群号=%s AND 群成员账号=%s"
		,Data.table, Data.GID, Data.XID);
	cout << order << endl;
	res = mysql_query(&myCont, order);
	if (!res)
	{
		cout << Data.ID << "将" << Data.XID << "从" << Data.GID << "群中踢出" << endl;
		return 1;
	}
	else
	{
		cout << "踢人sql语句执行失败" << endl;
		return -2;
	}

}
//去数据库中把某个群的成员信息返回
int Group_mysql::listMember(message & Data)
{
	char order[1024];
	sprintf_s(order
		, "SELECT * FROM %s_group WHERE 群号=%s"
		,Data.table, Data.GID);
	res = mysql_query(&myCont, order);
	//成员个数
	Data.GroupNum = 0;
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			while (sql_row = mysql_fetch_row(result))
			{
				strcpy_s(Data.GroupList[Data.GroupNum], sql_row[3]);	//账号
				strcpy_s(Data.GroupName[Data.GroupNum], sql_row[4]);	//名字
				Data.GroupStatus[Data.GroupNum] = atoi(sql_row[5]);		//身份
				Data.GroupNum++;
			}
		}
	}
	if (0 == Data.GroupNum)
	{
		cout << Data.GID << "群内没人" << endl;
		return 0;
	}
	else
	{
		cout << Data.GID << "群查到" << Data.GroupNum << "个成员" <<endl;
		return 1;
	}
}
//去数据库中升级QQ群
int Group_mysql::levelUpGroup(message & Data)
{
	char order[1024];
	sprintf_s(order
		, "SELECT * FROM %s_group WHERE 群号=%s AND 群成员账号=%s"
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
				if (1 == atoi(sql_row[1]))		//群已经是最高级
				{
					Data.action = 0;
					return 0;
				}
				if (3 != atoi(sql_row[5]))		//不是群主
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
			, "UPDATE %s_group SET 群类型=1 WHERE 群号=%s"
			,Data.table, Data.GID);
		res = mysql_query(&myCont, order);
		cout << order << endl;
		cout <<Data.GID<< "成功升级为常规"<< Data.table <<"群" << endl;
		Data.action = 1;
		return 1;
	}
	else
	{
		cout << "没有找到符合要求的群组" << endl;;
		return -1;
	}
}
//去数据库创建群
int Group_mysql::creatGroup(message & Data) 
{
	
	char order[1024];
	sprintf_s(order, "select 群号 FROM %s_group order by 群号 desc"
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
		cout << "sql 自动分配群账号失败" << endl;
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
//去数据库创建子群(复制原群成员，谁创建谁是子群的群主)
int Group_mysql::creatTempGroup(message & Data)
{
	char order[1024];
	sprintf_s(order, "select 群号 FROM %s_group order by 群号 desc"
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
		cout << "sql 自动分配子群账号失败" << endl;
		return 0;
	}
	char origin_Gid[10];
	strcpy_s(origin_Gid, Data.GID);
	char GName[21];
	strcpy_s(GName, Data.GroupName[1]);
	_itoa_s(maxGid + 1, Data.GID, 10);
	//从原群插入人
	sprintf_s(order
		, "SELECT * FROM %s_group WHERE 群号=%s"
		, Data.table, origin_Gid);
	res = mysql_query(&myCont, order);
	//成员个数
	Data.GroupNum = 0;
	if (!res)
	{
		result = mysql_store_result(&myCont);
		if (result)
		{
			while (sql_row = mysql_fetch_row(result))
			{
				strcpy_s(Data.GroupList[Data.GroupNum], sql_row[3]);	//账号
				strcpy_s(Data.GroupName[Data.GroupNum], sql_row[4]);	//名字
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
			cout << "复制成员出错" << endl;
			return -1;
		}
	}
	sprintf_s(order
		, "UPDATE %s_group SET 群成员身份 = 3 WHERE 群号=%d AND 群成员账号=%s"
		, Data.table, maxGid + 1, Data.XID);
	cout << order << endl;
	res = mysql_query(&myCont, order);
	if (!res)
	{
		cout << "子群群主设置成功" << endl;
		return 1;
	}
	else
	{
		cout << "子群群主设置失败" << endl;
		return -1;
	}
}
