#include <stdio.h>
#include "et/et.h"
#include "../headers/def_linux.h"
#include "../headers/defines.h"
#include "../headers/functions.h"

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
