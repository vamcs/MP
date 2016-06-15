#include <stdio.h>
#include "../include/Digraph.h"
#include "../include/manager.h"

int main(){

	Digraph G = DIGRAPHinit("./supporting_files/input.txt");

	DIGRAPHshow(G);

	modify(G);

	DIGRAPHshow(G);
	DIGRAPHsave(G);
	DIGRAPHdestroy(G);
}