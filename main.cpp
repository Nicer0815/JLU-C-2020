#pragma once
#include<iostream>
#include"clientHead.h"
int main()
{
	/*ע��: У԰����ͬ�ط���IP��仯*/
	clientSocket mySocket("49.140.97.77", 8888);
	if (!mySocket.connectToServer())
	{
		return 0;
	}
	mgc Data;
	mgc ret;

	//�˺������½���洢�� id_confrim ����
	while (initlogin(Data, ret, mySocket) != 1) {}
	strcpy_s(Data.table, "qq");
	MyQQ myqq(Data, ret, mySocket);
	strcpy_s(Data.table, "wc");
	MyWeChat mywechat(Data, ret, mySocket);
	strcpy_s(Data.table, "wb");
	MyWeBo mywebo(Data, ret, mySocket);

	//�����˷�������
	while (true)
	{

		menu();
		//���˵�����
		cout << "�����������";
		cin >> Data.action;
		mySocket.sendData(Data);
		int flag = 0;
		switch (Data.action)
		{
		case 1:
		{
			//��½QQ ��ȡ����ƽ̨�ĺ����Ƽ�
			strcpy_s(Data.table, "qq");
			myqq.getRecommendFriend(mywechat.getFriendList(), "WeChat", Data, mySocket);
			myqq.getRecommendFriend(mywebo.getFriendList(), "WeBo", Data, mySocket);
			/* ͨѶ��ʱ���� */
			Data.action = 1;	
			mySocket.sendData(Data);
			strcpy_s(Data.table, "qq");
			myqq.action(Data, mySocket);
		} break;
		case 2:
		{
			//��½΢�� ��ȡ����ƽ̨�ĺ����Ƽ�
			strcpy_s(Data.table, "wc");
			mywechat.getRecommendFriend(myqq.getFriendList(), "QQ", Data, mySocket);
			mywechat.getRecommendFriend(mywebo.getFriendList(), "WeBo", Data, mySocket);
			/* ͨѶ��ʱ���� */
			Data.action = 1;
			mySocket.sendData(Data);
			strcpy_s(Data.table, "wc");
			mywechat.action(Data, mySocket);
		}break;
		case 3:
		{
			//��½΢�� ��ȡ����ƽ̨�ĺ����Ƽ�
			strcpy_s(Data.table, "wb");
			mywebo.getRecommendFriend(myqq.getFriendList(), "QQ", Data, mySocket);
			mywebo.getRecommendFriend(mywechat.getFriendList(), "WeChat", Data, mySocket);
			/* ͨѶ��ʱ���� */
			Data.action = 1;
			mySocket.sendData(Data);
			strcpy_s(Data.table, "wb");
			mywebo.action(Data, mySocket);
		}break;
		case 9:flag = 1; break;

		default:cout << "������� ���������룺" << endl;
		}
		if (flag == 1)
		{
			Data.action = 9;
			//֪ͨ������ ���ͻ�������
			mySocket.sendData(Data);
			break;
		}
	}
	WSACleanup();
	return 0;
}