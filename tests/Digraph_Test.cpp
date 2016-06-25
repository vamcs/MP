#include "gtest/gtest.h"
#include "../include/Digraph.h"

using namespace std;

TEST(DigraphInit_Test, Initialize_Digraph){
	Digraph digraph = DIGRAPHinit();
	EXPECT_TRUE(digraph);
	DIGRAPHdestroy(digraph);
}