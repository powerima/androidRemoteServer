/*
	�̹��� ������ �ٷ�� Ŭ����
*/



#include "FileHandle.h"


//���� ���� �Լ�
bool FileHandle::fileOpen(const char *addr, const char *mode, int flags)
{	
	FILE *f = fopen(addr, mode);
	if(f == NULL){
		printf("[Error] ���� ���� ����\n");
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
		printf("[Error] �߸��� �÷��� �μ�\n");
		fclose(f);
		break;
	}

	return true;
}


//���� ���� �Լ�
bool FileHandle::fileCopy()
{
	if(srcFile == NULL){
		printf("[Error] ���� ���� ����: srcFile\n");
		return false;
	}
	if(desFile == NULL){
		printf("[Error] ���� ���� ����: desFile\n");
		return false;
	}

	//���� ����
	while(!feof(srcFile))
		fputc(fgetc(srcFile), desFile);
	
	return true;
}
		   
//���� �ݱ� �Լ�
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
			printf("[Error] ���� �ݱ� ����: srcFile\n");
			return false;
		}
		srcFile = NULL;

	case 2:
		retval = fclose(desFile);
		if(retval != 0){
			printf("[Error] ���� �ݱ� ����: desFile\n");
			return false;
		}
		desFile = NULL;
		break;

	case 3:
		retval = fclose(srcFile);
		if(retval != 0){
			printf("[Error] ���� �ݱ� ����: srcFile\n");
			return false;
		}
		srcFile = NULL;
		break;
	}

	return true;
}

//���� ������ ���۷� �����ϴ� �Լ�
char *FileHandle::getFileBuf()
{
	if(srcFile == NULL){
		printf("[Error] ���� ���� ����: srcFile\n");
		return NULL;
	}
	
	//���� �޸� ���
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

//���� ������ ���Ͽ� �����ϴ� �Լ�
bool FileHandle::writeFile()
{
	if(fileBuf == NULL){
		printf("[Error] ���� �б� ����: fileBuf\n");
		return false;
	}

	if(desFile == NULL){
		printf("[Error] ���� �б� ����: desFile\n");
		return false;
	}

	ptr = fileBuf;
	for(int i=0; i<size; i++)
		fputc(*ptr++, desFile);
	return true;
}
		   
//���� ũ�� ��� �Լ�
int FileHandle::getFileSize()
{
	int byte = 0;
	if(srcFile == NULL){
		printf("[Error] ���� ���� ����: srcFile\n");
		return 0;
	}

	fseek(srcFile, 0, SEEK_SET);	

	while(!feof(srcFile)){
		fgetc(srcFile);
		byte++;
	}

	return byte;
}


