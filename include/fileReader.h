#ifndef FILEREADER_H
#define FILEREADER_H
#endif

#include <stdlib.h>
#include <stdio.h>

enum fileReaderErrors{
	FileReaderCannotOpenFileError = -1
};

int numberOfLinesOnFile(FILE* fp);
char* readFileLine(FILE* fp);
void saveOnFile(FILE* fp, char* str);