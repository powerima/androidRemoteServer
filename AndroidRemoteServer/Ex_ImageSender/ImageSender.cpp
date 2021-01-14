/*
	이미지 전송

	- RemoteControl을 지원하기위한 서버 함수
	- 이미지 전송, 키보드와 마우스 제어
	- 이미지 퀄리티  0, 5, 10, 50, 100

*/


#include "ServerSocket.h"
#include <locale.h>



//원래 기능을 위한 함수
void start()
{
	ServerSocket s;
	s.setServerSock();
	s.clientHandle();
}



//스크린샷만 찍어서 저장하기위한 실험용 함수
void testSave()
{
	
	ScreenCapture s;
	s.initGdi();
	s.scrCapture();
	s.screenSave(L"d://screenshot3.jpg", 2, 1024, 864);	
	//s.screenSave(L"d://screenshot4.jpg", 100);
	
}

//키보드와 마우스 컨트롤 시험을 위한 함수
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

