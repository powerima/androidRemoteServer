/*
	��ũ���� ���� Ŭ����
*/

#pragma comment(lib, "gdiplus")
#include <windows.h>
#include <gdiplus.h>
#include <tchar.h>

using namespace Gdiplus;

class ScreenCapture{
private:				
	HBITMAP hBit;					//ȭ�� DC�� ������ ��Ʈ�� �ڵ�
	TCHAR BmpPath[MAX_PATH];		//��Ʈ���� ������ ���
	ULONG_PTR gpToken;				//Gdi+ ���̺귯�� �ʱ�ȭ�� ���� ����
    GdiplusStartupInput gpsi;		//Gdi+ ���̺귯�� �ʱ�ȭ�� ���� ����

protected:
	HBITMAP makeDIBSection(HDC dc, int width, int height);		//���ڷ� ���޵� DC�� ȣȯ�Ǵ� DDB����
	bool getEncCLSID(WCHAR *mime, CLSID *pClsid);				//�̹��� ���� ��ȯ�� ���� CLSID ��� �Լ�
	Bitmap *alterImageSize(Bitmap *b, int witdh, int height);	//�̹��� ũ�⸦ �ٲ㼭 ���ο� Bitmap �����͸� ����
		
	
	

public:	
	~ScreenCapture(){
		if(hBit == NULL)
			DeleteObject(hBit);
	}
	bool initGdi();												//GDI+ ���̺귯�� �ʱ� �Լ�
	void screenSave(HWND hWnd, ULONG Quality);					//����ȭ���� ���Ϸ� �����Ͽ� �����ϴ� �Լ�
	void screenSave(TCHAR path[], ULONG Quality);				//����ȭ���� ���Ϸ� �����Ͽ� �����ϴ� �Լ�
	void screenSave(TCHAR path[], ULONG Quality, int width, int height);		
	
	void drawBitmap(HDC hdc , HBITMAP hBitmap, int isSrcSize);	//��Ʈ���� ȭ�� DC�� ����ϴ� �Լ�		
	HBITMAP scrCapture();										//ȭ�� ������ ��Ʈ�� ������ �ٲٴ� �Լ�	
	HBITMAP scrCapture(int width, int height);					//ȭ�� ������ ��Ʈ�� ������ �ٲٴ� �Լ�	- ���ڷ� ���� ������ŭ ũ�� ����
	
};
