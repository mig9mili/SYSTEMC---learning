#include<systemc>
using namespace sc_core;
using namespace std; 

class HelloWorld : sc_module{  //class inherits  sc_module base  and /// define a systemc module

    
    public:
    char helloworld;
    
        SC_CTOR(HelloWorld){   // constructor
        SC_METHOD(myname);  // register the  member func to kernel
    }
   void myname(void){

    cout<<"my name is yashas"<<endl;

   }
};


int sc_main(int, char*[]){

    HelloWorld name1("helloworld"); // define the   class and it obj
    sc_start();  // kernel to inoke. name1.
    return 0;
}