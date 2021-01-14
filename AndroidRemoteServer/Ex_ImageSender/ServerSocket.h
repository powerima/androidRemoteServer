/*
	���� ����� ���� Ŭ����
*/

#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include "FileHandle.h"
#include "ScreenCapture.h"
#include "MessageDefine.h"
#include "ImageInfo.h"

#define BUFSIZE 128
#define SERVERPORT 9000

class ServerSocket:public MessageDefine, public ImageInfo{
	
private:
	int retval;						//���� �Լ� ���� ����
	int addrlen;					//�ּ� ����ü ũ��
	int bufSize;					//�̹��� ���� ũ��
	int msg;
	char *imageBuf;					//�̹��� ����
	char buf[BUFSIZE];				//������ ���� ���� - ������
	SOCKET listen_sock;				//���� ����
	SOCKET client_sock;				//Ŭ���̾�Ʈ ����
	SOCKADDR_IN serveraddr;			//���� �ּ� ����ü
	SOCKADDR_IN clientaddr;			//Ŭ���̾�Ʈ �ּ� ����ü

	ScreenCapture s;
	FileHandle f;

protected:	

	void err_quit(TCHAR *msg);					//���� �Լ� ���� ��� �� ����
	void err_display(TCHAR *msg);				//���� �Լ� ���� ���	
	char *getImageBuf(int *bufSize);			//�������� ������ �̹��� ���� ����
	char *getImageBuf(int *bufSize, ULONG Quality, int width, int height);
	void sendClient(SOCKET sock);				//Ŭ���̾�Ʈ���� ������ �۽�
	void recvClient(SOCKET sock);				//Ŭ���̾�Ʈ�κ��� ������ ����
	int recvn(SOCKET s, char *buf, int len, int flags);	//����� ���� ������ ���� �Լ�

public:
	//�̹��� ũ��� ����Ƽ ����Ʈ��
	ServerSocket(){
		MessageDefine();
		ImageInfo();


	}
	~ServerSocket(){
		//closesocket()
		closesocket(listen_sock);
		
		//���� ����
		WSACleanup();
	}
	
	static DWORD WINAPI sendThread(LPVOID arg);	//sendClient �� ������ �Լ�
	static DWORD WINAPI recvThread(LPVOID arg);	//recvClient �� ������ �Լ�
	void startThread(HANDLE hHandle[]);			//������ ����
	void stopThread();							//������ ����
	void clientHandle();						//Ŭ���̾�Ʈ ���ӽ� ���
	void setServerSock();						//���� ���� ���� - ����� ���� �޼ҵ�	
	void response(int arg);						//���� ��Ŷ�� �м��� Ŭ���̾�Ʈ ��û�� ���� �۾�����
	
};