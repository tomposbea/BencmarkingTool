#include <stdio.h>
#include "et/et.h"
#include "../headers/def_linux.h"
#include "../headers/defines.h"
#include "../headers/process/hash_map.h"
#include "../headers/process/binary_search_tree.h"
#include "../headers/process/redblacktree.h"
#include "../headers/process/lcm.h"
#include "../headers/generate/generate.h"
#include "../headers/generate/conversions.h"

#define CONFIG_SIZE (256)
#define SPEED_SET (1)
#define n_SET (2)
#define m_SET (3)

ET_TEST(int_foo){
	et_assert(1 == 1);
	et_asserteq(1,1);
	et_assertne(1,0);
}
ET_TEST(lcm_gcd){
	et_asserteq(60,LCM(10,12));
	et_asserteq(2,GCD(10,12));
}
ET_TEST(int_generate){
	et_assert(generate_int(1,99) <= 99);
	et_assert(generate_int(1,99) >= 1);
}
ET_TEST(find_duplicate){
	char* instring;
	instring = "1 1 1 1";
	int TablePtr = 0;
	strncpy_s(Table[TablePtr], STRINGLEN, instring, STRINGLEN);

	et_assert(0 == FindDuplicate(instring));
}
ET_TEST(check_parameters){
	//["Run","runtime","2","log","2"]
	char* v[4];
	v[0] = "Run";
	v[1] = "runtime";
	v[2] = "2";
	v[3] = "log";
	v[4] = "2";
	char** p = v;

	et_asserteq(1,CheckParameter(1,5,p,"runtime"));
}
ET_TEST(get_parameters){
	char* v[4];
	v[0] = "Run";
        v[1] = "runtime";
        v[2] = "2";
        v[3] = "log";
        v[4] = "2";
	char** p = v;

	et_asserteq(1,GetParameters(1,p));
}
ET_TEST(read_configuration){
	typedef struct config {
        unsigned set;
        int n;
        int m;
        int speed;
	} CONFIG;

	char buf[CONFIG_SIZE] = "n = 1";
	CONFIG config[1];

	config->set = 0u;

	et_asserteq(0,parse_config(buf, config));

}
ET_TEST(hash_code){
	long int a = 6600000;

	et_asserteq(0,hashCode(a));
}
ET_TEST(make_key_test){

	char v[10]= "12 31 42";
	long int a = 123142;
	et_asserteq(a,make_key(v));
}
ET_TEST(write_to_fifo){
	int fifonum = 1;
	char Temp[10] = "123456";
	int DroppedCtrl = 0;

	et_asserteq(0,WriteToFifo(fifonum,Temp,DroppedCtrl));
}
ET_TEST(generate_and_write_to_fifo){
	int fifonum = 1;

	et_asserteq(0,GenerateAndWriteToFIFO(fifonum));
}
//ET_TEST(process_string_and_calcualte){
//	int fifonum = 1;
//	char v[10] = "10 01 20";
	//insert_to_hash(make_key(v), v);
//	struct bstree {
//  		char data[STRINGLEN];
//  		struct bstree *left, *right;
//	};
//	struct bstree *bsroot = NULL;
//	bsroot = insert_bs(bsroot, v);

//	fifonum = 2;
//	et_asserteq(0,ProcessStringAndCalculate(v,fifonum));
//	bsroot = delete_bs(bsroot, v);
//}
//ET_TEST(get_from_fifo_and_process){
//	int fifonum = 1;
//	GenerateAndWriteToFIFO(fifonum);
//	char v[10] = "12 31 42 5";
//	int DroppedCtrl=0;
//	insert_to_hash(make_key(v), v);
//	WriteToFifo(fifonum, v, DroppedCtrl);


//	et_asserteq(1,GetFromFIFOAndProcess(fifonum));
//}
//ET_TEST(new_node){
//	struct bstree {
//	char data[STRINGLEN];
//	struct bstree *left, *right;
//	};
//	struct bstree *bsroot = NULL;
//
//	char v[10] = "101";
//	struct bstree *temp = (struct bstree *)malloc(sizeof(struct bstree));
//	strcpy(temp->data, v);
//	temp->left = temp->right = NULL;

	//struct bstree *result = newNode(v);
//	bsroot = newNode(v);

//	et_assert(*temp->data==bsroot->data);
//}
ET_TEST(search_bs_false){
	 struct bstree {
      char data[STRINGLEN];
      struct bstree *left, *right;
      };
      struct bstree *bsroot = NULL;

      char v[10] = "101";

	et_asserteq(false,search_bs(bsroot,v));
}
//ET_TEST(search_bs_true){
//	struct bstree {
//      char data[STRINGLEN];
//      struct bstree *left, *right;
//      };
//      struct bstree *bsroot;bsroot = NULL;

//      char* v = "100";
//	char* w = "200";
//	bsroot=insert_bs(bsroot,v);
//	bsroot=insert_bs(bsroot,w);
//        et_asserteq(true,search_bs(bsroot,v));

//}



















