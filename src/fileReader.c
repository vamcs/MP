#include "../include/fileReader.h"

int numberOfLinesOnFile(FILE* fp) {
	int count = 0;
	char aux;
	if(fp == NULL){
		return FileReaderCannotOpenFileError;
	}

	while(!feof(fp)) 
	{
		aux = fgetc(fp);
		if (aux == '\n') 
		{
			count++;
		}
	}

	return count;
}
char* readFileLine(FILE* fp){
	char* str = (char*)malloc(1000*sizeof(char));
	fscanf (fp, "%[^\n]", str);
	fgetc(fp);
	return str;
}

void saveOnFile(FILE* fp, char* str) {
	fprintf (fp, "%s\n", str);
}