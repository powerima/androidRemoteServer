/*
	�̹��� �ɼ� �����ϱ� ���� Ŭ����
*/

#include <tchar.h>
#include <winsock2.h>


class ImageInfo
{
public:
	ImageInfo(void);
	~ImageInfo(void);

protected:
	ULONG Quality;					//jpg ����Ƽ 0, 5, 10, 50, 100
	int width;						//jpg ��� �� Ȯ�� ���� ũ��
	int height;						//jpg ��� �� Ȯ�� ���� ũ��
	int delay;						//jpg ���� ������ - delay/1000�ʴ� ����
	TCHAR path[MAX_PATH];			//jpg �̹��� ���� ���


	TCHAR* getPath();
	int getWidth();
	int getHeight();
	int getQuality();
	int getDelay();

	void setPath(TCHAR *p);						//�̹��� ���� ��� ����
	void setWidth(int w);						//�̹��� ���� ũ�� ����
	void setHeight(int h);						//�̹��� ���� ũ�� ����
	void setQuality(ULONG q);					//�̹��� ����Ƽ ����
	void setDelay(int d);						//�̹��� ������ ����

};
