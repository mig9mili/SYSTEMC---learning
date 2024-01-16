#include<iostream>
#include<systemc>

using namespace std;
using namespace sc_core;

//1.
/// module without simulatiom

SC_MODULE(MODULE1){
    MODULE1(sc_module_name name){
   cout<<this->name()<<"no SC_CTOR and SC_HAS_PROCESS"<<endl;
    }
};

//2.
//only one inp
// implicitly declare of MODULE_2(sc_module_name)

SC_MODULE(MODULE2){
  SC_CTOR(MODULE2){
    SC_METHOD(func_2);
  }
  void func_2(){
    cout<< name()<<"SC_CTOR"<<endl;
  }
};


//3.
// only one inp
//no implicit constructor
// explicit declaration

SC_MODULE(MODULE3){
    SC_HAS_PROCESS(MODULE3);
    MODULE3(sc_module_name name){
    SC_METHOD(func_3);
    }
    void func_3(){

        cout<<name()<<"SC_HAS_PROCESS"<<endl;
    }

};


// use SC_CTOR  no additional agrument 
// use SC_HAS_PROCESS when additional  parameters needed

//4.pass additional parameter

SC_MODULE(MODULE4){
    const int i;
  SC_HAS_PROCESS(MODULE4);

  MODULE4(sc_module_name name,int i):i(i){

    SC_METHOD(func_4);
  }

  void func_4(){
    cout<<name()<<" "<<"additinal parameter"<<" "<< i <<endl;

  }


};


//5.SC_CTOR inside header and  constructor  defined outside 


SC_MODULE(MODULE5){
 SC_CTOR(MODULE5);
  void func_5(){
    cout<<this->name()<< " SC_CTOR inside header,constructor outside"<<endl;
  }
};

MODULE5::MODULE5(sc_module_name name): sc_module(name){
    SC_METHOD(func_5);
}


//6.SC_HAS_PROCESS inside header and  constructor  defined outside 



SC_MODULE(MODULE6){
 SC_HAS_PROCESS(MODULE6);
 MODULE6(sc_module_name);
 void func_6(){
    cout<<this->name()<<"SC_HAS_PROCESS inside header  and constructor defined outside"<<endl;
 }
 };

 MODULE6::MODULE6(sc_module_name name): sc_module(name){
  SC_METHOD(func_6);
 }



//7.constructor define outside  but not ok with SC_CTOR
SC_MODULE(MODULE7){
  MODULE7(sc_module_name name);
  void func_7(){
    cout<<this->name()<<"SC_HAS_PROCESS outside header,CANNOT use SC_CTOR "<<endl;
  }
  };

  MODULE7::MODULE7(sc_module_name name){
    SC_HAS_PROCESS(MODULE7);
    SC_METHOD(func_7);
  }


int sc_main(int, char*[]){
MODULE1 MOD1("MOD1");
MODULE2 MOD2("MOD2");
MODULE3 MOD3("MOD3");
MODULE4 MOD4("MOD4",1);
MODULE5 MOD5("MOD5");
MODULE6 MOD6("MOD6");
MODULE7 MOD7("MOD7");

sc_start();
return 0;
}
    

