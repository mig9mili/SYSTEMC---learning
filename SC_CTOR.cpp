#include <systemc>
using namespace std;
using namespace sc_core;

SC_MODULE(module){
   SC_CTOR(module){
     SC_METHOD(func_a);
   }

   void func_a(){
    cout<<name()<<endl;
   }
};

SC_MODULE(module1){
    SC_CTOR(module1){
        SC_METHOD(func_b);
    }

     void func_b(); /// call
};


void module1::func_b(){     ///func defination outside class defination
    cout<< this->name()<<endl;
}


SC_MODULE(module2){
    const int i;
    SC_CTOR(module2);
    module2(sc_module_name name, int i):sc_module(name),i(i){  // explcit constructor  // passing one more parameter
        SC_METHOD(func_c);
    }

    void func_c(){

        cout<<name()<< ", i= "<< i << endl;
    }
};


int sc_main(int , char*[]){

    module moda("moda");
    module1 mod1("mod1");
    module2 mod2("mod2",1);

  sc_start();
  return 0;
}

