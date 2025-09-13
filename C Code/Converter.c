#include <stdio.h> 
#include <Windows.h>

#define BUFFERMULTIPLIER 1.5

void convertFile(char* FileToParse) {
	FILE* filePointer;

	filePointer = fopen(FileToParse, "r");

	char buffer[100] = { 0 };
	fgets(&buffer, 100, filePointer);

	printf("File contents: %s \n", &buffer);

	fclose(filePointer);

}


int reallocBuffer(char** Buffer, int* CurrentSize, int sizeOfAddition, int bytesInBuffer) {
	// Buffer will be realloced and resized to 1.5x approx, currentSize will be altered
	// Returns 1 for sucess, 0 for failure
	// TODO: Test

	printf("Running a realloc");	// Some placeholder
	*CurrentSize = *CurrentSize * BUFFERMULTIPLIER;
	char* newPointer = realloc(*Buffer, *CurrentSize);
	if (newPointer == NULL) {
		return 0;	// Error
	}
	
	int bytesNeeded = sizeOfAddition + bytesInBuffer;
	if (bytesNeeded >= *CurrentSize) {
		return reallocBuffer(*Buffer, CurrentSize, sizeOfAddition, bytesInBuffer);
	}

	// Otherwise its all good...
	*Buffer = newPointer;
	return 1;	// Sucess

}


int main() {
	WIN32_FIND_DATAA fileData;
	HANDLE fileHandle;
	fileHandle = FindFirstFile("*", &fileData);	// This will return . file

	if (fileHandle == INVALID_HANDLE_VALUE) {
		printf("Failed to open the first file, terminating program...");
		return -1;
	}

	boolean sucess;
	int filesFound = 0;
	int fileAttributes;
	char FilesToParse[256] = { 0 };
	char* BufferStart = &FilesToParse;
	//char* WriteIndex = &FilesToParse;
	int bytesWritten = 0;
	int bufferSize = 256;
	HANDLE nextHandle;

	sucess = FindNextFile(fileHandle, &fileData);	// This will return .. file, call just to have OS discard.


	while (sucess) {
		sucess = FindNextFile(fileHandle, &fileData);
		if (!sucess) {
			// Failed to fine a next file, the directory is typically all read
			break;
		}

		fileAttributes = fileData.dwFileAttributes;

		if ((fileAttributes | 16) != fileAttributes) {
			// Check if the file is a directory
			// Directories aren't files we want to parse

			int nameLen = strlen(fileData.cFileName);
			if (nameLen + bytesWritten >= bufferSize){
				reallocBuffer(&BufferStart, &bufferSize, nameLen, bytesWritten);
			}

			strcpy(BufferStart + bytesWritten, fileData.cFileName);
			bytesWritten += strlen(fileData.cFileName) + 1; // +1 for null termination
			filesFound += 1;
		}
	}
	int stringStartIndex = 0;
	for (int i = 0; i < filesFound; i++) {
		int fileNameLen = strlen(stringStartIndex + BufferStart);


		convertFile(BufferStart + stringStartIndex);
		char* fileName = malloc(fileNameLen);

		printf("%s \n", BufferStart + stringStartIndex);

		stringStartIndex += fileNameLen + 1;	// +1 to account for null terminated byte
	}
}