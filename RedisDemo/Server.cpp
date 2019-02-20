#pragma warning(disable: 4786)
//����� https://www.cnblogs.com/maowang1991/p/3567788.html
#include <WINSOCK2.H>   
#include <stdio.h>     
#include <string>
#include <iostream>
#include<Windows.h>
#include<process.h>
#include <malloc.h>
#include <map>
#include <vector>
#include "myRedis.h"
using namespace std;

//���ݿ�����ĸ���
#define dbNum 15
//�˿ں�                    
#define PORT           5150    
//������Ϣ������ֽڳ���
#define MSGSIZE        1024    
                   
#pragma comment(lib, "ws2_32.lib")  

#define MAXCLIENTNUM 15
        
//��������Ҫһ�������Դ��db���飬ÿ���ͻ���һ���ӣ�
//�ʹ���һ��Client�������������Ӧ����Դdb[i]��
//db����һ���ֵ乹�ɵ����飬�ö�Ӧ��Client���key-value
//�ͻ������������������������в�֣�Ȼ��������
//���в�ѯ�����Ƿ�Ϸ���ִ��set�����key-value
//�洢��db���ֵ������У���󷵻���Ӧ���ͻ��ˡ�
typedef void (*CommandFun) (Server*,Client*,string,string&);
typedef map<string,CommandFun> mymap;
typedef map<string,string> myDB;
//���������
Server* server=new Server();

//�ָ��ַ����ķ���
vector<string> split(string str,string ch){
	vector<string>vec;
	if(""==str){
		return vec;
	}
	string strs=str+ch;

	size_t pos=strs.find(ch);
	size_t size=strs.size();
	
	//nposΪ����λ��
	while(pos!=std::string::npos){
		string x=strs.substr(0,pos);
		vec.push_back(x);
		strs=strs.substr(pos+1,size);
		pos=strs.find(ch);
	}
	return vec;
}

//get����
void getCommand(Server*server,Client*client,string key,string&value){
	myDB::iterator it;
	it=client->db.find(key);
	if(it==client->db.end()){
		cout<<"get null"<<endl;
	}else{
		//cout<<"get����:"<<it->second<<endl;
		value=it->second;
	}
}

//set����
void setCommand(Server*server,Client*client,string key,string&value){
	client->db.insert(pair<string,string>(key,value));
	//cout<<"set ����"<<endl;
}

//��ʼ�����ݿ�
void initDB(){
	//cout<<"init db..."<<endl;
	cout<<"Redis by ShimmerPig start-up..."<<endl;
}

//��ʼ�������
void initCommand(Server*&server){
	(server->Commands).insert(pair<string,CommandFun>("set",&setCommand));
/*	Client*c1=new Client();
	mymap::iterator it;
	it=server->Commands.find("get");
	CommandFun cmd=it->second;
	cmd(server,c1);*/

	(server->Commands).insert(pair<string,CommandFun>("get",&getCommand));
/*	Client*c1=new Client();
	mymap::iterator it;
	it=server->Commands.find("get");
	CommandFun cmd=it->second;
	cmd(server,c1);*/

}


//��ʼ�������
void initServer(Server*&server){
	//�����ͻ��˵��׽���
	server->sListen=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	//��ʼ�����ݿ�
	initDB();

	//��ʼ�������
	initCommand(server);

	//��
	(server->local).sin_family=AF_INET;  
	(server->local).sin_port=htons(PORT); 
	(server->local).sin_addr.s_addr = htonl(INADDR_ANY); 
	bind(server->sListen, (struct sockaddr *) &(server->local), sizeof(SOCKADDR_IN));
                 
    //����
	//1��ʾ�ȴ����Ӷ��е���󳤶�
    listen(server->sListen, 1);   
}


//���������߳�
DWORD WINAPI WorkerThread(LPVOID lpParam){
	char szMessage[MSGSIZE];      
	int ret;
	Client *c=(Client*)lpParam;

	while(true){
		//ret�Ƿ�����Ϣ���ֽڳ���
		//szMessage�Ƿ�����Ϣ������
		ret = recv(c->sClient, szMessage, MSGSIZE, 0); 
		szMessage[ret] = '\0';      
		printf("Received [%d bytes]: '%s'\n", ret, szMessage); 

		//����ת��������Ƚ�
		string message(szMessage);
		//���û����͵���Ϣ�����ж�
		if(message=="-v"||message=="--version"){
			char *str="Redis by Shimmer Pig v=0.0.1000 <00000000/0> 64 bit";
			send(c->sClient,str,strlen(str)+sizeof(char),NULL);
			cout<<"����ɹ�ִ�У�������ͻ��˷����� �汾��Ϣ"<<endl;
		}else if(message=="-h"||message=="--help"){
			char *str="help document [��δ����]";
			send(c->sClient,str,strlen(str)+sizeof(char),NULL);
			cout<<"����ɹ�ִ�У�������ͻ��˷����� �����ĵ���Ϣ"<<endl;	
		}else{
			//cout<<"else hh"<<endl;
			//�Է��͵���Ϣ���зָ�鿴�Ƿ����get/set����ĸ�ʽ
			//�ָ���
			string ch=" ";
			vector<string>vec=split(message,ch);

			//ִ��set����
			if(vec[0]=="set"){
				//���������ʽ
				if(vec.size()==3){
					//cout<<"ok"<<endl;
					//��ü�ֵ
					string key=vec[1];
					string value=vec[2];
					cout<<vec[0]<<" "<<vec[1]<<" "<<vec[2]<<endl;

					mymap::iterator it;
					it=server->Commands.find("set");
					if(it==server->Commands.end()){
						cout<<"can't find"<<endl;
					}else{
						CommandFun cmd=it->second;
						cmd(server,c,key,value);
						//��ͻ��˷�������
						char *str="set ok";
						send(c->sClient,str,strlen(str)+sizeof(char),NULL);
						cout<<"����ɹ�ִ�У�������ͻ��˷����� set ok"<<endl;
					}

					//�����������ʽ
				}else{
					char *str="(error) ERR wrong number of arguments for 'set' command";
					send(c->sClient,str,strlen(str)+sizeof(char),NULL);
					cout<<"��ͻ��˷��� (error) ERR wrong number of arguments for 'set' command"<<endl;
				}
			//ִ��get����
			}else if(vec[0]=="get"){
				//���������ʽ
				if(vec.size()==2){
					cout<<"ok"<<endl;
					//��ü�ֵ
					string key=vec[1];
					string value="";
					cout<<vec[0]<<" "<<vec[1]<<endl;

					mymap::iterator it;
					it=server->Commands.find("get");
					if(it==server->Commands.end()){
						cout<<"can't find"<<endl;
					}else{
						CommandFun cmd=it->second;
						cmd(server,c,key,value);
						//��ͻ��˷�������
						//char *str;
						//int len=value.length();
						//str=(char*)malloc((len+1)*sizeof(char));
						//value.copy(str,len,0);
						char*str=new char[strlen(value.c_str())];
						strcpy(str,value.c_str());
						send(c->sClient,str,strlen(str)+sizeof(char),NULL);
						cout<<"����ɹ�ִ�У�������ͻ��˷����� "<<value<<endl;
					}
					//�����������ʽ
				}else{
					//��ͻ��˷�������
					char *str="(error) ERR wrong number of arguments for 'get' command";
					send(c->sClient,str,strlen(str)+sizeof(char),NULL);
					cout<<"��ͻ��˷��� (error) ERR wrong number of arguments for 'get' command"<<endl;
				}
			}else{
				//��ͻ��˷�������
				char *str="(error) ERR no such a command";
				send(c->sClient,str,strlen(str)+sizeof(char),NULL);
				cout<<"��ͻ��˷��� (error) ERR no such a command"<<endl;
			}
		}
		//
		//chuLi(message,c,server);

		//��ͻ��˷�������
/*		char *str="������Ƿ�����";
		send(c->sClient,str,strlen(str)+sizeof(char), NULL);
		cout<<"�ɹ���ͻ��˷�������"<<endl;*/
	}

	return 0;
}
int main()      
{
	WSADATA wsaData;  
	int iaddrSize = sizeof(SOCKADDR_IN); 
	//�����ͻ���
	Client* client;
 
	DWORD dwThreadId;
      
	
	//��ʼ��Windows�׽��ֿ�
    WSAStartup(0x0202, &wsaData);
	
	//��ʼ�������
	initServer(server);      

    //����̵߳ľ��
    HANDLE hThread[MAXCLIENTNUM];

    for(int i=0;i<MAXCLIENTNUM;i++){     
		//�ȴ��ͻ�������
		Client* client=new Client();
		client->sClient = accept(server->sListen, (struct sockaddr *) &(client->my_client), &iaddrSize);      
		printf("Accepted client:%s:%d\n", inet_ntoa((client->my_client).sin_addr),      
            ntohs((client->my_client).sin_port)); 
		//���ͻ����������ж�Ӧ�����ݿ�����
		server->DB[i]=&(client->db);
		//client->db.insert(pair<string,string>("zhu","loveu"));
		/*myDB::iterator it;
		it=client->db.find("zhu");
		if(it==client->db.end()){
			cout<<"can't find"<<endl;
		}else{
			cout<<it->second<<endl;
		}*/

		//���������߳�
		hThread[i]=CreateThread(NULL, 0, WorkerThread, client, 0, NULL);
    }      
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
    //�ر��׽���  
    closesocket(server->sListen);
	cout << "�߳�ȫ��ִ�����" << endl;
    return 0;      
}