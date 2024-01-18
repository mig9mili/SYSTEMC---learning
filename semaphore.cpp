#include<systemc>
#include<iostream>

using namespace std;
using namespace sc_core;


SC_MODULE(SEMAPHORE){
    sc_semaphore s; // declare semaphore
    SC_CTOR(SEMAPHORE) : s(2){  //init semaphore with 2 resources
      SC_THREAD(thread1);
      SC_THREAD(thread2);
      SC_THREAD(thread3);
    }

    void thread1(){
        while(true){
            if(s.trywait()== -1){ // if  sema > 0   -->  dec the sema  value  and return 0
                                  // if sems ==0  -->  return  -1  without change sema value

                s.wait();    //wait till resource is available
            }
         
         // int get_value(): shall return the semaphore value
        cout<< sc_time_stamp() << ": locked by thread 1, value is " << s.get_value() << endl;
        wait(1,SC_SEC);
        s.post();
        //a) shall increment the semaphore value.
        //b) shall use immediate notification to signal the act of incrementing the semaphore value to any waiting processes.
        // c) shall unconditionally return the value 0.
        cout<< sc_time_stamp() << ": unlocked by thread_1, value is " << s.get_value() << endl;
        wait(SC_ZERO_TIME);
        }
    }

    void thread2(){
        while(true){
            if(s.trywait()== -1){ // if  sema > 0   -->  dec the sema  value  and return 0
                                  // if sems ==0  -->  return  -1  without change sema value

                s.wait();    //wait till resource is available
            }
         
         // int get_value(): shall return the semaphore value
        cout<< sc_time_stamp() << ": locked by thread 2, value is " << s.get_value() << endl;
        wait(1,SC_SEC);
        s.post();
        //a) shall increment the semaphore value.
        //b) shall use immediate notification to signal the act of incrementing the semaphore value to any waiting processes.
        // c) shall unconditionally return the value 0.
        cout<< sc_time_stamp() << ": unlocked by thread_2, value is " << s.get_value() << endl;
        wait(SC_ZERO_TIME);
        }
    }


    void thread3(){
        while(true){
            if(s.trywait()== -1){ // if  sema > 0   -->  dec the sema  value  and return 0
                                  // if sems ==0  -->  return  -1  without change sema value

                s.wait();    //wait till resource is available
            }
         
         // int get_value(): shall return the semaphore value
        cout<< sc_time_stamp() << ": locked by thread 3, value is " << s.get_value() << endl;
        wait(1,SC_SEC);
        s.post();
        //a) shall increment the semaphore value.
        //b) shall use immediate notification to signal the act of incrementing the semaphore value to any waiting processes.
        // c) shall unconditionally return the value 0.
        cout<< sc_time_stamp() << ": unlocked by thread_3, value is " << s.get_value() << endl;
        wait(SC_ZERO_TIME);
        }
    }
};


int sc_main(int , char*[]){
    SEMAPHORE sema("sema");
    sc_start(4,SC_SEC);
    return 0;
}