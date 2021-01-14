
#include "ScreenCapture.h"


//화면 내용을 비트맵 정보로 바꾸는 함수
HBITMAP ScreenCapture::scrCapture()	
{
	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	HDC hScrDC, hMemDC;
	HBITMAP hBitmap;

	//지정된 디바이스의 콘텍스트를 받아옴. DISPLAY 인수는 전체 화면 DC를 받아옴
	hScrDC = CreateDC(L"DISPLAY",NULL,NULL,NULL);
	//hScrDC와 같은 색상 포맷의 메모리 DC를 생성
	
	hMemDC = CreateCompatibleDC(hScrDC);
	//비트맵 생성하는 함수.  첫번째 hScrDC에 호환되는 비트맵 생성
	hBitmap = CreateCompatibleBitmap(hScrDC, ScreenWidth, ScreenHeight);
	//hBitmap = makeDIBSection(hScrDC, ScreenWidth, ScreenHeight);
	

	SelectObject(hMemDC, hBitmap);

	//비트맵을 복사하는 함수 hMemDC인수에 hScrDC 비트맵을 복사
	BitBlt(hMemDC,0,0,ScreenWidth, ScreenHeight, hScrDC, 0,0,SRCCOPY);	
	//BitBlt(hMemDC,10,30,600, 400, hScrDC, 0,0,SRCCOPY);	

	DeleteDC(hMemDC);
	DeleteDC(hScrDC);
	//InvalidateRect(hWnd, NULL, TRUE);

	return hBitmap;
}

//화면 내용을 비트맵 정보로 바꾸는 함수
HBITMAP ScreenCapture::scrCapture(int width, int height)	
{
	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	HDC hScrDC;
	HBITMAP hBitmap;

	//지정된 디바이스의 콘텍스트를 받아옴. DISPLAY 인수는 전체 화면 DC를 받아옴
	hScrDC = CreateDC(L"DISPLAY",NULL,NULL,NULL);

	//인자로 전달된 DC에 호환되는 DDB생성	
	hBitmap = makeDIBSection(hScrDC, ScreenWidth, ScreenHeight);		

	DeleteDC(hScrDC);
	
	return hBitmap;
}


//비트맵을 화면 DC에 출력하는 함수
void ScreenCapture::drawBitmap(HDC hdc, HBITMAP hBitmap, int isSrcSize)	
{
	HDC hMemDC;
	HBITMAP hOldBitmap;
	BITMAP bit;
	TCHAR str[100];
	int bx,by;
	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	hMemDC = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	GetObject(hBitmap, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;
	
	wsprintf(str, L"witdh: %d height : %d  pixel : %d bitType : %d",bit.bmWidth, bit.bmHeight, bit.bmBitsPixel, bit.bmType);
	


	switch(isSrcSize){
	case 0:	
		//비트맵 크기를 축소 또는 확대해서 DC에 복사하는 함수	
		StretchBlt(hdc, 0, 0, ScreenWidth, ScreenHeight, hMemDC, 0, 0, bx, by, SRCCOPY);
		TextOut(hdc, 10, 10, str, _tcslen(str));
		
		break;

	case 1:
		//비트맵 크기를 원본 그대로 DC에 복사하는 함수
		BitBlt(hdc, 0, 0, ScreenWidth, ScreenHeight, hMemDC, 0, 0, SRCCOPY);	
		break;

	}

	SelectObject(hdc, hOldBitmap);
	DeleteDC(hMemDC);

}

//인자로 전달된 DC에 호환되는 DDB생성
HBITMAP ScreenCapture::makeDIBSection(HDC dc, int width, int height)
{
	BITMAPINFO bmi;
	LPVOID pBits;
	HBITMAP hBitmap;

	memset(&bmi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biBitCount = 24;
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;	
	hBitmap = ::CreateDIBSection(dc, &bmi, DIB_RGB_COLORS, (void**)&pBits, NULL, 0);
	
	return hBitmap;
}

//GDI+ 라이브러리 초기화
bool ScreenCapture::initGdi()
{
	if (GdiplusStartup(&gpToken,&gpsi,NULL) != Ok) {
		MessageBox(NULL,TEXT("GDI+ 라이브러리를 초기화할 수 없습니다."),
		TEXT("알림"),MB_OK);
		return false;
	}
	return true;
}

//이미지 포맷 변환을 위한 CLSID 얻기
bool ScreenCapture::getEncCLSID(WCHAR *mime, CLSID *pClsid)
{
	UINT num,size,i;
	ImageCodecInfo *arCod;
	bool bFound=FALSE;

	GetImageEncodersSize(&num,&size);
	arCod=(ImageCodecInfo *)malloc(size);
	GetImageEncoders(num,size,arCod);

	for (i=0;i<num;i++) {
		if(wcscmp(arCod[i].MimeType,mime)==0) {
			*pClsid=arCod[i].Clsid;
			bFound=TRUE;
			break;
		}   
	}
	free(arCod);
	return bFound;
}


//스크린 저장- 퀄리티 옵션: 0, 5, 10, 50, 100
void ScreenCapture::screenSave(HWND hWnd, ULONG Quality)
{

	HBITMAP hBit = scrCapture();

	HPALETTE hpal; //팔레트 설정
	hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	 
	Bitmap image(hBit,hpal); //GDI 의 HBITMAP 을 GDI+의 Bitmap 으로 넘겨줌
	int width = image.GetWidth();
	int height = image.GetHeight();
	EncoderParameters Para;
	CLSID jpegClsid; // JPEG 포맷으로 저장	


	Para.Count=1;
	Para.Parameter[0].Guid=EncoderQuality;
	Para.Parameter[0].Type=EncoderParameterValueTypeLong;
	Para.Parameter[0].NumberOfValues=1;
	Para.Parameter[0].Value=&Quality;

	getEncCLSID(L"image/jpeg", &jpegClsid);

	image.Save(L"d://test.jpg", &jpegClsid, &Para);

	//오브젝트 해제
	DeleteObject(hpal);
	DeleteObject(hBit);

	//delete *image;

}

//스크린 저장- 퀄리티 옵션: 0, 5, 10, 50, 100
void ScreenCapture::screenSave(TCHAR path[], ULONG Quality)
{

	HBITMAP hBit = scrCapture();

	HPALETTE hpal; //팔레트 설정
	hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	
	
	Bitmap *image= new Bitmap(hBit,hpal); //GDI 의 HBITMAP 을 GDI+의 Bitmap 으로 넘겨줌
	//int width = image->GetWidth();
	//int height = image->GetHeight();
	EncoderParameters Para;
	CLSID jpegClsid; // JPEG 포맷으로 저장	


	Para.Count=1;
	Para.Parameter[0].Guid=EncoderQuality;
	Para.Parameter[0].Type=EncoderParameterValueTypeLong;
	Para.Parameter[0].NumberOfValues=1;
	Para.Parameter[0].Value=&Quality;

	getEncCLSID(L"image/png", &jpegClsid);

	image->Save(path, &jpegClsid, &Para);

	 
	//오브젝트 해제
	DeleteObject(hpal);
	DeleteObject(hBit);

	//delete image;

}

//스크린 저장- 퀄리티 옵션: 0, 5, 10, 50, 100
void ScreenCapture::screenSave(TCHAR path[], ULONG Quality, int width, int height)
{

	hBit = scrCapture();

	HPALETTE hpal; //팔레트 설정
	hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	 
	Bitmap *image = new Bitmap(hBit,hpal);			//GDI 의 HBITMAP 을 GDI+의 Bitmap 으로 넘겨줌	
	//Bitmap *pBit = alterImageSize(image, width, height);

	EncoderParameters Para;
	CLSID Clsid; // PNG, JPEG등 여러 포맷으로 저장	


	Para.Count=1;
	Para.Parameter[0].Guid=EncoderQuality;
	Para.Parameter[0].Type=EncoderParameterValueTypeLong;
	Para.Parameter[0].NumberOfValues=1;
	Para.Parameter[0].Value=&Quality;

	//getEncCLSID(L"image/jpeg", &Clsid);
	getEncCLSID(L"image/png", &Clsid);
	
	image->Save(path, &Clsid, &Para); 
	//pBit->Save(path, &Clsid, &Para);

	//오브젝트 해제
	DeleteObject(hpal);
	DeleteObject(hBit);

	//delete pBit;
	delete image;

}


//이미지 크기를 바꿔서 새로운 Bitmap 포인터를 리턴
Bitmap *ScreenCapture::alterImageSize(Bitmap *image, int width, int height)
{	
	
	Bitmap *pBit = new Bitmap((INT)width, (INT)height, image->GetPixelFormat());
	Graphics *memG = new Graphics(pBit);
	memG->SetSmoothingMode(SmoothingModeHighQuality);
	memG->DrawImage(image, 0, 0, width, height);
	
	delete memG;	
	
	return pBit;
}