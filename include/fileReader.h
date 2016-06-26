#ifndef FILEREADER_H
#define FILEREADER_H
#endif

#include <stdlib.h>
#include <stdio.h>

enum fileReaderErrors{
	FileReaderCannotOpenFileError = -1
};

/*!	\Brief Lê uma linha do arquivo passado como parâmetro de entrada.
*
*	Detailed A função lê uma linha do arquivo e entrada e retorna um ponteiro para um array de caracteres.
*
*	Funções clientes: main()
*
*	Assertivas de Entrada:
*	FILE fp != NULL - É o ponteiro para o arquivo de entrada.
*	
*	Assertivas de Saída:
*	fp != NULL:	(arquivo aberto com sucesso e diferente de feof)
*
*	Como não há input humano e as funções são confiáveis, as entradas e saída já estão verificadas.
*
*	\param fp Ponteiro para o arquivo de entrada.
*	\return Um ponteiro para um array de caracteres se a leitura do arquivo for realizada com sucesso.
*			Caso contrário, NULL é retornado.
*/
char* readFileLine(FILE* fp);

/*!	\Brief Salva uma string num arquivo.
*
*	Detailed A função escreve no arquivo de saída a string passada como parâmetro.
*
*
*	Assertivas de Entrada:
*	FILE fp != NULL - É o ponteiro para o arquivo de entrada.
*	char* str != NULL - É a string a ser inserida no arquivo.
*
*
*	Como não há input humano e as funções são confiáveis, as entradas e saída já estão verificadas.
*
*	\param fp Ponteiro para o arquivo de entrada.
*	\param str String a ser inserida no arquivo.
*	\return
*/
void saveOnFile(FILE* fp, char* str);