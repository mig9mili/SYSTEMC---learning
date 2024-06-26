#ifndef TOP_H
#define TOP_H

#include "initiator.h"
#include "target.h"

SC_MODULE(Top)
{
  Initiator *initiator;
  TARGET *target;

  SC_CTOR(Top)
  {
    // Instantiate components
    initiator = new Initiator("initiator");
    target    = new TARGET  ("target");

    // One initiator is bound directly to one target with no intervening bus

    // Bind initiator socket to target socket
    initiator->socket.bind(target->socket);
  }
};

#endif
