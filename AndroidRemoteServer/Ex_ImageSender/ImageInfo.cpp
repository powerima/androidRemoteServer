#include "ImageInfo.h"

ImageInfo::ImageInfo(void)
{		
	setWidth(480);					//이미지 가로 크기
	setHeight(320);					//이미지 세로 크기
	setQuality(30);					//이미지 화질 0 ~ 100
	setDelay(500);					//이미지 전송 프레임 x/1000 초 단위
	setPath(L"d://screenshot.jpg");	//이미지가 임시 저장될 경로
}

ImageInfo::~ImageInfo(void)
{
}

//이미지 저장 경로 설정
void ImageInfo::setPath(TCHAR *p)
{
	_tcscpy(path, p);
}

//이미지 가로 크기 설정
void ImageInfo::setWidth(int w)						
{
	width = w;
}
//이미지 세로 크기 설정
void ImageInfo::setHeight(int h)	
{
	height = h;
}

//이미지 퀄리티 설정
void ImageInfo::setQuality(ULONG q)
{
	Quality = q;
}

//이미지 프레임 설정
void ImageInfo::setDelay(int d)
{
	delay = d;
}
