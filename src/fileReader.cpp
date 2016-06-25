#include "../include/fileReader.h"

char* readFileLine(FILE* fp){

    if (fp == NULL) {
        return NULL;
    }

	char* str = (char*)malloc(1000*sizeof(char));
	fscanf (fp, "%1000[^\n]", str);
	fgetc(fp);
	return str;
}

void saveOnFile(FILE* fp, char* str) {
	fprintf (fp, "%s\n", str);
}