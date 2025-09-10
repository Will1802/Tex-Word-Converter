#include <stdio.h> 
#include <Windows.h>

int main(){
	WIN32_FIND_DATAA fileData;
	HANDLE fileHandle;
	HANDLE nextHandle;
	fileHandle = FindFirstFile("*", &fileData);	// This will return . file

	if (fileHandle == INVALID_HANDLE_VALUE) {
		printf("Failed to open the first file, terminating program...");
		return -1;
	}
	boolean sucess;

	sucess = FindNextFile(fileHandle, &fileData);	// This will return .. file, call just to have OS discard.
	int filesFound = 0;
	int fileAttributes;
	while(sucess){
		sucess = FindNextFile(fileHandle, &fileData);
		fileAttributes = fileData.dwFileAttributes;
		if (sucess && ((fileAttributes | 16) != fileAttributes)) {
			// ((fileAttributes | 16) == fileAttributes) is to check if the file is a directory
			// Directories aren't files we want to parse


			// Temp placeholder code to check that it works as expected
			printf("%d\n", fileAttributes);
			printf("%s\n", fileData.cFileName);

			filesFound += 1;
		}
	}
	// Testing basic funtionality
	printf("%d", filesFound);
}