/*

	�̹��� ������ �ٷ�� Ŭ����
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



	bool fileOpen(const char *addr, const char *mode, int flags);	//���� ���� �Լ�
	bool fileCopy();												//���� ���� �Լ�
	bool fileClose();												//���� �ݱ� �Լ�
	bool writeFile();												//���� ������ ���Ͽ� �����ϴ� �Լ�
	char *getFileBuf();												//���� ������ ���۷� �����ϴ� �Լ�
	int getFileSize();												//���� ũ�� ��� �Լ�
	
};