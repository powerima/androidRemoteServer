#include "MessageDefine.h"

MessageDefine::MessageDefine(void)
{
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);		
}

MessageDefine::~MessageDefine(void)
{
}


//받은 패킷을 분석후 클라이언트 요청에 대한 작업수행
void MessageDefine::response(int arg)
{
	id = arg>>24;	

	
	switch(id){
		case IMAGE_QUALITY:
		case IMAGE_FRAME :
			num = arg & 0x00ffffff;
			break;

		case IMAGE_SIZE :		

		case MOUSE_MOVE:		

		case MOUSE_LEFT_PRESSED :		

		case MOUSE_LEFT_RELEASED :		

		case MOUSE_RIGHT_PRESSED :	

		case MOUSE_RIGHT_RELEASED :		

		case MOUSE_LEFT_CLICKED :		

		case MOUSE_RIGHT_CLICKED :
			posx = (arg>>12) & 0xfff;
			posy = arg & 0xfff;
			break;


		case MOUSE_ENTERED :		

		case MOUSE_EXITED :
			break;

		
		case KEY_PRESSED :		

		case KEY_RELEASED :
			ch |= arg & 0xff;
			break;
		
	
	}
	
}