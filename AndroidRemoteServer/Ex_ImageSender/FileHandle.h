/*

	이미지 파일을 다루는 클래스
*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>


class FileHandle{
private:
	FILE *srcFile, *desFile;
	char *fileBuf, *ptr;
	int size;

public:	
	FileHandle(){
		srcFile = NULL;
		desFile = NULL;
		fileBuf = NULL;
		ptr = NULL;
		size = 0;
	}

	~FileHandle(){
		if(srcFile == NULL || desFile == NULL)
			fileClose();	
	}



	bool fileOpen(const char *addr, const char *mode, int flags);	//파일 열기 함수
	bool fileCopy();												//파일 복사 함수
	bool fileClose();												//파일 닫기 함수
	bool writeFile();												//버퍼 내용을 파일에 복사하는 함수
	char *getFileBuf();												//파일 내용을 버퍼로 복사하는 함수
	int getFileSize();												//파일 크기 얻는 함수
	
};