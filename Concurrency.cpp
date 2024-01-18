#include<stdio.h>
#include<systemc>

using namespace sc_core;
using namespace std;

//these processes are running concurrently on the same "simulated time"


//multiple processes are simulated as running concurrently,
// only one is executed at a particular time. However, 
//the simulated time remain unchanged until all concurrent processes finishes their current tasks.
SC_MODULE(CONCURENCY){
 SC_CTOR(CONCURENCY){
    SC_THREAD(thread1);
    SC_THREAD(thread2);
 }

void thread1(){
    while(true){
    cout<<sc_time_stamp()<<"thread1"<<endl;
    wait(2,SC_SEC);
    }
}
void thread2();
};

void CONCURENCY::thread2(){
    while(true){
        cout<<sc_time_stamp()<<"thread2"<<endl;
        wait(3,SC_SEC);
    }
}

int sc_main(int , char*[]){

    CONCURENCY concur("concur");

    sc_start(10,SC_SEC);

    return 0;

}



//output:-
/*
0 sthread1
0 sthread2
2 sthread1
3 sthread2
4 sthread1
6 sthread2
6 sthread1
8 sthread1
9 sthread2
*/