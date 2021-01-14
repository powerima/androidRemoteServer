
#include "ScreenCapture.h"


//ȭ�� ������ ��Ʈ�� ������ �ٲٴ� �Լ�
HBITMAP ScreenCapture::scrCapture()	
{
	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	HDC hScrDC, hMemDC;
	HBITMAP hBitmap;

	//������ ����̽��� ���ؽ�Ʈ�� �޾ƿ�. DISPLAY �μ��� ��ü ȭ�� DC�� �޾ƿ�
	hScrDC = CreateDC(L"DISPLAY",NULL,NULL,NULL);
	//hScrDC�� ���� ���� ������ �޸� DC�� ����
	
	hMemDC = CreateCompatibleDC(hScrDC);
	//��Ʈ�� �����ϴ� �Լ�.  ù��° hScrDC�� ȣȯ�Ǵ� ��Ʈ�� ����
	hBitmap = CreateCompatibleBitmap(hScrDC, ScreenWidth, ScreenHeight);
	//hBitmap = makeDIBSection(hScrDC, ScreenWidth, ScreenHeight);
	

	SelectObject(hMemDC, hBitmap);

	//��Ʈ���� �����ϴ� �Լ� hMemDC�μ��� hScrDC ��Ʈ���� ����
	BitBlt(hMemDC,0,0,ScreenWidth, ScreenHeight, hScrDC, 0,0,SRCCOPY);	
	//BitBlt(hMemDC,10,30,600, 400, hScrDC, 0,0,SRCCOPY);	

	DeleteDC(hMemDC);
	DeleteDC(hScrDC);
	//InvalidateRect(hWnd, NULL, TRUE);

	return hBitmap;
}

//ȭ�� ������ ��Ʈ�� ������ �ٲٴ� �Լ�
HBITMAP ScreenCapture::scrCapture(int width, int height)	
{
	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	HDC hScrDC;
	HBITMAP hBitmap;

	//������ ����̽��� ���ؽ�Ʈ�� �޾ƿ�. DISPLAY �μ��� ��ü ȭ�� DC�� �޾ƿ�
	hScrDC = CreateDC(L"DISPLAY",NULL,NULL,NULL);

	//���ڷ� ���޵� DC�� ȣȯ�Ǵ� DDB����	
	hBitmap = makeDIBSection(hScrDC, ScreenWidth, ScreenHeight);		

	DeleteDC(hScrDC);
	
	return hBitmap;
}


//��Ʈ���� ȭ�� DC�� ����ϴ� �Լ�
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
		//��Ʈ�� ũ�⸦ ��� �Ǵ� Ȯ���ؼ� DC�� �����ϴ� �Լ�	
		StretchBlt(hdc, 0, 0, ScreenWidth, ScreenHeight, hMemDC, 0, 0, bx, by, SRCCOPY);
		TextOut(hdc, 10, 10, str, _tcslen(str));
		
		break;

	case 1:
		//��Ʈ�� ũ�⸦ ���� �״�� DC�� �����ϴ� �Լ�
		BitBlt(hdc, 0, 0, ScreenWidth, ScreenHeight, hMemDC, 0, 0, SRCCOPY);	
		break;

	}

	SelectObject(hdc, hOldBitmap);
	DeleteDC(hMemDC);

}

//���ڷ� ���޵� DC�� ȣȯ�Ǵ� DDB����
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

//GDI+ ���̺귯�� �ʱ�ȭ
bool ScreenCapture::initGdi()
{
	if (GdiplusStartup(&gpToken,&gpsi,NULL) != Ok) {
		MessageBox(NULL,TEXT("GDI+ ���̺귯���� �ʱ�ȭ�� �� �����ϴ�."),
		TEXT("�˸�"),MB_OK);
		return false;
	}
	return true;
}

//�̹��� ���� ��ȯ�� ���� CLSID ���
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


//��ũ�� ����- ����Ƽ �ɼ�: 0, 5, 10, 50, 100
void ScreenCapture::screenSave(HWND hWnd, ULONG Quality)
{

	HBITMAP hBit = scrCapture();

	HPALETTE hpal; //�ȷ�Ʈ ����
	hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	 
	Bitmap image(hBit,hpal); //GDI �� HBITMAP �� GDI+�� Bitmap ���� �Ѱ���
	int width = image.GetWidth();
	int height = image.GetHeight();
	EncoderParameters Para;
	CLSID jpegClsid; // JPEG �������� ����	


	Para.Count=1;
	Para.Parameter[0].Guid=EncoderQuality;
	Para.Parameter[0].Type=EncoderParameterValueTypeLong;
	Para.Parameter[0].NumberOfValues=1;
	Para.Parameter[0].Value=&Quality;

	getEncCLSID(L"image/jpeg", &jpegClsid);

	image.Save(L"d://test.jpg", &jpegClsid, &Para);

	//������Ʈ ����
	DeleteObject(hpal);
	DeleteObject(hBit);

	//delete *image;

}

//��ũ�� ����- ����Ƽ �ɼ�: 0, 5, 10, 50, 100
void ScreenCapture::screenSave(TCHAR path[], ULONG Quality)
{

	HBITMAP hBit = scrCapture();

	HPALETTE hpal; //�ȷ�Ʈ ����
	hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	
	
	Bitmap *image= new Bitmap(hBit,hpal); //GDI �� HBITMAP �� GDI+�� Bitmap ���� �Ѱ���
	//int width = image->GetWidth();
	//int height = image->GetHeight();
	EncoderParameters Para;
	CLSID jpegClsid; // JPEG �������� ����	


	Para.Count=1;
	Para.Parameter[0].Guid=EncoderQuality;
	Para.Parameter[0].Type=EncoderParameterValueTypeLong;
	Para.Parameter[0].NumberOfValues=1;
	Para.Parameter[0].Value=&Quality;

	getEncCLSID(L"image/png", &jpegClsid);

	image->Save(path, &jpegClsid, &Para);

	 
	//������Ʈ ����
	DeleteObject(hpal);
	DeleteObject(hBit);

	//delete image;

}

//��ũ�� ����- ����Ƽ �ɼ�: 0, 5, 10, 50, 100
void ScreenCapture::screenSave(TCHAR path[], ULONG Quality, int width, int height)
{

	hBit = scrCapture();

	HPALETTE hpal; //�ȷ�Ʈ ����
	hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	 
	Bitmap *image = new Bitmap(hBit,hpal);			//GDI �� HBITMAP �� GDI+�� Bitmap ���� �Ѱ���	
	//Bitmap *pBit = alterImageSize(image, width, height);

	EncoderParameters Para;
	CLSID Clsid; // PNG, JPEG�� ���� �������� ����	


	Para.Count=1;
	Para.Parameter[0].Guid=EncoderQuality;
	Para.Parameter[0].Type=EncoderParameterValueTypeLong;
	Para.Parameter[0].NumberOfValues=1;
	Para.Parameter[0].Value=&Quality;

	//getEncCLSID(L"image/jpeg", &Clsid);
	getEncCLSID(L"image/png", &Clsid);
	
	image->Save(path, &Clsid, &Para); 
	//pBit->Save(path, &Clsid, &Para);

	//������Ʈ ����
	DeleteObject(hpal);
	DeleteObject(hBit);

	//delete pBit;
	delete image;

}


//�̹��� ũ�⸦ �ٲ㼭 ���ο� Bitmap �����͸� ����
Bitmap *ScreenCapture::alterImageSize(Bitmap *image, int width, int height)
{	
	
	Bitmap *pBit = new Bitmap((INT)width, (INT)height, image->GetPixelFormat());
	Graphics *memG = new Graphics(pBit);
	memG->SetSmoothingMode(SmoothingModeHighQuality);
	memG->DrawImage(image, 0, 0, width, height);
	
	delete memG;	
	
	return pBit;
}