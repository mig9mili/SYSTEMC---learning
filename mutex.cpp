#include<systemc>
#include<iostream>

using namespace std;
using namespace sc_core;

SC_MODULE(MUTEX){
    sc_mutex m;
    SC_CTOR(MUTEX){
        SC_THREAD(thread1);
        SC_THREAD(thread2);

    }


     void thread1(){
        while(true){
            if(m.trylock() == -1){  //mutex is unlocked, trylock() shall lock the mutex and shall return the value 0  or else return -1
                m.lock();  //a) If the mutex is unlocked, lock() shall lock the mutex and return.
                           //b) If the mutex is locked, lock() shall suspend until the mutex is unlocked (by another process).


                cout<<sc_time_stamp()<<": thread 1 obtained resource by lock()"<<endl;
    
            }
            else{
                cout<<sc_time_stamp()<<": thread 1 obtained by trylock()"<<endl;
            }
            wait(1,SC_SEC);
            m.unlock();  //mutex is unlocked, unlock() shall return the value –1. The mutex shall remain unlocked.  or else retur 0;
    
    // note:-if the mutex was locked by a process instance other than the calling process, unlock() shall return the value –1. The mutex shall remain locked

            cout<<sc_time_stamp()<<": unlocked by thread 1"<<endl;
            wait(SC_ZERO_TIME);
        }
     }

  void  thread2(){
    while(true){
        if(m.trylock() ==-1){
            m.lock();
            cout << sc_time_stamp() << ": thread 2 obtained resource by lock()" << endl;
        }
        else{
            cout << sc_time_stamp() << ": thread 2 obtained resource by trylock()" << endl;
        }

        wait(1,SC_SEC);
        m.unlock();
        cout<<sc_time_stamp()<<": unlocked by thread 2"<<endl;
        wait(SC_ZERO_TIME);
    }
  }
};

int sc_main(int , char*[]){
    MUTEX mutex ("mutex");
    sc_start(4, SC_SEC);
    return 0;
}