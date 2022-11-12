#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H

#include "can_message.h"

int ReadCAN(int, struct can_frame *);
union gps_data_u GetGPSLatLonData(struct can_frame, int);

#endif
