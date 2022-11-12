#ifndef CAN_CONTROL_H
#define CAN_CONTROL_H

#include "can_message.h"
#include "can_receive.h"
#include "can_send.h"

enum canbus {can0 = 0, can1 = 1};

char *InitialiseCAN(enum canbus, int *);
void CloseCAN(enum canbus, int);

#endif
