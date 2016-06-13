#include <stdio.h>
#include "Digraph.h"
#include "manager.h"

int main(){
	Digraph G = DIGRAPHinit("instructions.txt");
	DIGRAPHshow(G);
	DIGRAPHdestroy(G);
}