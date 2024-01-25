#include"nandgate.h"

void NAND :: nand_gate(void){

    next_trigger();
    out = (!(A && B));  // NAND LOGIC
    

}