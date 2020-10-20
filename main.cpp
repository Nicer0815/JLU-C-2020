#pragma once
#include<iostream>
#include"clientHead.h"
int main()
{
	/*注意: 校园网不同地方的IP会变化*/
	clientSocket mySocket("49.140.97.77", 8888);
	if (!mySocket.connectToServer())
	{
		return 0;
	}
	mgc Data;
	mgc ret;

	//账号密码登陆，存储在 id_confrim 表中
	while (initlogin(Data, ret, mySocket) != 1) {}
	strcpy_s(Data.table, "qq");
	MyQQ myqq(Data, ret, mySocket);
	strcpy_s(Data.table, "wc");
	MyWeChat mywechat(Data, ret, mySocket);
	strcpy_s(Data.table, "wb");
	MyWeBo mywebo(Data, ret, mySocket);

	//向服务端发送数据
	while (true)
	{

		menu();
		//主菜单操作
		cout << "请输入操作：";
		cin >> Data.action;
		mySocket.sendData(Data);
		int flag = 0;
		switch (Data.action)
		{
		case 1:
		{
			//登陆QQ 获取其他平台的好友推荐
			strcpy_s(Data.table, "qq");
			myqq.getRecommendFriend(mywechat.getFriendList(), "WeChat", Data, mySocket);
			myqq.getRecommendFriend(mywebo.getFriendList(), "WeBo", Data, mySocket);
			/* 通讯暂时结束 */
			Data.action = 1;	
			mySocket.sendData(Data);
			strcpy_s(Data.table, "qq");
			myqq.action(Data, mySocket);
		} break;
		case 2:
		{
			//登陆微信 获取其他平台的好友推荐
			strcpy_s(Data.table, "wc");
			mywechat.getRecommendFriend(myqq.getFriendList(), "QQ", Data, mySocket);
			mywechat.getRecommendFriend(mywebo.getFriendList(), "WeBo", Data, mySocket);
			/* 通讯暂时结束 */
			Data.action = 1;
			mySocket.sendData(Data);
			strcpy_s(Data.table, "wc");
			mywechat.action(Data, mySocket);
		}break;
		case 3:
		{
			//登陆微博 获取其他平台的好友推荐
			strcpy_s(Data.table, "wb");
			mywebo.getRecommendFriend(myqq.getFriendList(), "QQ", Data, mySocket);
			mywebo.getRecommendFriend(mywechat.getFriendList(), "WeChat", Data, mySocket);
			/* 通讯暂时结束 */
			Data.action = 1;
			mySocket.sendData(Data);
			strcpy_s(Data.table, "wb");
			mywebo.action(Data, mySocket);
		}break;
		case 9:flag = 1; break;

		default:cout << "输入错误 请重新输入：" << endl;
		}
		if (flag == 1)
		{
			Data.action = 9;
			//通知服务器 本客户端下线
			mySocket.sendData(Data);
			break;
		}
	}
	WSACleanup();
	return 0;
}