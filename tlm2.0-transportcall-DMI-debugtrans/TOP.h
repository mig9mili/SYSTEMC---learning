#ifndef TOP_H_
#define TOP_H_

#include"initiator.h"
#include"target.h"

SC_MODULE(TOP){
 Initiator *initiator;
 Memory   *memory;
 SC_CTOR(TOP){
    initiator = new Initiator("Initiator");
    memory = new Memory("memory");
   
   //One initiator is bound directly to one target with no intervening bus

    // Bind initiator socket to target socket
    initiator -> socket.bind(memory->socket);

 }
};
#endif
