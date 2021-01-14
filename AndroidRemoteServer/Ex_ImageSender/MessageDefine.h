/*
	���� ����� ���� �޽��� ����
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


	void response(int arg);		//���� ��Ŷ�� �м��� Ŭ���̾�Ʈ ��û�� ���� �۾�����
	


protected:
	int id;				//��Ŷ ���̵�
	int num;			//�ɼ� ��ġ
	int posx;			//���콺 x
	int posy;			//���콺 y
	char ch;			//Ű ��
	int ScreenWidth;	//���� ����ȭ���� ���� ũ��
	int ScreenHeight;	//���� ����ȭ���� ���� ũ��
};
