//����� https://www.cnblogs.com/maowang1991/p/3567788.html
#include <WINSOCK2.H>   
#include <stdio.h>     

//�˿ں�                    
#define PORT           5150    
//������Ϣ������ֽڳ���
#define MSGSIZE        1024    
                   
#pragma comment(lib, "ws2_32.lib")      
        
//��������Ҫһ�������Դ��db���飬ÿ���ͻ���һ���ӣ�
//�ʹ���һ��Client�������������Ӧ����Դdb[i]��
//db����һ���ֵ乹�ɵ����飬�ö�Ӧ��Client���key-value
//�ͻ������������������������в�֣�Ȼ��������
//���в�ѯ�����Ƿ�Ϸ���ִ��set�����key-value
//�洢��db���ֵ������У���󷵻���Ӧ���ͻ��ˡ�
           
int main()      
{      
	//��ʼ������
    WSADATA wsaData;      
    SOCKET sListen;      
    SOCKET sClient;      
    SOCKADDR_IN local;      
    SOCKADDR_IN client; 
	//��Ϣ����
    char szMessage[MSGSIZE];      
    int ret;      
    int iaddrSize = sizeof(SOCKADDR_IN); 
	//��ʼ��Windows�׽��ֿ�
    WSAStartup(0x0202, &wsaData);      
              
	//�����ͻ��˵��׽���
    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);      
                   
	//��
    local.sin_family = AF_INET;      
    local.sin_port = htons(PORT);      
    local.sin_addr.s_addr = htonl(INADDR_ANY);      
    bind(sListen, (struct sockaddr *) &local, sizeof(SOCKADDR_IN));      
       
	//����
	//1��ʾ�ȴ����Ӷ��е���󳤶�
    listen(sListen, 1);      
            
	//�ȴ��ͻ�������
    sClient = accept(sListen, (struct sockaddr *) &client, &iaddrSize);      
    printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr),      
            ntohs(client.sin_port));      
                   
    while (TRUE) {     
		//ret�Ƿ�����Ϣ���ֽڳ���
		//szMessage�Ƿ�����Ϣ������
        ret = recv(sClient, szMessage, MSGSIZE, 0); 
        szMessage[ret] = '\0';      
        printf("Received [%d bytes]: '%s'\n", ret, szMessage);      
    }      
    return 0;      
  }