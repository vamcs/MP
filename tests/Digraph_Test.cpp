#include "gtest/gtest.h"
#include "../include/Digraph.h"

using namespace std;

TEST(DigraphInit_Test, Initialize_Digraph){
	Digraph digraph = DIGRAPHinit();
	EXPECT_NE(digraph, nullptr);
    EXPECT_EQ(digraph->V, 0);
    EXPECT_EQ(digraph->E, 0);
    EXPECT_NE(digraph->array, nullptr);
	DIGRAPHdestroy(digraph);
}

TEST(DigraphDestroy_Test, Deinitialize_Digraph){
    Digraph digraph = DIGRAPHinit();
    DIGRAPHdestroy(digraph);
    EXPECT_EQ(digraph, nullptr);
}

TEST(InsertVertex_Test, InsertVertex_NoDep_Success){
    Digraph digraph = DIGRAPHinit();
    bool (*nameCheck)(Digraph, char*, int);
	nameCheck = NAMEcheck;

	bool (*inputCheck)(int);
	inputCheck = INPUTcheck;

    VertexArray w = (VertexArray)malloc(sizeof(struct vertexArray));
    w->id = 0;
    strcpy(w->name,"TR 0");
    w->exec = 1;
    w->duration = 1;
    w->min_start = 0;
    w->reqs = 0;
    w->reqs_id = NULL;
	int ret = DIGRAPHinsertV(digraph, w, inputCheck,nameCheck);
	EXPECT_EQ(ret, 0);
    EXPECT_EQ(digraph->array[0]->id,0);
    EXPECT_STREQ(digraph->array[0]->name,"TR 0");
    EXPECT_EQ(digraph->array[0]->exec,1);
    EXPECT_EQ(digraph->array[0]->duration,1);
    EXPECT_EQ(digraph->array[0]->min_start,0);
    EXPECT_EQ(digraph->array[0]->reqs,0);
    DIGRAPHdestroy(digraph);
}

TEST(InsertVertex_Test, InsertVertex_Duration_Error){
    Digraph digraph = DIGRAPHinit();
    bool (*nameCheck)(Digraph, char*, int);
	nameCheck = NAMEcheck;

	bool (*inputCheck)(int);
	inputCheck = INPUTcheck;

    VertexArray w = (VertexArray)malloc(sizeof(struct vertexArray));
    w->id = 0;
    strcpy(w->name,"TR 0");
    w->exec = 1;
    w->duration = 0;
    w->min_start = 0;
    w->reqs = 0;

	int ret = DIGRAPHinsertV(digraph, w, inputCheck,nameCheck);
	EXPECT_EQ(ret, DigraphInvalidVertexDurationError);
    DIGRAPHdestroy(digraph);
}

TEST(InsertVertex_Test, InsertVertex_MinStart_Error){
    Digraph digraph = DIGRAPHinit();
    bool (*nameCheck)(Digraph, char*, int);
	nameCheck = NAMEcheck;

	bool (*inputCheck)(int);
	inputCheck = INPUTcheck;

    VertexArray w = (VertexArray)malloc(sizeof(struct vertexArray));
    w->id = -1;
    strcpy(w->name,"TR 0");
    w->exec = 1;
    w->duration = 1;
    w->min_start = -1;
    w->reqs = 0;
    w->reqs_id = NULL;

	int ret = DIGRAPHinsertV(digraph, w, inputCheck,nameCheck);
	EXPECT_EQ(ret, DigraphInvalidVertexMinStartError);
    DIGRAPHdestroy(digraph);
}

TEST(InsertVertex_Test, InsertVertex_Req_Error){
    Digraph digraph = DIGRAPHinit();
    bool (*nameCheck)(Digraph, char*, int);
    nameCheck = NAMEcheck;

    bool (*inputCheck)(int);
    inputCheck = INPUTcheck;

    VertexArray w = (VertexArray)malloc(sizeof(struct vertexArray));
    w->id = -1;
    strcpy(w->name,"TR 0");
    w->exec = 1;
    w->duration = 1;
    w->min_start = 0;
    w->reqs = 1;
    w->reqs_id = NULL;
    w->reqs_id = (int*)malloc(1*sizeof(int));
    w->reqs_id[0] = 1;

    int ret = DIGRAPHinsertV(digraph, w, inputCheck,nameCheck);
    EXPECT_EQ(ret, DigraphInvalidVertexReqsError);
    DIGRAPHdestroy(digraph);
}