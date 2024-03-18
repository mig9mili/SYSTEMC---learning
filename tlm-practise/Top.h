#ifndef TOP_H_
#define  TOP_H_

#include "initiator.h"
#include "target.h"




SC_MODULE(Top){

    Initiator *initiator;
    Memory *memory;

    SC_CTOR(Top){
        initiator = new Initiator("initiator");
        memory = new Memory("memory");
        initiator->socket.bind(memory->socket);
    }

};
#endif