#ifndef MY_REDIS_H
#include <WINSOCK2.H>   
#include <iostream>
#include <string>
#include <map>
using namespace std;
#define MY_REDIS_H
//���ݿ�ĸ���
#define dbNum 15

//������redis����Ҫʹ�õ������ݽṹ
//ʹ��STL�е�map��Ϊdict�����ڿ��ܻ��Լ�������
//���ݿ�ʹ��һ��dict��ʾ
typedef map<string,string> myDB;
class Server;
class Client;
//������ĺ���ָ�����typedef
//������ĺ���ָ���ʾһ�������
typedef void (*CommandFun) (Server*,Client*,string,string&);

//�����
class Server{
public:
	SOCKET sListen;
	SOCKADDR_IN local;
	//�˿ں�
	int port;
	//����˴洢��Դ�����ݿ�
	//���ݿ����� ÿ��Ԫ����һ��MyDB���͵�ָ��
	myDB* DB[dbNum];
	//�����
	//��һ��map��ʾ�洢�ڷ���˵������
	//keyΪ��������֣�valueΪһ��ָ��
	//����ָ�룬�������������ָ���Ӧ�ĺ���
	map<string,CommandFun> Commands;
};

//�ͻ���
class Client{
public:
	SOCKET sClient;
	SOCKADDR_IN my_client;
	//�ͻ��˶�Ӧ�����ݿ�
	myDB db;
};

#endif