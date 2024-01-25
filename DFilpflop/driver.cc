#include "driver.h"

void driver :: drive(void){
    d_clr =0;
    d_din =0;

    wait(4,SC_NS);

    while(true){
        
        //wait(SC_ZERO_TIME);

        wait(10,SC_NS);
        d_din =1;
        
        wait(10,SC_NS);
        d_din =0;

        
    }
}