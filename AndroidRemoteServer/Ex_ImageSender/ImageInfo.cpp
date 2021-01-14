#include "ImageInfo.h"

ImageInfo::ImageInfo(void)
{		
	setWidth(480);					//�̹��� ���� ũ��
	setHeight(320);					//�̹��� ���� ũ��
	setQuality(30);					//�̹��� ȭ�� 0 ~ 100
	setDelay(500);					//�̹��� ���� ������ x/1000 �� ����
	setPath(L"d://screenshot.jpg");	//�̹����� �ӽ� ����� ���
}

ImageInfo::~ImageInfo(void)
{
}

//�̹��� ���� ��� ����
void ImageInfo::setPath(TCHAR *p)
{
	_tcscpy(path, p);
}

//�̹��� ���� ũ�� ����
void ImageInfo::setWidth(int w)						
{
	width = w;
}
//�̹��� ���� ũ�� ����
void ImageInfo::setHeight(int h)	
{
	height = h;
}

//�̹��� ����Ƽ ����
void ImageInfo::setQuality(ULONG q)
{
	Quality = q;
}

//�̹��� ������ ����
void ImageInfo::setDelay(int d)
{
	delay = d;
}
