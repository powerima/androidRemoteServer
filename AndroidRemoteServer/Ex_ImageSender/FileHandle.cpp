/*
	이미지 파일을 다루는 클래스
*/



#include "FileHandle.h"


//파일 열기 함수
bool FileHandle::fileOpen(const char *addr, const char *mode, int flags)
{	
	FILE *f = fopen(addr, mode);
	if(f == NULL){
		printf("[Error] 파일 열기 실패\n");
		return false;
	}
	switch(flags){
	case 1:
		srcFile = f;
		break;
	case 2:
		desFile = f;
		break;
	default:
		printf("[Error] 잘못된 플래그 인수\n");
		fclose(f);
		break;
	}

	return true;
}


//파일 복사 함수
bool FileHandle::fileCopy()
{
	if(srcFile == NULL){
		printf("[Error] 파일 열기 실패: srcFile\n");
		return false;
	}
	if(desFile == NULL){
		printf("[Error] 파일 열기 실패: desFile\n");
		return false;
	}

	//파일 복사
	while(!feof(srcFile))
		fputc(fgetc(srcFile), desFile);
	
	return true;
}
		   
//파일 닫기 함수
bool FileHandle::fileClose()
{
	int flags, retval;
	if(srcFile == NULL && desFile == NULL)
		return true;
	else if(srcFile != NULL)
		flags = 3;
	else if(desFile != NULL)
		flags = 2;
	else
		flags = 1;

	switch(flags){
	case 1:		
		retval = fclose(srcFile);
		if(retval != 0){
			printf("[Error] 파일 닫기 실패: srcFile\n");
			return false;
		}
		srcFile = NULL;

	case 2:
		retval = fclose(desFile);
		if(retval != 0){
			printf("[Error] 파일 닫기 실패: desFile\n");
			return false;
		}
		desFile = NULL;
		break;

	case 3:
		retval = fclose(srcFile);
		if(retval != 0){
			printf("[Error] 파일 닫기 실패: srcFile\n");
			return false;
		}
		srcFile = NULL;
		break;
	}

	return true;
}

//파일 내용을 버퍼로 복사하는 함수
char *FileHandle::getFileBuf()
{
	if(srcFile == NULL){
		printf("[Error] 파일 열기 실패: srcFile\n");
		return NULL;
	}
	
	//버퍼 메모리 얻기
	size = getFileSize();
	fileBuf = (char *)malloc(size);
	ptr = fileBuf;
	
	fseek(srcFile, 0, SEEK_SET);

	int i=0;
	while(!feof(srcFile)){		
		*ptr++ = fgetc(srcFile);		
	}
	ptr=fileBuf;

	return fileBuf;
}

//버퍼 내용을 파일에 복사하는 함수
bool FileHandle::writeFile()
{
	if(fileBuf == NULL){
		printf("[Error] 버퍼 읽기 실패: fileBuf\n");
		return false;
	}

	if(desFile == NULL){
		printf("[Error] 파일 읽기 실패: desFile\n");
		return false;
	}

	ptr = fileBuf;
	for(int i=0; i<size; i++)
		fputc(*ptr++, desFile);
	return true;
}
		   
//파일 크기 얻는 함수
int FileHandle::getFileSize()
{
	int byte = 0;
	if(srcFile == NULL){
		printf("[Error] 파일 열기 실패: srcFile\n");
		return 0;
	}

	fseek(srcFile, 0, SEEK_SET);	

	while(!feof(srcFile)){
		fgetc(srcFile);
		byte++;
	}

	return byte;
}


