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
typedef map<string,string> myDB;
class Server;
class Client;
//������ĺ���ָ�����typedef
typedef void (*CommandFun) (Server*,Client*,string,string&);

//�洢��Դʹ�õ����ݿ���
/*class MyDB{
public:
	//�����Դ���ֵ�
	dict Dict;
};*/

//�����
class Server{
public:
	SOCKET sListen;
	SOCKADDR_IN local;
	//���ݿ����� ÿ��Ԫ����һ��MyDB���͵�ָ��
	myDB* DB[dbNum];
	//�˿ں�
	int port;
	//�����
	//��һ��map��ʾ keyΪ���������֣�valueΪһ��ָ������ָ�룬���ݺ���������ָ���Ӧ�ĺ���
	map<string,CommandFun> Commands;
};

//�ͻ���
class Client{
public:
	//���ݿ��num
	//int dbNum;
	SOCKET sClient;
	SOCKADDR_IN my_client;
	//�ͻ��˶�Ӧ�����ݿ�
	myDB db;
};

#endif