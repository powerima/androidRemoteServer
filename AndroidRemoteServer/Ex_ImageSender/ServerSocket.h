/*
	소켓 통신을 위한 클래스
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
	int retval;						//소켓 함수 상태 변수
	int addrlen;					//주소 구조체 크기
	int bufSize;					//이미지 버퍼 크기
	int msg;
	char *imageBuf;					//이미지 버퍼
	char buf[BUFSIZE];				//데이터 수신 버퍼 - 사용안함
	SOCKET listen_sock;				//서버 소켓
	SOCKET client_sock;				//클라이언트 소켓
	SOCKADDR_IN serveraddr;			//서버 주소 구조체
	SOCKADDR_IN clientaddr;			//클라이언트 주소 구조체

	ScreenCapture s;
	FileHandle f;

protected:	

	void err_quit(TCHAR *msg);					//소켓 함수 오류 출력 후 종료
	void err_display(TCHAR *msg);				//소켓 함수 오류 출력	
	char *getImageBuf(int *bufSize);			//소켓으로 전송할 이미지 버퍼 셋팅
	char *getImageBuf(int *bufSize, ULONG Quality, int width, int height);
	void sendClient(SOCKET sock);				//클라이언트으로 데이터 송신
	void recvClient(SOCKET sock);				//클라이언트로부터 데이터 수신
	int recvn(SOCKET s, char *buf, int len, int flags);	//사용자 정의 데이터 수신 함수

public:
	//이미지 크기와 퀄리티 디폴트값
	ServerSocket(){
		MessageDefine();
		ImageInfo();


	}
	~ServerSocket(){
		//closesocket()
		closesocket(listen_sock);
		
		//윈속 종료
		WSACleanup();
	}
	
	static DWORD WINAPI sendThread(LPVOID arg);	//sendClient 의 스레드 함수
	static DWORD WINAPI recvThread(LPVOID arg);	//recvClient 의 스레드 함수
	void startThread(HANDLE hHandle[]);			//스레드 시작
	void stopThread();							//스레드 종료
	void clientHandle();						//클라이언트 접속시 통신
	void setServerSock();						//서버 소켓 셋팅 - 사용자 정의 메소드	
	void response(int arg);						//받은 패킷을 분석후 클라이언트 요청에 대한 작업수행
	
};