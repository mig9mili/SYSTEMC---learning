#include"driver.h"

void  driver :: drive(void){
    while(true){

      //input
        wait(SC_ZERO_TIME);// delay cylce
        d_din = 1;
        wait(5,SC_SEC);
        d_din = 0;
        wait(5,SC_SEC);
      }
};