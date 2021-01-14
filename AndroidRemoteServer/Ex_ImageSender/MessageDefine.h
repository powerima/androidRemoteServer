/*
	소켓 통신을 위한 메시지 정의
*/
#include <string.h>
#include <windows.h>

static const int IMAGE_QUALITY = 01;
static const int IMAGE_SIZE = 02;
static const int IMAGE_FRAME = 03;
static const int MOUSE_MOVE = 10;
static const int MOUSE_LEFT_PRESSED = 11;
static const int MOUSE_LEFT_RELEASED = 12;
static const int MOUSE_LEFT_CLICKED = 13;
static const int MOUSE_RIGHT_PRESSED = 21;
static const int MOUSE_RIGHT_RELEASED = 22;
static const int MOUSE_RIGHT_CLICKED = 23;
static const int MOUSE_ENTERED = 31;
static const int MOUSE_EXITED = 30;
static const int KEY_PRESSED = 41;
static const int KEY_RELEASED = 42;


class MessageDefine
{
public:
	MessageDefine(void);
	~MessageDefine(void);


	void response(int arg);		//받은 패킷을 분석후 클라이언트 요청에 대한 작업수행
	


protected:
	int id;				//패킷 아이디
	int num;			//옵션 수치
	int posx;			//마우스 x
	int posy;			//마우스 y
	char ch;			//키 값
	int ScreenWidth;	//서버 바탕화면의 가로 크기
	int ScreenHeight;	//서버 바탕화면의 세로 크기
};
