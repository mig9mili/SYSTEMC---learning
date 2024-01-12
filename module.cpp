#include <systemc>
using namespace std;
using namespace sc_core;


SC_MODULE(Module){

SC_CTOR(Module){

  cout<<"i am module "<<endl;

}

};


struct Module1 : public sc_module{

  //by default public
  SC_CTOR(Module1){
    cout<<"i am module 1"<<endl;
  }

};

class Module2 : public sc_module{
  // by default private
  public:
  SC_CTOR(Module2){

    cout<<"i am module 2"<<endl;

  }

};

int sc_main(int, char*[]){


  Module  mod("mod");
  Module1 mod1("mod1");
  Module2 mod2("mod2");
  
  sc_start();
  return 0;
 
}