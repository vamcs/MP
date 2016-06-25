#include <stdlib.h>
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

TEST(InsertVertex_Test, InsertVertex_Dep_Success){
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

    VertexArray u = (VertexArray)malloc(sizeof(struct vertexArray));
    u->id = 1;
    strcpy(u->name,"TR 1");
    u->exec = 1;
    u->duration = 1;
    u->min_start = 1;
    u->reqs = 1;
    u->reqs_id = NULL;
    u->reqs_id = (int*)malloc(u->reqs*sizeof(int));
    u->reqs_id[0] = 0;

    ret = DIGRAPHinsertV(digraph, u, inputCheck,nameCheck);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(digraph->array[1]->id,1);
    EXPECT_STREQ(digraph->array[1]->name,"TR 1");
    EXPECT_EQ(digraph->array[1]->exec,1);
    EXPECT_EQ(digraph->array[1]->duration,1);
    EXPECT_EQ(digraph->array[1]->min_start,1);
    EXPECT_EQ(digraph->array[1]->reqs,1);
    EXPECT_EQ(digraph->array[1]->reqs_id[0],0);

    DIGRAPHdestroy(digraph);
}

TEST(RemoveVertex_Test, RemoveVertex_Success){
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

    ret = DIGRAPHremoveV(digraph, 0);
    EXPECT_EQ(ret, 0);
    DIGRAPHdestroy(digraph);
}

TEST(RemoveVertex_Test, RemoveVertex_Error){
    Digraph digraph = DIGRAPHinit();
    int ret = DIGRAPHremoveV(digraph, 0);
    EXPECT_EQ(ret, DigraphInvalidEdgeIDError);
    DIGRAPHdestroy(digraph);
}

TEST(VertexReturn_Test, VertexReturn_Error){
    Digraph digraph = DIGRAPHinit();
    int ret = VERTEXreturn(digraph, 0);
    EXPECT_EQ(ret, -1);
    DIGRAPHdestroy(digraph);
}

TEST(VertexReturn_Test, VertexReturn_Success){
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

    ret = VERTEXreturn(digraph, 0);
    EXPECT_EQ(ret, 0);
    DIGRAPHdestroy(digraph);
}

TEST(EdgeInstantiation_Test, EdgeInstantiation_Success){
    Edge e = EDGE(0,1,0);
    EXPECT_EQ(e.v, 0);
    EXPECT_EQ(e.w,1);
    EXPECT_EQ(e.id, 0);
}

TEST(EdgeInstantiation_Test, EdgeInstantiation_Error){
    Edge e = EDGE(0,-1,0);
    EXPECT_EQ(e.v, -1);
    EXPECT_EQ(e.w,-1);
    EXPECT_EQ(e.id, -1);
}

TEST(VertexInstantiation_Test, VertexInstantiation_WithString_Success){\
    char* str = (char*)malloc(20*sizeof(char));
    strcpy(str,"100 'TR 0' 0 2 0 0");
    VertexArray v = cnvInputStrToVertex(str);
    EXPECT_EQ(v->id,100);
    EXPECT_STREQ(v->name,"TR 0");
    EXPECT_EQ(v->exec,0);
    EXPECT_EQ(v->duration,2);
    EXPECT_EQ(v->min_start,0);
    EXPECT_EQ(v->reqs,0);
    free(str);
}

TEST(VertexInstantiation_Test, VertexInstantiation_WithString_Error){
    char* str = (char*)malloc(20*sizeof(char));
    strcpy(str,"100");
    VertexArray v = cnvInputStrToVertex(str);
    EXPECT_EQ(v,nullptr);
    free(str);
}

TEST(VertexInstantiation_Test, VertexInstantiation_WithString_Error2){
    char* str = (char*)malloc(20*sizeof(char));
    strcpy(str,"100 'TR 0'");
    VertexArray v = cnvInputStrToVertex(str);
    EXPECT_EQ(v,nullptr);
    free(str);
}

TEST(VertexInstantiation_Test, VertexInstantiation_WithString_Error3){
    char* str = (char*)malloc(20*sizeof(char));
    strcpy(str,"100 'TR 0' 0");
    VertexArray v = cnvInputStrToVertex(str);
    EXPECT_EQ(v,nullptr);
    free(str);
}

TEST(VertexInstantiation_Test, VertexInstantiation_WithString_Error4){
    char* str = (char*)malloc(20*sizeof(char));
    strcpy(str,"100 'TR 0' 0 2");
    VertexArray v = cnvInputStrToVertex(str);
    EXPECT_EQ(v,nullptr);
    free(str);
}

TEST(VertexInstantiation_Test, VertexInstantiation_WithString_Error5){
    char* str = (char*)malloc(20*sizeof(char));
    strcpy(str,"100 'TR 0' 0 2 0");
    VertexArray v = cnvInputStrToVertex(str);
    EXPECT_EQ(v,nullptr);
    free(str);
}