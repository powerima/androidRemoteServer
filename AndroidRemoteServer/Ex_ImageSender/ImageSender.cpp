/*
	�̹��� ����

	- RemoteControl�� �����ϱ����� ���� �Լ�
	- �̹��� ����, Ű����� ���콺 ����
	- �̹��� ����Ƽ  0, 5, 10, 50, 100

*/


#include "ServerSocket.h"
#include <locale.h>



//���� ����� ���� �Լ�
void start()
{
	ServerSocket s;
	s.setServerSock();
	s.clientHandle();
}



//��ũ������ �� �����ϱ����� ����� �Լ�
void testSave()
{
	
	ScreenCapture s;
	s.initGdi();
	s.scrCapture();
	s.screenSave(L"d://screenshot3.jpg", 2, 1024, 864);	
	//s.screenSave(L"d://screenshot4.jpg", 100);
	
}

//Ű����� ���콺 ��Ʈ�� ������ ���� �Լ�
void keycontrol()
{
	for(int i=0; i<100; i+=10){
		for(int j=0; j<100; i+=10){
			mouse_event(MOUSEEVENTF_MOVE, i, j, 0, 0);
			Sleep(1000);
		}
	}
	
}
void main()
{	
	//testSave();
	//keycontrol();
	start();
}

