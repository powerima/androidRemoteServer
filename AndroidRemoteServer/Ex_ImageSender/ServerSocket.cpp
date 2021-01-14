/*
	���� ����� ���� Ŭ����

*/

#include "ServerSocket.h"


//���� �Լ� ���� ��� �� ����
void ServerSocket::err_quit(TCHAR *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

//���� �Լ� ���� ���
void ServerSocket::err_display(TCHAR *msg)
{
	LPVOID lpMsgBuf;
	
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
		NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);	
	_wsetlocale(LC_ALL, _T("korean"));
	_tprintf(_T("[%s] %s\n"), msg, (TCHAR *)lpMsgBuf);	
	LocalFree(lpMsgBuf);
}
//���� ���� ���� - ����� ���� �޼ҵ�
void ServerSocket::setServerSock()	
{
	//���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return;

	//GDI �ʱ�ȭ
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


//Ŭ���̾�Ʈ �ڵ鷯
void ServerSocket::clientHandle()
{	
	HANDLE hThread[2];

	while(1){
		//accept()
		addrlen = sizeof(clientaddr);
		printf("\n[TCP ����] Ŭ���̾�Ʈ ������ ����մϴ�.\n");

		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET) {
			err_display(L"accept()");
			break;
		}
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP ����=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		
		
		//����, ���� ������ ����
		startThread(hThread);
		
		//������ ���� ���
		WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
			
		CloseHandle(hThread[0]);
		CloseHandle(hThread[1]);

		//closesocket()
		closesocket(client_sock);
	}

}

//�������� ������ �̹��� ���� ����
char *ServerSocket::getImageBuf(int *bufSize)
{	

	s.screenSave(path, Quality , width, height);

	f.fileOpen("d://screenshot.jpg", "rb", 1);	
	*bufSize = f.getFileSize();
	imageBuf = f.getFileBuf();
	f.fileClose();

	return imageBuf;
}


//Ŭ���̾�Ʈ���� ������ �۽�
void ServerSocket::sendClient(SOCKET sock)
{
	
	printf("[TCP ����] ȭ�� ������ �����մϴ�.\n");

	while(1){		
		getImageBuf(&bufSize);

		
		//printf("[TCP ����] ���ۿ� �о�帰 ���� ũ�� =%d\n", bufSize);
		//printf("[TCP ����] ���ۿ� �о�帰 ũ�� = 0x%x\n", bufSize);

		
		//send() - ���� ũ�� ����	
		retval = send(sock, (char *)&bufSize, sizeof(int), 0);
		if(retval == SOCKET_ERROR){
			err_display(L"send()");
			break;
		}		
		//printf("[TCP ����] %d����Ʈ�� ���½��ϴ�.\n", retval);	



		//send() - �̹��� ����
		retval = send(sock, (char *)imageBuf, bufSize, 0);
		if(retval == SOCKET_ERROR){
			err_display(L"send()");
			break;	
		}
		printf("[TCP ����] %d����Ʈ�� ���½��ϴ�.\n", retval);


		free(imageBuf);
		Sleep(delay);		
	}
	printf("[TCP ����] ȭ�� ������ �����մϴ�.\n");
	
}

//Ŭ���̾�Ʈ�κ��� ������ ����
void ServerSocket::recvClient(SOCKET sock)
{
	printf("[TCP ����] Ŭ���̾�Ʈ�� ���� ��� ���� ���\n");
	while(1){				
		retval = recv(sock, buf, BUFSIZE, 0);
		if(retval == SOCKET_ERROR){
			err_display(L"recv()");
			break;
		}		
		
		msg = atoi(buf);
		
		puts("");
		printf("[���� ������] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[���� ������] %d\n",msg);		
		response(msg);
	}
	printf("[TCP ����] Ŭ���̾�Ʈ�� ���� ��� ���� ����\n");
	
}


//���۽�����
DWORD WINAPI ServerSocket::sendThread(LPVOID arg)
{
	ServerSocket *threadSock = (ServerSocket *)arg;
	if(threadSock == NULL){
		return 0;
	}
	threadSock->sendClient(threadSock->client_sock);	

	return 0;	
}

//���Ž�����
DWORD WINAPI ServerSocket::recvThread(LPVOID arg)
{
	ServerSocket *threadSock = (ServerSocket *)arg;
	if(threadSock == NULL){
		return 0;
	}
	threadSock->recvClient(threadSock->client_sock);	

	return 0;
}


//������ ����
void ServerSocket::startThread(HANDLE hHandle[])
{	
	hHandle[0] = CreateThread(NULL, 0, sendThread, this, 0, NULL);
	if(hHandle[0] == NULL) return;

	
	hHandle[1] = CreateThread(NULL, 0, recvThread, this, 0, NULL);
	if(hHandle[1] == NULL) return;	
}





//����� ���� ������ ���� �Լ�
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

//���� ��Ŷ�� �м��� Ŭ���̾�Ʈ ��û�� ���� �۾�����
void ServerSocket::response(int arg)
{
	MessageDefine::response(arg);		

	posx = posx * ScreenWidth / width;
	posy = posy * ScreenHeight / height;

	printf("[���� �޽���] id = %d posx = %d, posy = %d\n\n", id, posx, posy);
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