#include<systemc>
#include"TOP.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;


int sc_main(int argc, char* argv[]){

   TOP top("top");
   sc_start();
    return 0;
}