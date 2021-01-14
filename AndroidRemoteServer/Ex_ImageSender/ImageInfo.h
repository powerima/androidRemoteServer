/*
	이미지 옵션 지정하기 위한 클래스
*/

#include <tchar.h>
#include <winsock2.h>


class ImageInfo
{
public:
	ImageInfo(void);
	~ImageInfo(void);

protected:
	ULONG Quality;					//jpg 퀄리티 0, 5, 10, 50, 100
	int width;						//jpg 축소 및 확대 가로 크기
	int height;						//jpg 축소 및 확대 세로 크기
	int delay;						//jpg 전송 프레임 - delay/1000초당 한장
	TCHAR path[MAX_PATH];			//jpg 이미지 저장 경로


	TCHAR* getPath();
	int getWidth();
	int getHeight();
	int getQuality();
	int getDelay();

	void setPath(TCHAR *p);						//이미지 저장 경로 설정
	void setWidth(int w);						//이미지 가로 크기 설정
	void setHeight(int h);						//이미지 세로 크기 설정
	void setQuality(ULONG q);					//이미지 퀄리티 설정
	void setDelay(int d);						//이미지 프레임 설정

};
