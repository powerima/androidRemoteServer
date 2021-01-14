/*
	스크린샷 저장 클래스
*/

#pragma comment(lib, "gdiplus")
#include <windows.h>
#include <gdiplus.h>
#include <tchar.h>

using namespace Gdiplus;

class ScreenCapture{
private:				
	HBITMAP hBit;					//화면 DC를 저장할 비트맵 핸들
	TCHAR BmpPath[MAX_PATH];		//비트맵을 저장할 경로
	ULONG_PTR gpToken;				//Gdi+ 라이브러리 초기화를 위한 변수
    GdiplusStartupInput gpsi;		//Gdi+ 라이브러리 초기화를 위한 변수

protected:
	HBITMAP makeDIBSection(HDC dc, int width, int height);		//인자로 전달된 DC에 호환되는 DDB생성
	bool getEncCLSID(WCHAR *mime, CLSID *pClsid);				//이미지 포맷 변환을 위한 CLSID 얻는 함수
	Bitmap *alterImageSize(Bitmap *b, int witdh, int height);	//이미지 크기를 바꿔서 새로운 Bitmap 포인터를 리턴
		
	
	

public:	
	~ScreenCapture(){
		if(hBit == NULL)
			DeleteObject(hBit);
	}
	bool initGdi();												//GDI+ 라이브러리 초기 함수
	void screenSave(HWND hWnd, ULONG Quality);					//바탕화면을 파일로 압축하여 저장하는 함수
	void screenSave(TCHAR path[], ULONG Quality);				//바탕화면을 파일로 압축하여 저장하는 함수
	void screenSave(TCHAR path[], ULONG Quality, int width, int height);		
	
	void drawBitmap(HDC hdc , HBITMAP hBitmap, int isSrcSize);	//비트맵을 화면 DC에 출력하는 함수		
	HBITMAP scrCapture();										//화면 내용을 비트맵 정보로 바꾸는 함수	
	HBITMAP scrCapture(int width, int height);					//화면 내용을 비트맵 정보로 바꾸는 함수	- 인자로 받은 변수만큼 크기 변경
	
};
