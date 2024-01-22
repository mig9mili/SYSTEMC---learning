#include<iostream>
#include<systemc>
using namespace std;
using namespace sc_core;


//note:- f1 and f2 have same behavior, depite the block/non-blocking methods
//       read is slower than write, fifo soon becomes full. write speed reduces to same speed as read.

SC_MODULE(FIFO){
sc_fifo<int> f1,f2,f3;

SC_CTOR(FIFO) : f1(2),f2(2),f3(2){
  
  SC_THREAD(GENT1);
  SC_THREAD(CONS1);
  SC_THREAD(GENT2);
  SC_THREAD(CONS2);
  SC_THREAD(GENT3);
  SC_THREAD(CONS3);

  
  

}

 void GENT1(){
    int v=0;
   while(true){
   //blocking write
   // same as f=v
    f1.write(v);

    cout<<sc_time_stamp()<<" generator 1 writes "<<v++<<endl;
    wait(1,SC_SEC); // wait for 1 sec
 }
 }

void CONS1(){
    int v =-1;
        while(true){
            //blocking read
            // same as v=int(f)
            f1.read(v);
            cout<<sc_time_stamp()<<" consumer1 read"<<v<<endl;
           
           //read every 3 sec fifo will fill up
            wait(3,SC_SEC);

        }
    
}

void GENT2(){
    int v=0;
    while(true){
        //non-blocking write
        //nb write until it succeeded // if false wait(i.e fifo is full) for data read
        while(f2.nb_write(v)==false){
               wait(f2.data_read_event());
        }

        cout<<sc_time_stamp()<<" generator 2 writes"<<v++<<endl;
        wait(1,SC_SEC);
    }
}

void CONS2(){
    int v=-1;
    while(true){
        // non-blocking read
        //nb read until it  succeeded  // if false (i.e fifo is empty)wait for data write 
         while(f2.nb_read(v)==false){
            wait(f2.data_written_event());
         }
      cout<<sc_time_stamp()<<" consumer 2 reads"<<v<<endl;
      wait(3,SC_SEC);
    }
}

 
void GENT3(){
    int v=0;
    //free/available slots before/after write
    //int num_available(): returns the number of values that are available for reading in the current delta cycle
    //int num_free(): returns the number of empty slots that are free for writing in the current delta cycle
    while(true){
        cout<<sc_time_stamp()<<" generator 3 before write  free/available = "<<f3.num_free()<<"/"<<f3.num_available()<<endl;
        f3.write(v++);
        cout<<sc_time_stamp()<<" generator 3 after write  free/available = "<<f3.num_free()<<"/"<<f3.num_available()<<endl;
        wait(1,SC_SEC);
    }
}


void CONS3(){
    int v = -1;
    while(true){
        ////free/available slots before/after read
        cout<<sc_time_stamp()<<" consumer 3 before read  free/available = "<<f3.num_free()<<"/"<<f3.num_available()<<endl;
        f3.read(v);
        cout<<sc_time_stamp()<<" consumer 3 after read free/available = "<<f3.num_free()<<"/"<<f3.num_available()<<endl;
        wait(3,SC_SEC);
    }
}

};


int sc_main(int , char*[]){
    FIFO fifo("fifo");
    sc_start(10,SC_SEC);

return 0;
}