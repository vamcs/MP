#include "gtest/gtest.h"
#include "../include/Digraph.h"

using namespace std;

TEST(DigraphInit_Test, Initialize_Digraph){
	char filePath[] = "./supporting_files/input.txt";
	Digraph digraph = DIGRAPHinit(filePath);
	EXPECT_TRUE(digraph);
	DIGRAPHdestroy(digraph);
}