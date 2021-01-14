/*
	소켓 통신을 위한 클래스

*/

#include "ServerSocket.h"


//소켓 함수 오류 출력 후 종료
void ServerSocket::err_quit(TCHAR *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

//소켓 함수 오류 출력
void ServerSocket::err_display(TCHAR *msg)
{
	LPVOID lpMsgBuf;
	
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);	
	_wsetlocale(LC_ALL, _T("korean"));
	_tprintf(_T("[%s] %s\n"), msg, (TCHAR *)lpMsgBuf);	
	LocalFree(lpMsgBuf);
}
//서버 소켓 셋팅 - 사용자 정의 메소드
void ServerSocket::setServerSock()	
{
	//윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return;

	//GDI 초기화
	s.initGdi();

	//socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit(L"socket()");

	//bind()
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = ntohs(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit(L"bind()");
	
	//listen()
	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) err_quit(L"listen()");	
}


//클라이언트 핸들러
void ServerSocket::clientHandle()
{	
	HANDLE hThread[2];

	while(1){
		//accept()
		addrlen = sizeof(clientaddr);
		printf("\n[TCP 서버] 클라이언트 접속을 대기합니다.\n");

		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET) {
			err_display(L"accept()");
			break;
		}
		printf("[TCP 서버] 클라이언트 접속: IP 접속=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		
		
		//전송, 수신 스레드 시작
		startThread(hThread);
		
		//스레드 종료 대기
		WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
			
		CloseHandle(hThread[0]);
		CloseHandle(hThread[1]);

		//closesocket()
		closesocket(client_sock);
	}

}

//소켓으로 전송할 이미지 버퍼 셋팅
char *ServerSocket::getImageBuf(int *bufSize)
{	

	s.screenSave(path, Quality , width, height);

	f.fileOpen("d://screenshot.jpg", "rb", 1);	
	*bufSize = f.getFileSize();
	imageBuf = f.getFileBuf();
	f.fileClose();

	return imageBuf;
}


//클라이언트으로 데이터 송신
void ServerSocket::sendClient(SOCKET sock)
{
	
	printf("[TCP 서버] 화면 전송을 시작합니다.\n");

	while(1){		
		getImageBuf(&bufSize);

		
		//printf("[TCP 서버] 버퍼에 읽어드린 파일 크기 =%d\n", bufSize);
		//printf("[TCP 서버] 버퍼에 읽어드린 크기 = 0x%x\n", bufSize);

		
		//send() - 버퍼 크기 전송	
		retval = send(sock, (char *)&bufSize, sizeof(int), 0);
		if(retval == SOCKET_ERROR){
			err_display(L"send()");
			break;
		}		
		//printf("[TCP 서버] %d바이트를 보냈습니다.\n", retval);	



		//send() - 이미지 전송
		retval = send(sock, (char *)imageBuf, bufSize, 0);
		if(retval == SOCKET_ERROR){
			err_display(L"send()");
			break;	
		}
		printf("[TCP 서버] %d바이트를 보냈습니다.\n", retval);


		free(imageBuf);
		Sleep(delay);		
	}
	printf("[TCP 서버] 화면 전송을 종료합니다.\n");
	
}

//클라이언트로부터 데이터 수신
void ServerSocket::recvClient(SOCKET sock)
{
	printf("[TCP 서버] 클라이언트로 부터 명령 수신 대기\n");
	while(1){				
		retval = recv(sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){
			err_display(L"recv()");
			break;
		}		
		
		msg = atoi(buf);
		
		puts("");
		printf("[받은 데이터] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %d\n",msg);		
		response(msg);
	}
	printf("[TCP 서버] 클라이언트로 부터 명령 수신 종료\n");
	
}


//전송스레드
DWORD WINAPI ServerSocket::sendThread(LPVOID arg)
{
	ServerSocket *threadSock = (ServerSocket *)arg;
	if(threadSock == NULL){
		return 0;
	}
	threadSock->sendClient(threadSock->client_sock);	

	return 0;	
}

//수신스레드
DWORD WINAPI ServerSocket::recvThread(LPVOID arg)
{
	ServerSocket *threadSock = (ServerSocket *)arg;
	if(threadSock == NULL){
		return 0;
	}
	threadSock->recvClient(threadSock->client_sock);	

	return 0;
}


//스레드 시작
void ServerSocket::startThread(HANDLE hHandle[])
{	
	hHandle[0] = CreateThread(NULL, 0, sendThread, this, 0, NULL);
	if(hHandle[0] == NULL) return;

	
	hHandle[1] = CreateThread(NULL, 0, recvThread, this, 0, NULL);
	if(hHandle[1] == NULL) return;	
}





//사용자 정의 데이터 수신 함수
int ServerSocket::recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while(left > 0){
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if(received == 0)
			break;
		left -= received;
		ptr += received;
	}
	return (len - left);
}

//받은 패킷을 분석후 클라이언트 요청에 대한 작업수행
void ServerSocket::response(int arg)
{
	MessageDefine::response(arg);		

	posx = posx * ScreenWidth / width;
	posy = posy * ScreenHeight / height;

	printf("[받은 메시지] id = %d posx = %d, posy = %d\n\n", id, posx, posy);
	switch(id){
		
		case IMAGE_QUALITY:
			setQuality((ULONG)num);
			break;				

		case IMAGE_SIZE :
			setWidth(posx);
			setHeight(posy);
			break;

		case IMAGE_FRAME :
			setDelay(num);
			break;

		case MOUSE_MOVE:
					
			mouse_event(MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE, (DWORD)posx, (DWORD)posy, 0,0);

			break;
	
		case MOUSE_LEFT_PRESSED :
			SetCursorPos(posx, posy);			
			mouse_event(MOUSEEVENTF_LEFTDOWN, (DWORD)posx, (DWORD)posy, 0,0);
			mouse_event(MOUSEEVENTF_LEFTUP, (DWORD)posx, (DWORD)posy, 0,0);
			
			break;
	
		case MOUSE_LEFT_RELEASED :
			mouse_event(MOUSEEVENTF_LEFTUP, (DWORD)posx, (DWORD)posy, 0,0);
			
			break;

		case MOUSE_RIGHT_PRESSED :
			
			//SetCursorPos(posx, posy);
			mouse_event(MOUSEEVENTF_RIGHTDOWN|MOUSEEVENTF_ABSOLUTE, (DWORD)posx, (DWORD)posy, 0,0);
			break;
			
		case MOUSE_RIGHT_CLICKED :
			//SetCursorPos(posx, posy);
			
			mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0,0);
		
			mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0,0);
			break;

		case MOUSE_RIGHT_RELEASED :
			//SetCursorPos(posx, posy);
			mouse_event(MOUSEEVENTF_RIGHTUP|MOUSEEVENTF_ABSOLUTE, (DWORD)posx, (DWORD)posy, 0,0);
			break;

			


		case MOUSE_LEFT_CLICKED :
			//SetCursorPos(posx, posy);
			mouse_event(MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_ABSOLUTE, (DWORD)posx, (DWORD)posy, 0,0);
			mouse_event(MOUSEEVENTF_LEFTUP|MOUSEEVENTF_ABSOLUTE, (DWORD)posx, (DWORD)posy, 0,0);
			break;


		case MOUSE_ENTERED :
			break;

		case MOUSE_EXITED :
			break;

		case KEY_PRESSED :			
			keybd_event(num,0,0,0);
			keybd_event(num,0,0x0002,0);
			break;

		case KEY_RELEASED :
			break;
	
	
	}
}